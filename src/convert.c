/*
 * convert.c
 * Convert _ped Python types to libparted typedefs.  Only typedef structs
 * need to be converted.  When a typedef in libparted is a primitive type,
 * we can just use it directly.
 *
 * Copyright (C) 2007, 2008  Red Hat, Inc.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions of
 * the GNU General Public License v.2, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY expressed or implied, including the implied warranties of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.  You should have received a copy of the
 * GNU General Public License along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.  Any Red Hat trademarks that are incorporated in the
 * source code or documentation are not subject to the GNU General Public
 * License and may only be used or replicated with the express permission of
 * Red Hat, Inc.
 *
 * Red Hat Author(s): David Cantrell <dcantrell@redhat.com>
 *                    Chris Lumens <clumens@redhat.com>
 */

#include <Python.h>

#include "convert.h"
#include "exceptions.h"
#include "pyconstraint.h"
#include "pydevice.h"
#include "pygeom.h"
#include "pynatmath.h"
#include "pytimer.h"
#include "pyunit.h"

/*
 * These functions convert between pyparted Python types and libparted types.
 * Some are structures, some are simply C primitives.  Important notes:
 *
 * 1) When using a _ped_X2Y() function, you are converting a pyparted Python
 *    type to a libparted type.  If the function returns a pointer, you need
 *    to free it when you are done using it.
 * 2) When using a PedX2_ped_Y() function, you are converting a libparted
 *    type to a pyparted Python type.  You will get a pointer to a PyObject
 *    back, but don't free this variable.  Python will handle clean up of
 *    these variables through reference counts.
 * 3) Some functions return C primitives, so no memory management needs to
 *    be done.
 *
 * Error handling notes:
 *
 * 1) When converting from a PyObject, first check for NULL and raise a
 *    PyExc_Type error.
 * 2) When convertion from a Ped*, first check for NULL and raise a
 *    PyExc_Type error.
 * 3) Check the return value of all calls to other convert.c functions and
 *    return NULL on error, but do not raise an exception.
 * 4) Check the return value of PyObject_new, malloc, strdup, and any other
 *    functions that allocate memory and call PyErr_NoMemory on error.
 * 5) When calling a libparted function, check for error conditions and
 *    raise the appropriate exceptions.  Create new exceptions if needed.
 * 6) At the end of a conversion function, make sure the return value is
 *    not NULL.  Raise the appropriate exception if it is.
 */

/* _ped_Alignment -> PedAlignment functions */
PedAlignment *_ped_Alignment2PedAlignment(PyObject *s) {
    PedAlignment *ret = NULL;
    _ped_Alignment *alignment = (_ped_Alignment *) s;

    if (alignment == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Alignment()");
        return NULL;
    }

    ret = ped_alignment_new(alignment->offset, alignment->grain_size);
    if (ret == NULL)
        return (PedAlignment *) PyErr_NoMemory();

    return ret;
}

_ped_Alignment *PedAlignment2_ped_Alignment(PedAlignment *alignment) {
    _ped_Alignment *ret = NULL;
    PyObject *args = NULL;

    if (alignment == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty PedAlignment()");
        return NULL;
    }

    ret = (_ped_Alignment *) _ped_Alignment_Type_obj.tp_new(&_ped_Alignment_Type_obj, NULL, NULL);
    if (!ret)
        return (_ped_Alignment *) PyErr_NoMemory();

    args = Py_BuildValue("ll", alignment->offset, alignment->grain_size);

    if (_ped_Alignment_Type_obj.tp_init((PyObject *) ret, args, NULL)) {
        PyErr_SetString(PyExc_TypeError, "Failed to initialize _ped.Alignment");
        return NULL;
    }

    return ret;
}

