/*
 * pydisk.c
 *
 * Copyright (C) 2007-2013 Red Hat, Inc.
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
 * Author(s): David Cantrell <dcantrell@redhat.com>
 *            Chris Lumens <clumens@redhat.com>
 *            Alex Skinner <alex@lx.lc>
 */

#include <Python.h>

#include <stdlib.h>

#include "convert.h"
#include "exceptions.h"
#include "pydisk.h"
#include "docstrings/pydisk.h"
#include "typeobjects/pydisk.h"

/* _ped.Partition functions */
void _ped_Partition_dealloc(_ped_Partition *self) {
    PyObject_GC_UnTrack(self);

    Py_CLEAR(self->disk);
    self->disk = NULL;

    Py_CLEAR(self->geom);
    self->geom = NULL;

    Py_CLEAR(self->fs_type);
    self->fs_type = NULL;

    PyObject_GC_Del(self);
}

int _ped_Partition_compare(_ped_Partition *self, PyObject *obj) {
    _ped_Partition *comp = NULL;
    int check = PyObject_IsInstance(obj, (PyObject *) &_ped_Partition_Type_obj);

    if (PyErr_Occurred()) {
        return -1;
    }

    if (!check) {
        PyErr_SetString(PyExc_ValueError, "object comparing to must be a _ped.Partition");
        return -1;
    }

    comp = (_ped_Partition *) obj;
    if ((_ped_Disk_Type_obj.tp_richcompare(self->disk, comp->disk, Py_EQ)) &&
        (_ped_Geometry_Type_obj.tp_richcompare(self->geom, comp->geom, Py_EQ)) &&
        (self->ped_partition->num == comp->ped_partition->num) &&
        (self->type == comp->type) &&
        (_ped_FileSystemType_Type_obj.tp_richcompare(self->fs_type, comp->fs_type, Py_EQ))) {
        return 0;
    } else {
        return 1;
    }
}

PyObject *_ped_Partition_richcompare(_ped_Partition *a, PyObject *b, int op) {
    if (op == Py_EQ) {
        if ((_ped_Partition_Type_obj.tp_richcompare((PyObject *) a, b, Py_EQ))) {
            Py_RETURN_TRUE;
        } else {
            Py_RETURN_FALSE;
        }
    } else if (op == Py_NE) {
        if (!(_ped_Partition_Type_obj.tp_richcompare((PyObject *) a, b, Py_EQ))) {
            Py_RETURN_TRUE;
        } else {
            Py_RETURN_FALSE;
        }
    } else if ((op == Py_LT) || (op == Py_LE) ||
               (op == Py_GT) || (op == Py_GE)) {
        PyErr_SetString(PyExc_TypeError, "comparison operator not supported for _ped.Partition");
        return NULL;
    } else {
        PyErr_SetString(PyExc_ValueError, "unknown richcompare op");
        return NULL;
    }
}

PyObject *_ped_Partition_str(_ped_Partition *self) {
    char *ret = NULL;
    char *disk = NULL, *fs_type = NULL, *geom = NULL;

    disk = (char *) PyUnicode_AsUTF8(_ped_Disk_Type_obj.tp_repr(self->disk));
    if (disk == NULL) {
        return NULL;
    }

    fs_type = (char *) PyUnicode_AsUTF8(_ped_FileSystemType_Type_obj.tp_repr(self->fs_type));
    if (fs_type == NULL) {
        return NULL;
    }

    geom = (char *) PyUnicode_AsUTF8(_ped_Geometry_Type_obj.tp_repr(self->geom));
    if (geom == NULL) {
        return NULL;
    }

    if (asprintf(&ret, "_ped.Partition instance --\n"
                       "  disk: %s  fs_type: %s\n"
                       "  num: %d  type: %d\n"
                       "  geom: %s",
                 disk, fs_type, self->ped_partition->num, self->type,
                 geom) == -1) {
        return PyErr_NoMemory();
    }

    return Py_BuildValue("s", ret);
}

int _ped_Partition_traverse(_ped_Partition *self, visitproc visit, void *arg) {
    int err;

    if (self->disk) {
        if ((err = visit(self->disk, arg))) {
            return err;
        }
    }

    if (self->geom) {
        if ((err = visit(self->geom, arg))) {
            return err;
        }
    }

    if (self->fs_type) {
        if ((err = visit(self->fs_type, arg))) {
            return err;
        }
    }

    return 0;
}

int _ped_Partition_clear(_ped_Partition *self) {
    Py_CLEAR(self->disk);
    self->disk = NULL;

    Py_CLEAR(self->geom);
    self->geom = NULL;

    Py_CLEAR(self->fs_type);
    self->fs_type = NULL;

    return 0;
}

int _ped_Partition_init(_ped_Partition *self, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"disk", "type", "start", "end", "fs_type", NULL};
    PedSector start, end;
    PedDisk *disk = NULL;
    PedFileSystemType *fstype = NULL;
    PedPartition *part = NULL;

    self->fs_type = Py_None;

    if (kwds == NULL) {
        if (!PyArg_ParseTuple(args, "O!iLL|O!",
                              &_ped_Disk_Type_obj, &self->disk,
                              &self->type, &start, &end,
                              &_ped_FileSystemType_Type_obj, &self->fs_type)) {
            self->disk = self->fs_type = NULL;
            return -1;
        }
    } else {
        if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!iLL|O!", kwlist,
                                         &_ped_Disk_Type_obj, &self->disk,
                                         &self->type, &start, &end,
                                         &_ped_FileSystemType_Type_obj, &self->fs_type)) {
            self->disk = self->fs_type = NULL;
            return -1;
        }
    }

    /*
     * try to call libparted with provided information,
     * on failure, raise an exception
     */
    disk = _ped_Disk2PedDisk(self->disk);

    if (self->fs_type != Py_None)
        fstype = _ped_FileSystemType2PedFileSystemType(self->fs_type);

    part = ped_partition_new(disk, self->type, fstype, start, end);
    if (part == NULL) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError)) {
                PyErr_SetString(PartitionException, partedExnMessage);
            }
        } else {
            if (disk == NULL) {
                PyErr_Format(PartitionException, "Could not create new partition");
            } else {
                PyErr_Format(PartitionException, "Could not create new partition on device %s", disk->dev->path);
            }
        }

        self->disk = self->fs_type = NULL;
        return -3;
    }

    /* increment reference count for PyObjects read by PyArg_ParseTuple */
    Py_INCREF(self->disk);
    Py_INCREF(self->fs_type);

    /* copy in non-PyObject object members generated by libparted */
    self->type = part->type;

    /*
     * copy in PyObject object members generated by libparted
     * first, we drop the reference count to zero and set it to NULL
     * second, we convert the libparted type to a PyObject
     */
    Py_CLEAR(self->geom);
    self->geom = (PyObject *) PedGeometry2_ped_Geometry(&(part->geom));
    if (self->geom == NULL) {
        Py_CLEAR(self->disk);
        Py_CLEAR(self->fs_type);
        ped_partition_destroy(part);
        return -4;
    }

    self->ped_partition = part;

    /* On creation the object is not owned by any disk */
    self->_owned = 0;

    return 0;
}

