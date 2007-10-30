/*
 * convert.c
 * Convert _ped Python types to libparted typedefs.  Only typedef structs
 * need to be converted.  When a typedef in libparted is a primitive type,
 * we can just use it directly.
 *
 * Copyright (C) 2007  Red Hat, Inc.
 * All rights reserved.
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
#include "pyconstraint.h"
#include "pydevice.h"
#include "pygeom.h"
#include "pynatmath.h"
#include "pytimer.h"
#include "pyunit.h"

/* Note... all of these functions malloc for memory to create the returning
 * type.  It is up to the caller to free the memory after using it.
 */

/* _ped_Alignment -> PedAlignment functions */
PedAlignment *_ped_Alignment2PedAlignment(PyObject *s) {
    PedAlignment *ret;
    PedSector offset, grain_size;
    _ped_Alignment *alignment = (_ped_Alignment *) s;

    if (alignment == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Alignment()");
        return NULL;
    }

    offset = _ped_Sector2PedSector(alignment->offset);
    if (offset == -1) {
        return NULL;
    }

    grain_size = _ped_Sector2PedSector(alignment->grain_size);
    if (grain_size == -1) {
        return NULL;
    }

    ret = ped_alignment_new(offset, grain_size);
    if (ret == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Out of memory");
        return NULL;
    }

    return ret;
}

_ped_Alignment *PedAlignment2_ped_Alignment(PedAlignment *alignment) {
    _ped_Alignment *ret;

    ret = PyObject_New(_ped_Alignment, &_ped_Alignment_Type_obj);
    ret->offset = (PyObject *) PedSector2_ped_Sector(alignment->offset);
    ret->grain_size = (PyObject *) PedSector2_ped_Sector(alignment->grain_size);

    return ret;
}

/* _ped_Constraint -> PedConstraint functions */
PedConstraint *_ped_Constraint2PedConstraint(PyObject *s) {
    PedConstraint *ret;
    PedAlignment *start_align, *end_align;
    PedGeometry *start_range, *end_range;
    PedSector min_size, max_size;
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
        return NULL;
    }

    start_range = _ped_Geometry2PedGeometry(constraint->start_range);
    if (start_range == NULL) {
        return NULL;
    }

    end_range = _ped_Geometry2PedGeometry(constraint->end_range);
    if (end_range == NULL) {
        return NULL;
    }

    min_size = _ped_Sector2PedSector(constraint->min_size);
    if (min_size == -1) {
        return NULL;
    }

    max_size = _ped_Sector2PedSector(constraint->max_size);
    if (max_size == -1) {
        return NULL;
    }

    ret = ped_constraint_new(start_align, end_align, start_range, end_range,
                             min_size, max_size);
    if (ret == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Out of memory");
        return NULL;
    }

    ped_alignment_destroy(start_align);
    ped_alignment_destroy(end_align);
    ped_geometry_destroy(start_range);
    ped_geometry_destroy(end_range);

    return ret;
}

_ped_Constraint *PedConstraint2_ped_Constraint(PedConstraint *constraint) {
    _ped_Constraint *ret;

    ret = PyObject_New(_ped_Constraint, &_ped_Constraint_Type_obj);
    ret->start_align = (PyObject *) PedAlignment2_ped_Alignment(constraint->start_align);
    ret->end_align = (PyObject *) PedAlignment2_ped_Alignment(constraint->end_align);
    ret->start_range = (PyObject *) PedGeometry2_ped_Geometry(constraint->start_range);
    ret->end_range = (PyObject *) PedGeometry2_ped_Geometry(constraint->end_range);
    ret->min_size = (PyObject *) PedSector2_ped_Sector(constraint->min_size);
    ret->max_size = (PyObject *) PedSector2_ped_Sector(constraint->max_size);

    return ret;
}

/* _ped_Device -> PedDevice functions */
PedDevice *_ped_Device2PedDevice(PyObject *s) {
    PedDevice *ret;
    _ped_Device *dev = (_ped_Device *) s;

    if (dev == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Device()");
        return NULL;
    }

    if (dev->path) {
        ret = ped_device_get(dev->path);
        if (ret == NULL) {
            PyErr_SetString(PyExc_MemoryError, "Out of memory");
            return NULL;
        }
    } else {
        return NULL;
    }

    return ret;
}