/* _ped_Constraint -> PedConstraint functions */
PedConstraint *_ped_Constraint2PedConstraint(PyObject *s) {
    PedConstraint *ret = NULL;
    PedAlignment *start_align = NULL, *end_align = NULL;
    PedGeometry *start_range = NULL, *end_range = NULL;
    _ped_Constraint *constraint = (_ped_Constraint *) s;

    if (constraint == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Constraint()");
        return NULL;
    }

    start_align = _ped_Alignment2PedAlignment(constraint->start_align);
    if (start_align == NULL) {
        return NULL;
    }

    end_align = _ped_Alignment2PedAlignment(constraint->end_align);
    if (end_align == NULL) {
        ped_alignment_destroy(start_align);
        return NULL;
    }

    start_range = _ped_Geometry2PedGeometry(constraint->start_range);
    if (start_range == NULL) {
        ped_alignment_destroy(start_align);
        ped_alignment_destroy(end_align);
        return NULL;
    }

    end_range = _ped_Geometry2PedGeometry(constraint->end_range);
    if (end_range == NULL) {
        ped_alignment_destroy(start_align);
        ped_alignment_destroy(end_align);
        ped_geometry_destroy(start_range);
        return NULL;
    }

    ret = ped_constraint_new(start_align, end_align, start_range, end_range,
                             constraint->min_size, constraint->max_size);
    if (ret == NULL) {
        /* Fall through to clean up memory, but set the error condition now. */
        PyErr_NoMemory();
    }

    ped_alignment_destroy(start_align);
    ped_alignment_destroy(end_align);
    ped_geometry_destroy(start_range);
    ped_geometry_destroy(end_range);

    return ret;
}

_ped_Constraint *PedConstraint2_ped_Constraint(PedConstraint *constraint) {
    _ped_Constraint *ret = NULL;
    _ped_Alignment *start_align = NULL;
    _ped_Alignment *end_align = NULL;
    _ped_Geometry *start_range = NULL;
    _ped_Geometry *end_range = NULL;
    PyObject *args = NULL;

    if (constraint == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty PedConstraint()");
        return NULL;
    }

    ret = (_ped_Constraint *) _ped_Constraint_Type_obj.tp_new(&_ped_Constraint_Type_obj, NULL, NULL);
    if (!ret)
        return (_ped_Constraint *) PyErr_NoMemory();

    if ((start_align = PedAlignment2_ped_Alignment(constraint->start_align)) == NULL)
        return NULL;

    if ((end_align = PedAlignment2_ped_Alignment(constraint->end_align)) == NULL)
        return NULL;

    if ((start_range = PedGeometry2_ped_Geometry(constraint->start_range)) == NULL)
        return NULL;

    if ((end_range = PedGeometry2_ped_Geometry(constraint->end_range)) == NULL)
        return NULL;

    args = Py_BuildValue("OOOOll", start_align, end_align, start_range, end_range, constraint->min_size, constraint->max_size);

    if (_ped_Constraint_Type_obj.tp_init((PyObject *) ret, args, NULL)) {
        PyErr_SetString(PyExc_TypeError, "Failed to initialize _ped.Constraint");
        return NULL;
    }

    return ret;
}

/* _ped_Device -> PedDevice functions */
PedDevice *_ped_Device2PedDevice(PyObject *s) {
    PedDevice *ret = NULL;
    _ped_Device *dev = (_ped_Device *) s;

    if (dev == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Device()");
        return NULL;
    }

    ret = dev->ped_device;
    return ret;
}

/* PedDevice -> _ped_Device functions */
_ped_Device *PedDevice2_ped_Device(PedDevice *device) {
    _ped_Device *ret = NULL;

    if (device == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty PedDevice");
        return NULL;
    }

    ret = (_ped_Device *) _ped_Device_Type_obj.tp_alloc(&_ped_Device_Type_obj, 1);
    if (!ret)
        return (_ped_Device *) PyErr_NoMemory();

    ret->model = strdup(device->model);
    if (ret->model == NULL) {
        PyObject_GC_Del(ret);
        return (_ped_Device *) PyErr_NoMemory();
    }

    ret->path = strdup(device->path);
    if (ret->path == NULL) {
        PyObject_GC_Del(ret);
        return (_ped_Device *) PyErr_NoMemory();
    }

    ret->type = device->type;
    ret->sector_size = device->sector_size;
    ret->phys_sector_size = device->phys_sector_size;
    ret->open_count = device->open_count;
    ret->read_only = device->read_only;
    ret->external_mode = device->external_mode;
    ret->dirty = device->dirty;
    ret->boot_dirty = device->boot_dirty;
    ret->host = device->host;
    ret->did = device->did;
    ret->length = device->length;

    ret->hw_geom = (PyObject *) PedCHSGeometry2_ped_CHSGeometry(&device->hw_geom);
    if (ret->hw_geom == NULL)
        return NULL;
    Py_INCREF(ret->hw_geom);

    ret->bios_geom = (PyObject *) PedCHSGeometry2_ped_CHSGeometry(&device->bios_geom);
    if (ret->bios_geom == NULL)
        return NULL;
    Py_INCREF(ret->bios_geom);

    ret->ped_device = device;

    return ret;
}