PyObject *_ped_Partition_get(_ped_Partition *self, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Partition()");
        return NULL;
    }

    if (!strcmp(member, "num")) {
        return Py_BuildValue("i", self->ped_partition->num);
    } else if (!strcmp(member, "type")) {
        return PyLong_FromLong(self->type);
    } else {
        PyErr_Format(PyExc_AttributeError, "_ped.Partition object has no attribute %s", member);
        return NULL;
    }
}

int _ped_Partition_set(_ped_Partition *self, PyObject *value, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Partition()");
        return -1;
    }

    if (!strcmp(member, "type")) {
        self->type = PyLong_AsLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else {
        PyErr_Format(PyExc_AttributeError, "_ped.Partition object has no attribute %s", member);
        return -1;
    }

    return 0;
}

/* _ped.Disk functions */
void _ped_Disk_dealloc(_ped_Disk *self) {
    if (self->ped_disk) {
        ped_disk_destroy(self->ped_disk);
    }

    PyObject_GC_UnTrack(self);

    Py_CLEAR(self->dev);
    self->dev = NULL;

    Py_CLEAR(self->type);
    self->type = NULL;

    PyObject_GC_Del(self);
}

int _ped_Disk_compare(_ped_Disk *self, PyObject *obj) {
    _ped_Disk *comp = NULL;
    int check = PyObject_IsInstance(obj, (PyObject *) &_ped_Disk_Type_obj);

    if (PyErr_Occurred()) {
        return -1;
    }

    if (!check) {
        PyErr_SetString(PyExc_ValueError, "object comparing to must be a _ped.Disk");
        return -1;
    }

    comp = (_ped_Disk *) obj;
    if ((_ped_Device_Type_obj.tp_richcompare(self->dev, comp->dev, Py_EQ)) &&
        (_ped_DiskType_Type_obj.tp_richcompare(self->type, comp->type, Py_EQ))) {
        return 0;
    } else {
        return 1;
    }
}

PyObject *_ped_Disk_richcompare(_ped_Disk *a, PyObject *b, int op) {
    if (op == Py_EQ || op == Py_NE) {
        int rv = _ped_Disk_compare(a, b);
        if (PyErr_Occurred())
            return NULL;
        return PyBool_FromLong(op == Py_EQ ? rv == 0 : rv != 0);
    } else if ((op == Py_LT) || (op == Py_LE) ||
               (op == Py_GT) || (op == Py_GE)) {
        PyErr_SetString(PyExc_TypeError, "comparison operator not supported for _ped.Disk");
        return NULL;
    } else {
        PyErr_SetString(PyExc_ValueError, "unknown richcompare op");
        return NULL;
    }
}

PyObject *_ped_Disk_str(_ped_Disk *self) {
    char *ret = NULL;
    char *dev = NULL, *type = NULL;

    dev = (char *) PyUnicode_AsUTF8(_ped_Device_Type_obj.tp_repr(self->dev));
    if (dev == NULL) {
        return NULL;
    }

    type = (char *) PyUnicode_AsUTF8(_ped_Device_Type_obj.tp_repr(self->type));
    if (type == NULL) {
        return NULL;
    }

    if (asprintf(&ret, "_ped.Disk instance --\n  dev: %s  type: %s",
                 dev, type) == -1) {
        return PyErr_NoMemory();
    }

    return Py_BuildValue("s", ret);
}

int _ped_Disk_traverse(_ped_Disk *self, visitproc visit, void *arg) {
    int err;

    if (self->dev) {
        if ((err = visit(self->dev, arg))) {
            return err;
        }
    }

    if (self->type) {
        if ((err = visit(self->type, arg))) {
            return err;
        }
    }

    return 0;
}

int _ped_Disk_clear(_ped_Disk *self) {
    Py_CLEAR(self->dev);
    self->dev = NULL;

    Py_CLEAR(self->type);
    self->type = NULL;

    return 0;
}

int _ped_Disk_init(_ped_Disk *self, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"dev", NULL};
    PedDevice *device = NULL;
    PedDisk *disk = NULL;

    if (kwds == NULL) {
        if (!PyArg_ParseTuple(args, "O!", &_ped_Device_Type_obj, &self->dev)) {
            self->dev = NULL;
            return -1;
        }
    } else {
        if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist,
                                         &_ped_Device_Type_obj, &self->dev)) {
            self->dev = NULL;
            return -2;
        }
    }

    device = _ped_Device2PedDevice(self->dev);
    if (device == NULL) {
        self->dev = NULL;
        return -3;
    }
    disk = ped_disk_new(device);

    if (disk == NULL) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError)) {
                PyErr_SetString(DiskLabelException, partedExnMessage);
            }
        } else {
            PyErr_Format(IOException, "Failed to read partition table from device %s", device->path);
        }

        self->dev = NULL;
        return -4;
    }

    Py_INCREF(self->dev);

    self->type = (PyObject *) PedDiskType2_ped_DiskType((PedDiskType *) disk->type);

    self->ped_disk = disk;

    return 0;
}

/* _ped.DiskType functions */
void _ped_DiskType_dealloc(_ped_DiskType *self) {
    PyObject_GC_UnTrack(self);
    free(self->name);
    PyObject_GC_Del(self);
}