/* PedDevice -> _ped_Device functions */
_ped_Device *PedDevice2_ped_Device(PedDevice *device) {
    _ped_Device *ret;

    ret = PyObject_New(_ped_Device, &_ped_Device_Type_obj);
    ret->model = strdup(device->model);
    ret->path = strdup(device->path);
    ret->type = device->type;
    ret->sector_size = device->sector_size;
    ret->phys_sector_size = device->phys_sector_size;
    ret->length = (PyObject *) PedSector2_ped_Sector(device->length);
    ret->open_count = device->open_count;
    ret->read_only = device->read_only;
    ret->external_mode = device->external_mode;
    ret->dirty = device->dirty;
    ret->boot_dirty = device->boot_dirty;
    ret->hw_geom = (PyObject *) PedCHSGeometry2_ped_CHSGeometry(&device->hw_geom);
    ret->bios_geom = (PyObject *) PedCHSGeometry2_ped_CHSGeometry(&device->bios_geom);
    ret->host = device->host;
    ret->did = device->did;

    /* XXX: don't know what to do with these */
    ret->next = device->next;
    ret->arch_specific = device->arch_specific;

    return ret;
}

/* _ped_FileSystem -> PedFileSystem functions */
PedFileSystem *_ped_FileSystem2PedFileSystem(PyObject *s) {
    PedFileSystem *ret;
    PedFileSystemType *fstype;
    PedGeometry *geom;
    _ped_FileSystem *fs = (_ped_FileSystem *) s;

    if (fs == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.FileSystem");
        return NULL;
    }

    fstype = _ped_FileSystemType2PedFileSystemType(fs->type);
    if (fstype == NULL) {
        return NULL;
    }

    geom = _ped_Geometry2PedGeometry(fs->geom);
    if (geom == NULL) {
        return NULL;
    }

    ret->type = fstype;
    ret->geom = geom;
    ret->checked = fs->checked;

    /* don't know what to do with these */
    ret->type_specific = fs->type_specific;

    return ret;
}

_ped_FileSystem *PedFileSystem2_ped_FileSystem(PedFileSystem *fs) {
    _ped_FileSystem *ret;

    ret = PyObject_New(_ped_FileSystem, &_ped_FileSystem_Type_obj);
    ret->type = (PyObject *) PedFileSystemType2_ped_FileSystemType(fs->type);
    ret->geom = (PyObject *) PedGeometry2_ped_Geometry(fs->geom);
    ret->checked = fs->checked;

    /* XXX: don't know what to do with these */
    ret->type_specific = fs->type_specific;

    return ret;
}

/* _ped_FileSystemType -> PedFileSystemType functions */
PedFileSystemType *_ped_FileSystemType2PedFileSystemType(PyObject *s) {
    /* XXX */
    return NULL;
}

_ped_FileSystemType *PedFileSystemType2_ped_FileSystemType(PedFileSystemType *fstype) {
    /* XXX */
    return NULL;
}

/* _ped_Geometry -> PedGeometry functions */
PedGeometry *_ped_Geometry2PedGeometry(PyObject *s) {
    PedGeometry *ret;
    PedDevice *dev;
    PedSector start, length;
    _ped_Geometry *geometry = (_ped_Geometry *) s;

    if (geometry == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Geometry()");
        return NULL;
    }

    dev = _ped_Device2PedDevice(geometry->dev);
    if (dev == NULL) {
        return NULL;
    }

    start = _ped_Sector2PedSector(geometry->start);
    if (start == -1) {
        return NULL;
    }

    length = _ped_Sector2PedSector(geometry->length);
    if (length == -1) {
        return NULL;
    }

    ret = ped_geometry_new(dev, start, length);
    if (ret == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Out of memory");
        return NULL;
    }

    /* do not ped_destroy_device(dev) here because ret contains a ptr to it */

    return ret;
}