PedDisk *_ped_Disk2PedDisk(PyObject *s) {
    PedDisk *ret = NULL;
    PedDevice *dev = NULL;
    _ped_Disk *disk = (_ped_Disk *) s;

    if (disk == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Disk()");
        return NULL;
    }

    dev = _ped_Device2PedDevice(disk->dev);
    if (dev == NULL) {
        return NULL;
    }

    ret = ped_disk_new(dev);
    if (ret == NULL) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (PyErr_ExceptionMatches(PartedException) ||
                PyErr_ExceptionMatches(PyExc_NotImplementedError))
                return NULL;

            PyErr_SetString(DiskException, partedExnMessage);
            return NULL;
        }
    }

    return ret;
}

_ped_Disk *PedDisk2_ped_Disk(PedDisk *disk) {
    _ped_Disk *ret = NULL;
    _ped_Device *dev = NULL;
    _ped_DiskType *type = NULL;
    PyObject *args = NULL;

    if (disk == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty PedDisk()");
        return NULL;
    }

    ret = (_ped_Disk *) _ped_Disk_Type_obj.tp_new(&_ped_Disk_Type_obj, NULL, NULL);
    if (!ret)
        return (_ped_Disk *) PyErr_NoMemory();

    if ((dev = PedDevice2_ped_Device(disk->dev)) == NULL)
        return NULL;

    if ((type = PedDiskType2_ped_DiskType((PedDiskType *) disk->type)) == NULL)
        return NULL;

    args = Py_BuildValue("OO", (PyObject *) dev, (PyObject *) type);

    if (_ped_Disk_Type_obj.tp_init((PyObject *) ret, args, NULL)) {
        PyErr_SetString(PyExc_TypeError, "Failed to initialize _ped.Disk");
        return NULL;
    }

    return ret;
}

PedDiskType *_ped_DiskType2PedDiskType(PyObject *s) {
    PedDiskType *ret = NULL;
    _ped_DiskType *type = (_ped_DiskType *) s;

    if (type == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.DiskType()");
        return NULL;
    }

    ret = ped_disk_type_get(type->name);
    if (ret == NULL) {
        PyErr_SetString(UnknownTypeException, type->name);
        return NULL;
    }

    return ret;
}

_ped_DiskType *PedDiskType2_ped_DiskType(PedDiskType *type) {
    _ped_DiskType *ret = NULL;

    if (type == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty PedDiskType()");
        return NULL;
    }

    ret = (_ped_DiskType *) _ped_DiskType_Type_obj.tp_alloc(&_ped_DiskType_Type_obj, 1);
    if (!ret)
        return (_ped_DiskType *) PyErr_NoMemory();

    ret->name = strdup(type->name);
    if (ret->name == NULL) {
        return (_ped_DiskType *) PyErr_NoMemory();
    }

    ret->features = type->features;

    return ret;
}

/* _ped_FileSystem -> PedFileSystem functions */
PedFileSystem *_ped_FileSystem2PedFileSystem(PyObject *s) {
    PedFileSystem *ret = NULL;
    PedGeometry *geom = NULL;
    _ped_FileSystem *fs = (_ped_FileSystem *) s;

    if (fs == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.FileSystem");
        return NULL;
    }

    geom = _ped_Geometry2PedGeometry(fs->geom);
    if (geom == NULL) {
        return NULL;
    }

    ret = ped_file_system_open(geom);
    if (ret == NULL) {
        ped_geometry_destroy(geom);

        if (partedExnRaised) {
            partedExnRaised = 0;

            if (PyErr_ExceptionMatches(PartedException) ||
                PyErr_ExceptionMatches(PyExc_NotImplementedError))
                return NULL;

            PyErr_SetString(FileSystemException, partedExnMessage);
            return NULL;
        }
    }

    return ret;
}

_ped_FileSystem *PedFileSystem2_ped_FileSystem(PedFileSystem *fs) {
    _ped_FileSystem *ret = NULL;
    _ped_FileSystemType *type = NULL;
    _ped_Geometry *geom = NULL;
    PyObject *args = NULL;

    if (fs == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty PedFileSystem()");
        return NULL;
    }

    ret = (_ped_FileSystem *) _ped_FileSystem_Type_obj.tp_new(&_ped_FileSystem_Type_obj, NULL, NULL);
    if (!ret)
        return (_ped_FileSystem *) PyErr_NoMemory();

    if ((type = PedFileSystemType2_ped_FileSystemType(fs->type)) == NULL)
        return NULL;

    if ((geom = PedGeometry2_ped_Geometry(fs->geom)) == NULL)
        return NULL;

    args = Py_BuildValue("OOl", type, geom, fs->checked);

    if (_ped_FileSystem_Type_obj.tp_init((PyObject *) ret, args, NULL)) {
        PyErr_SetString(PyExc_TypeError, "Failed to initialize _ped.FileSystem");
        return NULL;
    }

    return ret;
}