int _ped_DiskType_compare(_ped_DiskType *self, PyObject *obj) {
    _ped_DiskType *comp = NULL;
    int check = PyObject_IsInstance(obj, (PyObject *) &_ped_DiskType_Type_obj);

    if (PyErr_Occurred()) {
        return -1;
    }

    if (!check) {
        PyErr_SetString(PyExc_ValueError, "object comparing to must be a _ped.DiskType");
        return -1;
    }

    comp = (_ped_DiskType *) obj;
    if ((!strcmp(self->name, comp->name)) &&
        (self->features == comp->features)) {
        return 0;
    } else {
        return 1;
    }
}

PyObject *_ped_DiskType_richcompare(_ped_DiskType *a, PyObject *b, int op) {
    if (op == Py_EQ || op == Py_NE) {
        int rv = _ped_DiskType_compare(a, b);
        if (PyErr_Occurred())
            return NULL;
        return PyBool_FromLong(op == Py_EQ ? rv == 0 : rv != 0);
    } else if ((op == Py_LT) || (op == Py_LE) ||
               (op == Py_GT) || (op == Py_GE)) {
        PyErr_SetString(PyExc_TypeError, "comparison operator not supported for _ped.DiskType");
        return NULL;
    } else {
        PyErr_SetString(PyExc_ValueError, "unknown richcompare op");
        return NULL;
    }
}

PyObject *_ped_DiskType_str(_ped_DiskType *self) {
    char *ret = NULL;

    if (asprintf(&ret, "_ped.DiskType instance --\n"
                       "  name: %s  features: %lld",
                 self->name, self->features) == -1) {
        return PyErr_NoMemory();
    }

    return Py_BuildValue("s", ret);
}

int _ped_DiskType_traverse(_ped_DiskType *self, visitproc visit, void *arg) {
    return 0;
}

int _ped_DiskType_clear(_ped_DiskType *self) {
    return 0;
}

PyObject *_ped_DiskType_get(_ped_DiskType *self, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.DiskType()");
        return NULL;
    }

    if (!strcmp(member, "name")) {
        if (self->name != NULL)
            return PyUnicode_FromString(self->name);
        else
            return PyUnicode_FromString("");
    } else if (!strcmp(member, "features")) {
        return PyLong_FromLong(self->features);
    } else {
        PyErr_Format(PyExc_AttributeError, "_ped.DiskType object has no attribute %s", member);
        return NULL;
    }
}

/* 1:1 function mappings for disk.h in libparted */
PyObject *py_ped_disk_type_get_next(PyObject *s, PyObject *args) {
    PyObject *in_type = NULL;
    PedDiskType *cur = NULL, *next = NULL;
    _ped_DiskType *ret = NULL;

    if (!PyArg_ParseTuple(args, "|O!", &_ped_DiskType_Type_obj, &in_type)) {
        return NULL;
    }

    if (in_type) {
        cur = _ped_DiskType2PedDiskType(in_type);
        if (!cur) {
            return NULL;
        }
    }

    next = ped_disk_type_get_next(cur);
    if (next) {
        ret = PedDiskType2_ped_DiskType(next);
        return (PyObject *) ret;
    } else {
        PyErr_SetNone(PyExc_IndexError);
        return NULL;
    }
}

PyObject *py_ped_disk_type_get(PyObject *s, PyObject *args) {
    char *in_name = NULL;
    PedDiskType *out_type = NULL;
    _ped_DiskType *ret = NULL;

    if (!PyArg_ParseTuple(args, "s", &in_name)) {
        return NULL;
    }

    if (in_name) {
        out_type = ped_disk_type_get(in_name);

        if (out_type == NULL) {
            PyErr_SetString(UnknownTypeException, in_name);
            return NULL;
        }

        ret = PedDiskType2_ped_DiskType(out_type);
        if (ret == NULL) {
            return NULL;
        }

    }

    return (PyObject *) ret;
}

PyObject *py_ped_disk_type_check_feature(PyObject *s, PyObject *args) {
    PedDiskType *disktype = NULL;
    PedDiskTypeFeature feature = -1;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "i", &feature)) {
        return NULL;
    }

    disktype = _ped_DiskType2PedDiskType(s);
    if (disktype) {
        ret = ped_disk_type_check_feature(disktype, feature);
    }
    else {
        return NULL;
    }

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_disk_clobber(PyObject *s, PyObject *args) {
    PedDevice *device = NULL;
    int ret = 0;

    device = _ped_Device2PedDevice(s);
    if (device == NULL)
        return NULL;

    ret = ped_disk_clobber(device);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_Format(DiskException, "Failed to clobber partition table on device %s", device->path);

        return NULL;
    }

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

/* XXX: is this necessary? */
PyObject *py_ped_disk_duplicate(PyObject *s, PyObject *args) {
    PedDisk *disk = NULL, *pass_disk = NULL;
    _ped_Disk *ret = NULL;

    disk = _ped_Disk2PedDisk(s);
    if (disk) {
        pass_disk = ped_disk_duplicate(disk);
        if (pass_disk == NULL) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException) &&
                    !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                    PyErr_SetString(IOException, partedExnMessage);
            }
            else
                PyErr_Format(DiskException, "Could not duplicate device %s", disk->dev->path);

            return NULL;
        }

        ret = PedDisk2_ped_Disk(pass_disk);
        if (ret == NULL) {
            return NULL;
        }
    }
    else {
        return NULL;
    }

    return (PyObject *) ret;
}

/*
 * XXX:
 * We need to call ped_disk_destroy() to make sure the OS-specific
 * free() function is called on the disk.  That flushes buffers,
 * closes handles, arms, and crosschecks and whatnot.
 *
 * Similar to py_ped_device_destroy(), we might move the call to
 * ped_disk_destroy() to the tp_clear() for _ped.Disk.
 */