_ped_Geometry *PedGeometry2_ped_Geometry(PedGeometry *geometry) {
    _ped_Geometry *ret;

    ret = PyObject_New(_ped_Geometry, &_ped_Geometry_Type_obj);
    ret->dev = (PyObject *) PedDevice2_ped_Device(geometry->dev);
    ret->start = (PyObject *) PedSector2_ped_Sector(geometry->start);
    ret->length = (PyObject *) PedSector2_ped_Sector(geometry->length);
    ret->end = (PyObject *) PedSector2_ped_Sector(geometry->end);
}

/* _ped_CHSGeometry -> PedCHSGeometry functions */
PedCHSGeometry *_ped_CHSGeometry2PedCHSGeometry(PyObject *s) {
    PedCHSGeometry *ret;
    _ped_CHSGeometry *srcgeom = (_ped_CHSGeometry *) s;

    if (srcgeom == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.CHSGeometry()");
        return;
    }

    ret = malloc(sizeof(PedCHSGeometry));
    if (ret == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Out of memory");
        return NULL;
    }

    ret->cylinders = srcgeom->cylinders;
    ret->heads = srcgeom->heads;
    ret->sectors = srcgeom->sectors;

    return ret;
}

/* PedCHSGeometry -> _ped_CHSGeometry functions */
_ped_CHSGeometry *PedCHSGeometry2_ped_CHSGeometry(PedCHSGeometry *geom) {
    _ped_CHSGeometry *ret;

    ret = PyObject_New(_ped_CHSGeometry, &_ped_CHSGeometry_Type_obj);
    ret->cylinders = geom->cylinders;
    ret->heads = geom->heads;
    ret->sectors = geom->sectors;

    return ret;
}

/* _ped_Sector -> PedSector functions */
PedSector _ped_Sector2PedSector(PyObject *s) {
    PedSector ret;
    _ped_Sector *sector = (_ped_Sector *) s;

    if (sector == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Sector()");
        return -1;
    }

    ret = sector->val;
    return ret;
}

/* PedSector -> _ped_Sector functions */
_ped_Sector *PedSector2_ped_Sector(PedSector s) {
    _ped_Sector *ret;

    ret = PyObject_New(_ped_Sector, &_ped_Sector_Type_obj);
    ret->val = s;
    return ret;
}

/* _ped_Timer -> PedTimer functions */
PedTimer *_ped_Timer2PedTimer(PyObject *s) {
    PedTimer *ret;
    _ped_Timer *timer = (_ped_Timer *) s;

    if (timer == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Timer()");
        return NULL;
    }

    if ((ret = malloc(sizeof(PedTimer))) == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Out of memory");
        return NULL;
    }

    ret->frac = timer->frac;
    ret->start = timer->start;
    ret->now = timer->now;
    ret->predicted_end = timer->predicted_end;
    ret->state_name = strdup(timer->state_name);
    ret->handler = timer->handler;
    ret->context = timer->context;

    return ret;
}

/* PedTimer -> _ped_Timer functions */
_ped_Timer *PedTimer2_ped_timer(PedTimer *timer) {
    _ped_Timer *ret;

    ret = PyObject_New(_ped_Timer, &_ped_Timer_Type_obj);
    ret->frac = timer->frac;
    ret->start = timer->start;
    ret->now = timer->now;
    ret->predicted_end = timer->predicted_end;
    ret->state_name = strdup(timer->state_name);

    /* XXX: don't know what to do with these */
    ret->handler = timer->handler;
    ret->context = timer->context;

    return ret;
}

/* _ped_Unit -> PedUnit functions */
PedUnit _ped_Unit2PedUnit(PyObject *s) {
    PedUnit ret;
    _ped_Unit *unit = (_ped_Unit *) s;

    if (unit == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped_Unit()");
        return -1;
    }

    ret = unit->val;
    return ret;
}

/* PedUnit -> _ped_Unit function s*/
_ped_Unit *PedUnit2_ped_Unit(PedUnit s) {
    _ped_Unit *ret;

    ret = PyObject_New(_ped_Unit, &_ped_Unit_Type_obj);
    ret->val = s;
    return ret;
}