/* _ped_FileSystemType -> PedFileSystemType functions */
PedFileSystemType *_ped_FileSystemType2PedFileSystemType(PyObject *s) {
    PedFileSystemType *ret = NULL;
    _ped_FileSystemType *type = (_ped_FileSystemType *) s;

    if (type == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.FileSystemType");
        return NULL;
    }

    if ((ret = ped_file_system_type_get(type->name)) == NULL) {
        PyErr_SetString(UnknownTypeException, type->name);
        return NULL;
    }

    return ret;
}

_ped_FileSystemType *PedFileSystemType2_ped_FileSystemType(PedFileSystemType *fstype) {
    _ped_FileSystemType *ret = NULL;

    if (fstype == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty PedFileSystemType()");
        return NULL;
    }

    ret = (_ped_FileSystemType *) _ped_FileSystemType_Type_obj.tp_alloc(&_ped_FileSystemType_Type_obj, 1);
    if (!ret)
       return (_ped_FileSystemType *) PyErr_NoMemory();

    ret->name = strdup(fstype->name);
    if (ret->name == NULL) {
        return (_ped_FileSystemType *) PyErr_NoMemory();
    }

    return ret;
}

/* _ped_Geometry -> PedGeometry functions */
PedGeometry *_ped_Geometry2PedGeometry(PyObject *s) {
    PedGeometry *ret = NULL;
    PedDevice *dev = NULL;
    _ped_Geometry *geometry = (_ped_Geometry *) s;

    if (geometry == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Geometry()");
        return NULL;
    }

    dev = _ped_Device2PedDevice(geometry->dev);
    if (dev == NULL) {
        return NULL;
    }

    ret = ped_geometry_new(dev, geometry->start, geometry->length);
    if (ret == NULL) {
        return (PedGeometry *) PyErr_NoMemory();
    }

    return ret;
}

_ped_Geometry *PedGeometry2_ped_Geometry(PedGeometry *geometry) {
    _ped_Geometry *ret = NULL;
    _ped_Device *dev = NULL;
    PyObject *args = NULL;

    if (geometry == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty PedGeometry()");
        return NULL;
    }

    ret = (_ped_Geometry *) _ped_Geometry_Type_obj.tp_new(&_ped_Geometry_Type_obj, NULL, NULL);
    if (!ret)
        return (_ped_Geometry *) PyErr_NoMemory();

    if ((dev = PedDevice2_ped_Device(geometry->dev)) == NULL)
        return NULL;

    args = Py_BuildValue("Olll", dev, geometry->start, geometry->length, geometry->end);

    if (_ped_Geometry_Type_obj.tp_init((PyObject *) ret, args, NULL)) {
        PyErr_SetString(PyExc_TypeError, "Failed to initialize _ped.Geometry");
        return NULL;
    }

    return ret;
}

/* _ped_CHSGeometry -> PedCHSGeometry functions */
PedCHSGeometry *_ped_CHSGeometry2PedCHSGeometry(PyObject *s) {
    PedCHSGeometry *ret = NULL;
    _ped_CHSGeometry *srcgeom = (_ped_CHSGeometry *) s;

    if (srcgeom == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.CHSGeometry()");
        return NULL;
    }

    if ((ret = malloc(sizeof(PedCHSGeometry))) == NULL)
        return (PedCHSGeometry *) PyErr_NoMemory();

    ret->cylinders = srcgeom->cylinders;
    ret->heads = srcgeom->heads;
    ret->sectors = srcgeom->sectors;

    return ret;
}

/* PedCHSGeometry -> _ped_CHSGeometry functions */
_ped_CHSGeometry *PedCHSGeometry2_ped_CHSGeometry(PedCHSGeometry *geom) {
    _ped_CHSGeometry *ret = NULL;

    if (geom == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty PedCHSGeometry()");
        return NULL;
    }

    ret = (_ped_CHSGeometry *) _ped_CHSGeometry_Type_obj.tp_alloc(&_ped_CHSGeometry_Type_obj, 1);
    if (!ret)
        return (_ped_CHSGeometry *) PyErr_NoMemory();

    ret->cylinders = geom->cylinders;
    ret->heads = geom->heads;
    ret->sectors = geom->sectors;

    return ret;
}