PyObject *py_ped_disk_destroy(PyObject *s, PyObject *args) {
    PedDisk *disk = NULL;

    disk = _ped_Disk2PedDisk(s);
    if (disk == NULL) {
        return NULL;
    }

    ped_disk_destroy(disk);
    Py_CLEAR(s);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_commit(PyObject *s, PyObject *args) {
    PedDisk *disk = NULL;
    int ret = 0;

    disk = _ped_Disk2PedDisk(s);
    if (disk) {
        ret = ped_disk_commit(disk);
        if (ret == 0) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException) &&
                    !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                    PyErr_SetString(IOException, partedExnMessage);
            }
            else
                PyErr_Format(DiskException, "Could not commit to disk %s, (%s)",
                             disk->dev->path, __func__);

            return NULL;
        }
    }
    else {
        return NULL;
    }

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_disk_commit_to_dev(PyObject *s, PyObject *args) {
    PedDisk *disk = NULL;
    int ret = 0;

    disk = _ped_Disk2PedDisk(s);
    if (disk) {
        ret = ped_disk_commit_to_dev(disk);
        if (ret == 0) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException) &&
                    !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                    PyErr_SetString(IOException, partedExnMessage);
            }
            else
                PyErr_Format(DiskException, "Could not commit to disk %s, (%s)",
                             disk->dev->path, __func__);

            return NULL;
        }
    }
    else {
        return NULL;
    }

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_disk_commit_to_os(PyObject *s, PyObject *args) {
    PedDisk *disk = NULL;
    int ret = 0;

    disk = _ped_Disk2PedDisk(s);
    if (disk) {
        ret = ped_disk_commit_to_os(disk);
        if (ret == 0) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException) &&
                    !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                    PyErr_SetString(IOException, partedExnMessage);
            }
            else
                PyErr_Format(DiskException, "Could not commit to disk %s, (%s)",
                             disk->dev->path, __func__);

            return NULL;
        }
    }
    else {
        return NULL;
    }

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_disk_check(PyObject *s, PyObject *args) {
    PedDisk *disk = NULL;
    int ret = 0;

    disk = _ped_Disk2PedDisk(s);
    if (disk) {
        ret = ped_disk_check(disk);
        if (ret == 0) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException) &&
                    !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                    PyErr_SetString(IOException, partedExnMessage);
            }
            else
                PyErr_Format(DiskException, "Could not check disk %s", disk->dev->path);

            return NULL;
        }
    }
    else {
        return NULL;
    }

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_disk_print(PyObject *s, PyObject *args) {
    PedDisk *disk = NULL;

    disk = _ped_Disk2PedDisk(s);
    if (disk) {
        ped_disk_print(disk);
    }
    else {
        return NULL;
    }

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_get_primary_partition_count(PyObject *s, PyObject *args) {
    PedDisk *disk = NULL;
    int ret = 0;

    disk = _ped_Disk2PedDisk(s);
    if (disk) {
        ret = ped_disk_get_primary_partition_count(disk);
    }
    else {
        return NULL;
    }

    return PyLong_FromLong(ret);
}

PyObject *py_ped_disk_get_last_partition_num(PyObject *s, PyObject *args) {
    PedDisk *disk = NULL;
    int ret = 0;

    disk = _ped_Disk2PedDisk(s);
    if (disk) {
        ret = ped_disk_get_last_partition_num(disk);
    }
    else {
        return NULL;
    }

    return PyLong_FromLong(ret);
}

PyObject *py_ped_disk_get_max_primary_partition_count(PyObject *s,
                                                      PyObject *args) {
    PedDisk *disk = NULL;
    int ret = 0;

    disk = _ped_Disk2PedDisk(s);
    if (disk) {
        ret = ped_disk_get_max_primary_partition_count(disk);
    }
    else {
        return NULL;
    }

    return PyLong_FromLong(ret);
}

PyObject *py_ped_disk_get_max_supported_partition_count(PyObject *s,
                                                        PyObject *args) {
    PedDisk *disk = NULL;
    int max = 0;

    disk = _ped_Disk2PedDisk(s);
    if (disk) {
        if (ped_disk_get_max_supported_partition_count(disk, &max) == true) {
            return Py_BuildValue("i", max);
        }
    }

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_get_partition_alignment(PyObject *s, PyObject *args) {
    PedDisk *disk = NULL;
    PedAlignment *alignment = NULL;
    _ped_Alignment *ret = NULL;

    disk = _ped_Disk2PedDisk(s);
    if (!disk)
        return NULL;

    alignment = ped_disk_get_partition_alignment(disk);
    if (!alignment) {
        PyErr_SetString(CreateException, "Could not get alignment for device");
        return NULL;
    }

    ret = PedAlignment2_ped_Alignment(alignment);
    ped_alignment_destroy(alignment);

    return (PyObject *) ret;
}

PyObject *py_ped_disk_max_partition_length(PyObject *s, PyObject *args) {
    PedDisk *disk = NULL;

    disk = _ped_Disk2PedDisk(s);
    if (!disk)
        return NULL;

    return PyLong_FromUnsignedLongLong(ped_disk_max_partition_length(disk));
}

PyObject *py_ped_disk_max_partition_start_sector(PyObject *s, PyObject *args) {
    PedDisk *disk = NULL;

    disk = _ped_Disk2PedDisk(s);
    if (!disk)
        return NULL;

    return PyLong_FromUnsignedLongLong(ped_disk_max_partition_start_sector(disk));
}

PyObject *py_ped_disk_set_flag(PyObject *s, PyObject *args) {
    int ret, flag, state;
    PedDisk *disk = NULL;

    if (!PyArg_ParseTuple(args, "ii", &flag, &state)) {
        return NULL;
    }

    disk = _ped_Disk2PedDisk(s);
    if (disk == NULL) {
        return NULL;
    }

    ret = ped_disk_set_flag(disk, flag, state);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                PyErr_SetString(DiskException, partedExnMessage);
        }
        else
            PyErr_Format(DiskException, "Could not set flag on disk %s", disk->dev->path);

        return NULL;
    }

    Py_RETURN_TRUE;
}

PyObject *py_ped_disk_get_flag(PyObject *s, PyObject *args) {
    int flag;
    PedDisk *disk = NULL;

    if (!PyArg_ParseTuple(args, "i", &flag)) {
        return NULL;
    }

    disk = _ped_Disk2PedDisk(s);
    if (disk == NULL) {
        return NULL;
    }

    if (ped_disk_get_flag(disk, flag)) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_disk_is_flag_available(PyObject *s, PyObject *args) {
    int flag;
    PedDisk *disk = NULL;

    if (!PyArg_ParseTuple(args, "i", &flag)) {
        return NULL;
    }

    disk = _ped_Disk2PedDisk(s);
    if (disk == NULL) {
        return NULL;
    }

    if (ped_disk_is_flag_available(disk, flag)) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_disk_flag_get_name(PyObject *s, PyObject *args) {
    int flag;
    char *ret = NULL;

    if (!PyArg_ParseTuple(args, "i", &flag)) {
        return NULL;
    }

    if (flag < PED_DISK_FIRST_FLAG) {
        PyErr_SetString(PyExc_ValueError, "Invalid flag provided.");
        return NULL;
    }

    ret = (char *) ped_disk_flag_get_name(flag);
    if (ret == NULL) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                PyErr_SetString(DiskException, partedExnMessage);
        }
        else
            PyErr_Format(DiskException, "Could not get disk flag name for %d", flag);

        return NULL;
    }

    return PyUnicode_FromString(ret);
}

PyObject *py_ped_disk_flag_get_by_name(PyObject *s, PyObject *args) {
    char *name = NULL;

    if (!PyArg_ParseTuple(args, "s", &name)) {
        return NULL;
    }

    return PyLong_FromLong(ped_disk_flag_get_by_name(name));
}

PyObject *py_ped_disk_flag_next(PyObject *s, PyObject *args) {
    int flag;

    if (!PyArg_ParseTuple(args, "i", &flag)) {
        return NULL;
    }

    return Py_BuildValue("i", ped_disk_flag_next(flag));
}

/*
 * XXX:
 * We need to call ped_disk_destroy() to make sure the OS-specific
 * free() function is called on the disk.  That flushes buffers,
 * closes handles, arms, and crosschecks and whatnot.
 *
 * Similar to py_ped_device_destroy(), we might move the call to
 * ped_disk_destroy() to the tp_clear() for _ped.Disk.
 */
PyObject *py_ped_partition_destroy(_ped_Partition *s, PyObject *args) {
    PedPartition *partition = NULL;

    partition = _ped_Partition2PedPartition(s);
    if (partition == NULL) {
        return NULL;
    }

    ped_partition_destroy(partition);
    Py_CLEAR(s);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_partition_is_active(_ped_Partition *s, PyObject *args) {
    PedPartition *partition = NULL;
    int ret = 0;

    partition = _ped_Partition2PedPartition(s);
    if (partition) {
        ret = ped_partition_is_active(partition);
    }
    else {
        return NULL;
    }

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_partition_set_flag(_ped_Partition *s, PyObject *args) {
    int in_state = -1;
    PedPartition *part = NULL;
    int flag;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "ii", &flag, &in_state)) {
        return NULL;
    }

    part = _ped_Partition2PedPartition(s);
    if (part == NULL) {
        return NULL;
    }

    if (part && flag && in_state > -1) {
        ret = ped_partition_set_flag(part, flag, in_state);
        if (ret == 0) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException) &&
                    !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                    PyErr_SetString(PartitionException, partedExnMessage);
            }
            else
                PyErr_Format(PartitionException, "Could not set flag on partition %s%d", part->disk->dev->path, part->num);

            return NULL;
        }
    }

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_partition_get_flag(_ped_Partition *s, PyObject *args) {
    PedPartition *part = NULL;
    int flag;
    int ret = -1;

    if (!PyArg_ParseTuple(args, "i", &flag)) {
        return NULL;
    }

    part = _ped_Partition2PedPartition(s);
    if (part == NULL) {
        return NULL;
    }

    /* ped_partition_get_flag will assert on this. */
    if (!ped_partition_is_active(part)) {
        PyErr_Format(PartitionException, "Could not get flag on inactive partition %s%d", part->disk->dev->path, part->num);
        return NULL;
    }

    ret = ped_partition_get_flag(part, flag);

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_partition_is_flag_available(_ped_Partition *s, PyObject *args) {
    PedPartition *part = NULL;
    int flag;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "i", &flag)) {
        return NULL;
    }

    part = _ped_Partition2PedPartition(s);
    if (part == NULL) {
        return NULL;
    }

    if (!ped_partition_is_active(part)) {
        PyErr_Format(PartitionException, "Flag is not available on inactive partition %s%d", part->disk->dev->path, part->num);
        return NULL;
    }

    ret = ped_partition_is_flag_available(part, flag);

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_partition_set_system(_ped_Partition *s, PyObject *args) {
    PyObject *in_fstype = NULL;
    PedPartition *part = NULL;
    PedFileSystemType *out_fstype = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!", &_ped_FileSystemType_Type_obj, &in_fstype)) {
        return NULL;
    }

    part = _ped_Partition2PedPartition(s);
    if (part == NULL) {
        return NULL;
    }

    if (in_fstype != NULL) {
        out_fstype = _ped_FileSystemType2PedFileSystemType(in_fstype);
        if (out_fstype == NULL) {
            return NULL;
        }
    }

    /* ped_partition_set_system will assert on this. */
    if (!ped_partition_is_active(part)) {
        PyErr_Format(PartitionException, "Could not set system flag on inactive partition %s%d", part->disk->dev->path, part->num);
        return NULL;
    }

    ret = ped_partition_set_system(part, out_fstype);
    if (ret == 0) {
        PyErr_Format(PartitionException, "Could not set system flag on partition %s%d", part->disk->dev->path, part->num);
        return NULL;
    }

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_partition_set_name(_ped_Partition *s, PyObject *args) {
    PedPartition *part = NULL;
    char *in_name = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "s", &in_name)) {
        return NULL;
    }

    part = _ped_Partition2PedPartition(s);
    if (part == NULL) {
        return NULL;
    }

    /* ped_partition_set_name will assert on this. */
    if (!ped_partition_is_active(part)) {
        PyErr_Format(PartitionException, "Could not set system flag on inactive partition %s%d", part->disk->dev->path, part->num);
        return NULL;
    }

    ret = ped_partition_set_name(part, in_name);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError)) {
                PyErr_SetString(PartitionException, partedExnMessage);
            }
        } else {
            PyErr_Format(PartitionException, "Could not set name on partition %s%d", part->disk->dev->path, part->num);
        }

        return NULL;
    }

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_partition_get_name(_ped_Partition *s, PyObject *args) {
    PedPartition *part = NULL;
    char *ret = NULL;

    part = _ped_Partition2PedPartition(s);
    if (part == NULL) {
        return NULL;
    }

    /* ped_partition_get_name will assert on this. */
    if (!ped_partition_is_active(part)) {
        PyErr_Format(PartitionException, "Could not get name on inactive partition %s%d", part->disk->dev->path, part->num);
        return NULL;
    }

    ret = (char *) ped_partition_get_name(part);
    if (ret == NULL) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError)) {
                PyErr_SetString(PartitionException, partedExnMessage);
            }
        } else {
            PyErr_Format(PartitionException, "Could not read name on partition %s%d", part->disk->dev->path, part->num);
        }

        return NULL;
    }

    return PyUnicode_FromString(ret);
}