PedPartition *_ped_Partition2PedPartition(PyObject *s) {
    PedPartition *ret = NULL;
    PedDisk *disk = NULL;
    PedFileSystemType *fs_type = NULL;
    PedGeometry *tmpgeom = NULL;
    _ped_Partition *part = (_ped_Partition *) s;

    if (part == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Partition()");
        return NULL;
    }

    disk = _ped_Disk2PedDisk(part->disk);
    if (disk == NULL) {
        return NULL;
    }

    fs_type = _ped_FileSystemType2PedFileSystemType(part->fs_type);
    if (fs_type == NULL) {
        ped_disk_destroy(disk);
        return NULL;
    }

    tmpgeom = _ped_Geometry2PedGeometry(part->geom);
    if (tmpgeom == NULL) {
        ped_disk_destroy(disk);
        return NULL;
    }

    if ((ret = ped_partition_new(disk, part->type, fs_type, tmpgeom->start,
                                 tmpgeom->end)) == NULL) {
        ped_disk_destroy(disk);
        ped_geometry_destroy(tmpgeom);

        if (partedExnRaised) {
            partedExnRaised = 0;

            if (PyErr_ExceptionMatches(PartedException) ||
                PyErr_ExceptionMatches(PyExc_NotImplementedError))
                return NULL;

            PyErr_SetString(PartitionException, partedExnMessage);
            return NULL;
        }
    }

    return ret;
}

_ped_Partition *PedPartition2_ped_Partition(PedPartition *part) {
    _ped_Partition *ret = NULL;
    _ped_Disk *disk = NULL;
    _ped_FileSystemType *fs_type = NULL;
    PyObject *args = NULL;

    if (part == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty PedPartition()");
        return NULL;
    }

    ret = (_ped_Partition *) _ped_Partition_Type_obj.tp_new(&_ped_Partition_Type_obj, NULL, NULL);
    if (!ret)
        return (_ped_Partition *) PyErr_NoMemory();

    if ((disk = PedDisk2_ped_Disk(part->disk)) == NULL)
        return NULL;

    if ((fs_type = PedFileSystemType2_ped_FileSystemType((PedFileSystemType *) part->fs_type)) == NULL)
        return NULL;

    args = Py_BuildValue("OlOll", disk, part->type, fs_type, part->geom.start, part->geom.end);

    if (_ped_Partition_Type_obj.tp_init((PyObject *) ret, args, NULL)) {
        PyErr_SetString(PyExc_TypeError, "Failed to initialize _ped.Partition");
        return NULL;
    }

    return ret;
}

/* _ped_Timer -> PedTimer functions */
PedTimer *_ped_Timer2PedTimer(PyObject *s) {
    PedTimer *ret = NULL;
    _ped_Timer *timer = (_ped_Timer *) s;

    if (timer == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Timer()");
        return NULL;
    }

    if ((ret = malloc(sizeof(PedTimer))) == NULL)
        return (PedTimer *) PyErr_NoMemory();

    ret->frac = timer->frac;
    ret->start = timer->start;
    ret->now = timer->now;
    ret->predicted_end = timer->predicted_end;
    ret->handler = timer->handler;
    ret->context = timer->context;

    ret->state_name = strdup(timer->state_name);
    if (ret->state_name == NULL) {
        free(ret);
        return (PedTimer *) PyErr_NoMemory();
    }

    return ret;
}

/* PedTimer -> _ped_Timer functions */
_ped_Timer *PedTimer2_ped_Timer(PedTimer *timer) {
    _ped_Timer *ret = NULL;

    if (timer == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty PedTimer()");
        return NULL;
    }

    ret = (_ped_Timer *) _ped_Timer_Type_obj.tp_new(&_ped_Timer_Type_obj, NULL, NULL);
    if (!ret)
        return (_ped_Timer *) PyErr_NoMemory();

    ret->frac = timer->frac;
    ret->start = timer->start;
    ret->now = timer->now;
    ret->predicted_end = timer->predicted_end;

    ret->state_name = strdup(timer->state_name);
    if (ret->state_name == NULL) {
        PyObject_GC_Del(ret);
        return NULL;
    }

    /* XXX: don't know what to do with these */
    ret->handler = timer->handler;
    ret->context = timer->context;

    return ret;
}

/* vim:tw=78:ts=4:et:sw=4
 */