PyObject *py_ped_partition_is_busy(_ped_Partition *s, PyObject *args) {
    PedPartition *part = NULL;
    int ret = 0;

    part = _ped_Partition2PedPartition(s);
    if (part) {
        ret = ped_partition_is_busy(part);
    }
    else {
        return NULL;
    }

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_partition_get_path(_ped_Partition *s, PyObject *args) {
    PedPartition *part = NULL;
    char *ret = NULL;

    part = _ped_Partition2PedPartition(s);
    if (part) {
        ret = ped_partition_get_path(part);
        if (ret == NULL) {
            PyErr_Format(PartitionException, "Could not get path for partition %s%d", part->disk->dev->path, part->num);
            return NULL;
        }
    }
    else {
        return NULL;
    }

    return PyUnicode_FromString(ret);
}

PyObject *py_ped_partition_reset_num(_ped_Partition *s, PyObject *args) {
    PedPartition *part = NULL;

    part = _ped_Partition2PedPartition(s);
    if (part == NULL) {
        Py_RETURN_FALSE;
    }

    part->num = -1;

    Py_RETURN_TRUE;
}


PyObject *py_ped_partition_type_get_name(PyObject *s, PyObject *args) {
    long type;
    char *ret = NULL;

    if (!PyArg_ParseTuple(args, "i", &type)) {
        return NULL;
    }

    if (type) {
        ret = (char *) ped_partition_type_get_name(type);
    }

    if (ret != NULL)
        return PyUnicode_FromString(ret);
    else
        return PyUnicode_FromString("");
}

PyObject *py_ped_partition_flag_get_name(PyObject *s, PyObject *args) {
    int flag;
    char *ret = NULL;

    if (!PyArg_ParseTuple(args, "i", &flag)) {
        return NULL;
    }

    if (flag < PED_PARTITION_FIRST_FLAG) {
        PyErr_SetString(PyExc_ValueError, "Invalid flag provided.");
        return NULL;
    }

    if (flag) {
        ret = (char *) ped_partition_flag_get_name(flag);

        if (!ret) {
            /* Re-raise the libparted exception. */
            partedExnRaised = 0;
            return NULL;
        }
    }

    return PyUnicode_FromString(ret);
}

PyObject *py_ped_partition_flag_get_by_name(PyObject *s, PyObject *args) {
    char *name = NULL;

    if (!PyArg_ParseTuple(args, "s", &name)) {
        return NULL;
    }

    return PyLong_FromLong(ped_partition_flag_get_by_name(name));
}

PyObject *py_ped_partition_flag_next(PyObject *s, PyObject *args) {
    int flag;

    if (!PyArg_ParseTuple(args, "i", &flag)) {
        return NULL;
    }

    return Py_BuildValue("i", ped_partition_flag_next(flag));
}

PyObject *py_ped_disk_add_partition(PyObject *s, PyObject *args) {
    _ped_Partition *in_part = NULL;
    PyObject *in_constraint = NULL;
    PedDisk *disk = NULL;
    PedPartition *out_part = NULL;
    PedConstraint *out_constraint = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!|O!",&_ped_Partition_Type_obj,
                          &in_part, &_ped_Constraint_Type_obj, &in_constraint)) {
        return NULL;
    }

    disk = _ped_Disk2PedDisk(s);
    if (disk == NULL) {
        return NULL;
    }

    /* Its non-sense to add an owned partition */
    if (in_part->_owned == 1) {
        PyErr_SetString(PartitionException, "Attempting to add a partition "
                                            "that is already owned by a disk.");
        return NULL;
    }


    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part == NULL) {
        return NULL;
    }

    if (out_part->disk != disk) {
        PyErr_SetString(PartitionException, "Cannot add a partition to another disk then the one used for creating the partition");
        return NULL;
    }

    if (in_constraint) {
        out_constraint = _ped_Constraint2PedConstraint(in_constraint);
        if (out_constraint == NULL) {
            return NULL;
        }
    }

    ret = ped_disk_add_partition(disk, out_part, out_constraint);

    if (out_constraint)
        ped_constraint_destroy(out_constraint);

    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                PyErr_SetString(PartitionException, partedExnMessage);
        }
        else
            PyErr_Format(PartitionException, "Could not create partition %s%d", out_part->disk->dev->path, out_part->num);

        return NULL;
    }

    /* update our _ped.Partition object with out_part values */
    in_part->type = out_part->type;
    in_part->_owned = 1;
    *((_ped_Geometry *)in_part->geom)->ped_geometry = out_part->geom;

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_disk_remove_partition(PyObject *s, PyObject *args) {
    _ped_Partition *in_part = NULL;
    PedDisk *disk = NULL;
    PedPartition *out_part = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Partition_Type_obj, &in_part)) {
        return NULL;
    }

    disk = _ped_Disk2PedDisk(s);
    if (disk == NULL) {
        return NULL;
    }

    /* Its non-sense  to remove an unowned partition */
    if (in_part->_owned == 0) {
        PyErr_SetString(PartitionException, "Attempting to remove a partition "
                                            "that is not owned by any disk.");
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part == NULL) {
        return NULL;
    }

    if (out_part->disk != disk) {
        PyErr_SetString(PartitionException, "Partition is not part of the disk it is being removed from");
        return NULL;
    }

    if (out_part->part_list != NULL) {
        PedPartition *part;
        for (part = out_part->part_list; part; part = part->next) {
            if (ped_partition_is_active(part))
                break;
        }
        if (part) {
            PyErr_SetString(PartitionException, "Attempting to remove an extended partition that still contains logical partitions");
            return NULL;
        }
    }

    ret = ped_disk_remove_partition(disk, out_part);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                PyErr_SetString(PartitionException, partedExnMessage);
        }
        else
            PyErr_Format(PartitionException, "Could not remove partition %s%d", out_part->disk->dev->path, out_part->num);

        return NULL;
    }

    in_part->_owned = 0;

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_disk_delete_partition(PyObject *s, PyObject *args) {
    return py_ped_disk_remove_partition(s, args);
}

PyObject *py_ped_disk_delete_all(PyObject *s, PyObject *args) {
    PedDisk *disk = NULL;
    int ret = 0;

    disk = _ped_Disk2PedDisk(s);
    if (disk) {
        ret = ped_disk_delete_all(disk);
        if (ret == 0) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException) &&
                    !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                    PyErr_SetString(PartitionException, partedExnMessage);
            }
            else
                PyErr_Format(PartitionException, "Could not remove all partitions on %s", disk->dev->path);

            return NULL;
        }
    }
    else {
        return NULL;
    }

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_disk_set_partition_geom(PyObject *s, PyObject *args) {
    _ped_Partition *in_part = NULL;
    PyObject *in_constraint = NULL;
    PedDisk *disk = NULL;
    PedPartition *out_part = NULL;
    PedConstraint *out_constraint = NULL;
    PedSector start, end;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!OLL", &_ped_Partition_Type_obj, &in_part,
                          &in_constraint, &start, &end)) {
        return NULL;
    }

    if (in_constraint != Py_None && !PyObject_IsInstance(in_constraint,
                                      (PyObject *)&_ped_Constraint_Type_obj)) {
        PyErr_SetString(PyExc_ValueError, "invalid constraint type");
        return NULL;
    }

    disk = _ped_Disk2PedDisk(s);
    if (disk == NULL) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part == NULL) {
        return NULL;
    }

    if (out_part->disk != disk) {
        PyErr_SetString(PartitionException, "partition.disk does not match disk");
        return NULL;
    }

    if (in_constraint != Py_None) {
        out_constraint = _ped_Constraint2PedConstraint(in_constraint);
        if (out_constraint == NULL) {
            return NULL;
        }
    }

    ret = ped_disk_set_partition_geom(disk, out_part, out_constraint,
                                      start, end);

    if (out_constraint)
        ped_constraint_destroy(out_constraint);

    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                PyErr_SetString(PartitionException, partedExnMessage);
        }
        else
            PyErr_Format(PartitionException, "Could not set geometry on %s%d", disk->dev->path, out_part->num);

        return NULL;
    }

    *((_ped_Geometry *)in_part->geom)->ped_geometry = out_part->geom;

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_disk_maximize_partition(PyObject *s, PyObject *args) {
    _ped_Partition *in_part = NULL;
    PyObject *in_constraint = NULL;
    PedDisk *disk = NULL;
    PedPartition *out_part = NULL;
    PedConstraint *out_constraint = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!|O!", &_ped_Partition_Type_obj, &in_part,
                          &_ped_Constraint_Type_obj, &in_constraint)) {
        return NULL;
    }

    disk = _ped_Disk2PedDisk(s);
    if (disk == NULL) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part == NULL) {
        return NULL;
    }

    if (out_part->disk != disk) {
        PyErr_SetString(PartitionException, "partition.disk does not match disk");
        return NULL;
    }

    if (in_constraint) {
        out_constraint = _ped_Constraint2PedConstraint(in_constraint);
        if (out_constraint == NULL) {
            return NULL;
        }
    }

    ret = ped_disk_maximize_partition(disk, out_part, out_constraint);

    if (out_constraint)
        ped_constraint_destroy(out_constraint);

    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                PyErr_SetString(PartitionException, partedExnMessage);
        }
        else
            PyErr_Format(PartitionException, "Could not maximize partition size for %s%d", disk->dev->path, out_part->num);

        return NULL;
    }

    *((_ped_Geometry *)in_part->geom)->ped_geometry = out_part->geom;

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_disk_get_max_partition_geometry(PyObject *s, PyObject *args) {
    _ped_Partition *in_part = NULL;
    PyObject *in_constraint = NULL;
    PedDisk *disk = NULL;
    PedPartition *out_part = NULL;
    PedConstraint *out_constraint = NULL;
    PedGeometry *pass_geom = NULL;
    _ped_Geometry *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!|O!", &_ped_Partition_Type_obj, &in_part,
                          &_ped_Constraint_Type_obj, &in_constraint)) {
        return NULL;
    }

    disk = _ped_Disk2PedDisk(s);
    if (disk == NULL) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part == NULL) {
        return NULL;
    }

    if (out_part->disk != disk) {
        PyErr_SetString(PartitionException, "partition.disk does not match disk");
        return NULL;
    }

    if (in_constraint) {
        out_constraint = _ped_Constraint2PedConstraint(in_constraint);
        if (out_constraint == NULL) {
            return NULL;
        }
    }

    pass_geom = ped_disk_get_max_partition_geometry(disk, out_part,
                                                    out_constraint);
    if (out_constraint)
        ped_constraint_destroy(out_constraint);

    if (pass_geom == NULL) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                PyErr_SetString(PartitionException, partedExnMessage);
        }
        else
            PyErr_Format(PartitionException, "Could not get maximum partition size for %s%d", disk->dev->path, out_part->num);

        return NULL;
    }

    ret = PedGeometry2_ped_Geometry(pass_geom);
    if (ret == NULL) {
        return NULL;
    }

    return (PyObject *) ret;
}

PyObject *py_ped_disk_minimize_extended_partition(PyObject *s, PyObject *args) {
    PedDisk *disk = NULL;
    int ret = 0;

    disk = _ped_Disk2PedDisk(s);
    if (disk) {
        ret = ped_disk_minimize_extended_partition(disk);
        if (ret == 0) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException) &&
                    !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                    PyErr_SetString(PartitionException, partedExnMessage);
            }
            else
                PyErr_Format(PartitionException, "Could not shrink extended partition on %s", disk->dev->path);

            return NULL;
        }
    }
    else {
        return NULL;
    }

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_disk_next_partition(PyObject *s, PyObject *args) {
    _ped_Partition *in_part = NULL;
    PedDisk *disk = NULL;
    PedPartition *out_part = NULL;
    PedPartition *pass_part = NULL;
    _ped_Partition *ret = NULL;

    if (!PyArg_ParseTuple(args, "|O!", &_ped_Partition_Type_obj, &in_part)) {
        return NULL;
    }

    disk = _ped_Disk2PedDisk(s);
    if (disk == NULL) {
        return NULL;
    }

    if (in_part) {
        out_part = _ped_Partition2PedPartition(in_part);
        if (out_part == NULL) {
            return NULL;
        }

        if (out_part->disk != disk) {
            PyErr_SetString(PartitionException, "partition.disk does not match disk");
            return NULL;
        }
    }

    pass_part = ped_disk_next_partition(disk, out_part);
    if (pass_part == NULL) {
        Py_INCREF(Py_None);
        return Py_None;
    }

    ret = PedPartition2_ped_Partition(pass_part, (_ped_Disk *)s);
    if (ret == NULL) {
        return NULL;
    }

    ret->_owned = 1;
    return (PyObject *) ret;
}

PyObject *py_ped_disk_get_partition(PyObject *s, PyObject *args) {
    int num;
    PedDisk *disk = NULL;
    PedPartition *pass_part = NULL;
    _ped_Partition *ret = NULL;

    if (!PyArg_ParseTuple(args, "i", &num)) {
        return NULL;
    }

    disk = _ped_Disk2PedDisk(s);
    if (disk) {
        pass_part = ped_disk_get_partition(disk, num);
        if (pass_part == NULL) {
            PyErr_SetString(PartitionException, "Partition does not exist");
            return NULL;
        }

        ret = PedPartition2_ped_Partition(pass_part, (_ped_Disk *)s);
        if (ret == NULL) {
            return NULL;
        }
    }
    else {
        return NULL;
    }

    ret->_owned = 1;
    return (PyObject *) ret;
}

PyObject *py_ped_disk_get_partition_by_sector(PyObject *s, PyObject *args) {
    PedDisk *disk = NULL;
    PedSector sector;
    PedPartition *pass_part = NULL;
    _ped_Partition *ret = NULL;

    if (!PyArg_ParseTuple(args, "L", &sector)) {
        return NULL;
    }

    disk = _ped_Disk2PedDisk(s);
    if (disk == NULL) {
        return NULL;
    }

    pass_part = ped_disk_get_partition_by_sector(disk, sector);
    if (pass_part == NULL) {
        PyErr_SetString(PartitionException, "Partition does not exist");
        return NULL;
    }

    ret = PedPartition2_ped_Partition(pass_part, (_ped_Disk *)s);
    if (ret == NULL) {
        return NULL;
    }

    ret->_owned = 1;
    return (PyObject *) ret;
}

PyObject *py_ped_disk_extended_partition(PyObject *s, PyObject *args) {
    PedDisk *disk = NULL;
    PedPartition *pass_part = NULL;
    _ped_Partition *ret = NULL;

    disk = _ped_Disk2PedDisk(s);
    if (disk) {
        pass_part = ped_disk_extended_partition(disk);
        if (pass_part == NULL) {
            PyErr_SetString(PartitionException, "Extended partition does not exist");
            return NULL;
        }

        ret = PedPartition2_ped_Partition(pass_part, (_ped_Disk *)s);
        if (ret == NULL) {
            return NULL;
        }
    }
    else {
        return NULL;
    }

    ret->_owned = 1;
    return (PyObject *) ret;
}

PyObject *py_ped_disk_new_fresh(PyObject *s, PyObject *args) {
    _ped_Device *in_device = NULL;
    _ped_DiskType *in_type = NULL;
    PedDevice *device = NULL;
    PedDiskType *type = NULL;
    PedDisk *disk = NULL;
    _ped_Disk *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_Device_Type_obj, &in_device,
                                        &_ped_DiskType_Type_obj, &in_type)) {
        return NULL;
    }

    if ((device = _ped_Device2PedDevice((PyObject *) in_device)) == NULL) {
        return NULL;
    }

    if ((type = _ped_DiskType2PedDiskType((PyObject *) in_type)) == NULL) {
        return NULL;
    }

    if ((disk = ped_disk_new_fresh(device, type)) == NULL) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                PyErr_SetString(DiskException, partedExnMessage);
        } else {
            if (disk->dev == NULL) {
                PyErr_Format(DiskException, "Could not create new disk label");
            } else {
                PyErr_Format(DiskException, "Could not create new disk label on %s", disk->dev->path);
            }
        }

        return NULL;
    }

    ret = PedDisk2_ped_Disk(disk);
    return (PyObject *) ret;
}

PyObject *py_ped_disk_new(PyObject *s, PyObject *args) {
    _ped_Device *in_device = NULL;
    PedDevice *device = NULL;
    PedDisk *disk = NULL;
    _ped_Disk *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Device_Type_obj, &in_device)) {
        return NULL;
    }

    if ((device = _ped_Device2PedDevice((PyObject *) in_device)) == NULL) {
        return NULL;
    }

    if ((disk = ped_disk_new(device)) == NULL) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                PyErr_SetString(DiskException, partedExnMessage);
        } else {
            if (disk->dev == NULL) {
                PyErr_Format(DiskException, "Could not create new disk label");
            } else {
                PyErr_Format(DiskException, "Could not create new disk label on %s", disk->dev->path);
            }
        }

        return NULL;
    }

    ret = PedDisk2_ped_Disk(disk);
    return (PyObject *) ret;
}

/* vim:tw=78:ts=4:et:sw=4
 */
