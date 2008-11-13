/*
 * pydisk.c
 *
 * Copyright (C) 2007  Red Hat, Inc.
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

#include <stdlib.h>

#include "convert.h"
#include "exceptions.h"
#include "pydisk.h"

/* _ped.Partition functions */
void _ped_Partition_dealloc(_ped_Partition *self) {
    PyObject_Del(self);
}

PyObject *_ped_Partition_new(PyTypeObject *type, PyObject *args,
                             PyObject *kwds) {
    _ped_Partition *self = NULL;

    self = PyObject_New(_ped_Partition, &_ped_Partition_Type_obj);
    return (PyObject *) self;
}

int _ped_Partition_init(_ped_Partition *self, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"disk", "geom", "num", "type", "fs_type", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O!O!ilO!", kwlist,
                                     &_ped_Disk_Type_obj, &self->disk,
                                     &_ped_Geometry_Type_obj, &self->geom,
                                     &self->num, &self->type,
                                     &_ped_FileSystemType_Type_obj,
                                     &self->fs_type))
        return -1;
    else
        return 0;
}

PyObject *_ped_Partition_get(_ped_Partition *self, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Partition()");
        return NULL;
    }

    if (!strcmp(member, "num")) {
        return Py_BuildValue("i", self->num);
    } else if (!strcmp(member, "type")) {
        return PyLong_FromLongLong(self->type);
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

    if (!strcmp(member, "num")) {
        self->num = PyInt_AsLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else if (!strcmp(member, "type")) {
        self->type = PyInt_AsLong(value);
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
    PyObject_Del(self);
}

PyObject *_ped_Disk_new(PyTypeObject *type, PyObject *args,
                             PyObject *kwds) {
    _ped_Disk *self = NULL;

    self = PyObject_New(_ped_Disk, &_ped_Disk_Type_obj);
    return (PyObject *) self;
}

int _ped_Disk_init(_ped_Disk *self, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"dev", "type", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O!O!", kwlist,
                                     &_ped_Device_Type_obj, &self->dev,
                                     &_ped_DiskType_Type_obj, &self->type))
        return -1;
    else
        return 0;
}

/* _ped.DiskType functions */
void _ped_DiskType_dealloc(_ped_DiskType *self) {
    PyObject_Del(self);
}

PyObject *_ped_DiskType_new(PyTypeObject *type, PyObject *args,
                            PyObject *kwds) {
    _ped_DiskType *self = NULL;

    self = (_ped_DiskType *) PyObject_New(PyObject, &_ped_DiskType_Type_obj);
    return (PyObject *) self;
}

int _ped_DiskType_init(_ped_DiskType *self, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"name", "features", NULL};

    self->name = NULL;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|zl", kwlist,
                                     &self->name, &self->features))
        return -1;
    else
        return 0;
}

PyObject *_ped_DiskType_get(_ped_DiskType *self, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.DiskType()");
        return NULL;
    }

    if (!strcmp(member, "name")) {
        return PyString_FromString(self->name);
    } else if (!strcmp(member, "features")) {
        return PyLong_FromLongLong(self->features);
    } else {
        PyErr_Format(PyExc_AttributeError, "_ped.DiskType object has no attribute %s", member);
        return NULL;
    }
}

int _ped_DiskType_set(_ped_DiskType *self, PyObject *value, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.DiskType()");
        return -1;
    }

    if (!strcmp(member, "name")) {
        self->name = PyString_AsString(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else if (!strcmp(member, "features")) {
        self->features = PyLong_AsLongLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else {
        PyErr_Format(PyExc_AttributeError, "_ped.DiskType object has no attribute %s", member);
        return -1;
    }

    return 0;
}

/* 1:1 function mappings for disk.h in libparted */
PyObject *py_ped_disk_type_register(PyObject *s, PyObject *args) {
    PyObject *in_disktype = NULL;
    PedDiskType *out_disktype = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_DiskType_Type_obj, &in_disktype)) {
        return NULL;
    }

    out_disktype = _ped_DiskType2PedDiskType(in_disktype);
    if (out_disktype == NULL) {
        return NULL;
    }

    ped_disk_type_register(out_disktype);
    /* do not free out_disktype here because it's now in libparted's list */

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_type_unregister(PyObject *s, PyObject *args) {
    PyObject *in_disktype = NULL;
    PedDiskType *out_disktype = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_DiskType_Type_obj, &in_disktype)) {
        return NULL;
    }

    out_disktype = _ped_DiskType2PedDiskType(in_disktype);
    if (out_disktype == NULL) {
        return NULL;
    }

    ped_disk_type_unregister(out_disktype);
    free(out_disktype);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_type_get_next(PyObject *s, PyObject *args) {
    PyObject *in_type = NULL;
    PedDiskType *out_type = NULL, *ret_type = NULL;
    _ped_DiskType *ret = NULL;

    if (!PyArg_ParseTuple(args, "|O!", &_ped_DiskType_Type_obj, &in_type)) {
        return NULL;
    }

    if (in_type == Py_None)
       out_type = NULL;
    else
       out_type = _ped_DiskType2PedDiskType(in_type);

    ret_type = ped_disk_type_get_next(out_type);
    if (ret_type) {
        ret = PedDiskType2_ped_DiskType(ret_type);
        return (PyObject *) ret;
    }
    else {
        PyErr_SetNone(PyExc_IndexError);
        return NULL;
    }
}

PyObject *py_ped_disk_type_get(PyObject *s, PyObject *args) {
    char *in_name = NULL;
    PedDiskType *out_type = NULL;
    _ped_DiskType *ret = NULL;

    if (!PyArg_ParseTuple(args, "z", &in_name)) {
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
    PyObject *in_disktype = NULL;
    PedDiskType *out_disktype = NULL;
    PedDiskTypeFeature feature = -1;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!l", &_ped_DiskType_Type_obj, &in_disktype,
                          &feature)) {
        return NULL;
    }

    out_disktype = _ped_DiskType2PedDiskType(in_disktype);
    if (out_disktype && feature) {
        ret = ped_disk_type_check_feature(out_disktype, feature);
        free(out_disktype);
    }
    else {
        return NULL;
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_probe(PyObject *s, PyObject *args) {
    PyObject *in_device = NULL;
    PedDevice *out_device = NULL;
    PedDiskType *out_type = NULL;
    _ped_DiskType *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Device_Type_obj, &in_device)) {
        return NULL;
    }

    out_device = _ped_Device2PedDevice(in_device);
    if (in_device) {
        out_type = ped_disk_probe(out_device);
        if (out_type == NULL) {
            PyErr_Format(IOException, "Could not probe device %s", out_device->path);
            return NULL;
        }

        ret = PedDiskType2_ped_DiskType(out_type);
        if (ret == NULL) {
            return NULL;
        }

        free(out_type);
    }

    return (PyObject *) ret;
}

PyObject *py_ped_disk_clobber(PyObject *s, PyObject *args) {
    PyObject *in_device = NULL;
    PedDevice *out_device = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Device_Type_obj, &in_device)) {
        return NULL;
    }

    out_device = _ped_Device2PedDevice(in_device);
    if (out_device) {
        ret = ped_disk_clobber(out_device);
        if (ret == 0) {
            PyErr_Format(DiskException, "Failed to clobber partition table on device %s", out_device->path);
            return NULL;
        }

        ped_device_destroy(out_device);
    }
    else {
        return NULL;
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_clobber_exclude(PyObject *s, PyObject *args) {
    PyObject *in_device = NULL, *in_disktype = NULL;
    PedDevice *out_device = NULL;
    PedDiskType *out_disktype = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_Device_Type_obj, &in_device,
                          &_ped_DiskType_Type_obj, &in_disktype)) {
        return NULL;
    }

    out_device = _ped_Device2PedDevice(in_device);
    if (out_device == NULL) {
        return NULL;
    }

    out_disktype = _ped_DiskType2PedDiskType(in_disktype);
    if (out_disktype == NULL) {
        return NULL;
    }

    ret = ped_disk_clobber_exclude(out_device, out_disktype);
    if (ret == 0) {
        PyErr_Format(DiskException, "Failed to clobber partition table on device %s", out_device->path);
        return NULL;
    }

    ped_device_destroy(out_device);
    free(out_disktype);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_new(PyObject *s, PyObject *args) {
    PyObject *in_device = NULL;
    PedDevice *pass_device = NULL;
    PedDisk *out_disk = NULL;
    _ped_Disk *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Device_Type_obj, &in_device)) {
        return NULL;
    }

    pass_device = _ped_Device2PedDevice(in_device);
    if (pass_device) {
        out_disk = ped_disk_new(pass_device);
        if (out_disk == NULL) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException))
                    PyErr_SetString(IOException, partedExnMessage);
            }
            else
                PyErr_Format(IOException, "Failed to read partition table from device %s", pass_device->path);

            return NULL;
        }

        ret = PedDisk2_ped_Disk(out_disk);
        if (ret == NULL) {
            return NULL;
        }

        ped_device_destroy(pass_device);
        ped_disk_destroy(out_disk);
    }
    else {
        return NULL;
    }

    return (PyObject *) ret;
}

PyObject *py_ped_disk_new_fresh(PyObject *s, PyObject *args) {
    PyObject *in_device = NULL, *in_disktype = NULL;
    PedDevice *pass_device = NULL;
    PedDiskType *pass_disktype = NULL;
    PedDisk *out_disk = NULL;
    _ped_Disk *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_Device_Type_obj, &in_device,
                          &_ped_DiskType_Type_obj, &in_disktype)) {
        return NULL;
    }

    pass_device = _ped_Device2PedDevice(in_device);
    if (pass_device == NULL) {
        return NULL;
    }

    pass_disktype = _ped_DiskType2PedDiskType(in_disktype);
    if (pass_disktype == NULL) {
        return NULL;
    }

    out_disk = ped_disk_new_fresh(pass_device, pass_disktype);
    if (out_disk == NULL) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_Format(IOException, "Could not create new partition table on device %s", pass_device->path);

        return NULL;
    }

    ret = PedDisk2_ped_Disk(out_disk);
    if (ret == NULL) {
        return NULL;
    }

    ped_device_destroy(pass_device);
    free(pass_disktype);
    ped_disk_destroy(out_disk);

    return (PyObject *) ret;
}

PyObject *py_ped_disk_duplicate(PyObject *s, PyObject *args) {
    PyObject *in_disk = NULL;
    PedDisk *out_disk = NULL, *pass_disk = NULL;
    _ped_Disk *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Disk_Type_obj, &in_disk)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk) {
        pass_disk = ped_disk_duplicate(out_disk);
        if (pass_disk == NULL) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException))
                    PyErr_SetString(IOException, partedExnMessage);
            }
            else
                PyErr_Format(DiskException, "Could not duplicate device %s", out_disk->dev->path);

            return NULL;
        }

        ret = PedDisk2_ped_Disk(pass_disk);
        if (ret == NULL) {
            return NULL;
        }

        ped_disk_destroy(out_disk);
        ped_disk_destroy(pass_disk);
    }
    else {
        return NULL;
    }

    return (PyObject *) ret;
}

PyObject *py_ped_disk_destroy(PyObject *s, PyObject *args) {
    PyObject *in_disk = NULL;
    PedDisk *out_disk = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Disk_Type_obj, &in_disk)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk) {
        ped_disk_destroy(out_disk);
    }
    else {
        return NULL;
    }

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_commit(PyObject *s, PyObject *args) {
    PyObject *in_disk = NULL;
    PedDisk *out_disk = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Disk_Type_obj, &in_disk)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk) {
        ret = ped_disk_commit(out_disk);
        if (ret == 0) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException))
                    PyErr_SetString(IOException, partedExnMessage);
            }
            else
                PyErr_Format(DiskException, "Could not commit to disk %s", out_disk->dev->path);

            return NULL;
        }

        ped_disk_destroy(out_disk);
    }
    else {
        return NULL;
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_commit_to_dev(PyObject *s, PyObject *args) {
    PyObject *in_disk = NULL;
    PedDisk *out_disk = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Disk_Type_obj, &in_disk)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk) {
        ret = ped_disk_commit_to_dev(out_disk);
        if (ret == 0) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException))
                    PyErr_SetString(IOException, partedExnMessage);
            }
            else
                PyErr_Format(DiskException, "Could not commit to disk %s", out_disk->dev->path);

            return NULL;
        }

        ped_disk_destroy(out_disk);
    }
    else {
        return NULL;
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_commit_to_os(PyObject *s, PyObject *args) {
    PyObject *in_disk = NULL;
    PedDisk *out_disk = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Disk_Type_obj, &in_disk)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk) {
        ret = ped_disk_commit_to_os(out_disk);
        if (ret == 0) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException))
                    PyErr_SetString(IOException, partedExnMessage);
            }
            else
                PyErr_Format(DiskException, "Could not commit to disk %s", out_disk->dev->path);

            return NULL;
        }

        ped_disk_destroy(out_disk);
    }
    else {
        return NULL;
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_check(PyObject *s, PyObject *args) {
    PyObject *in_disk = NULL;
    PedDisk *out_disk = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Disk_Type_obj, &in_disk)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk) {
        ret = ped_disk_check(out_disk);
        if (ret == 0) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException))
                    PyErr_SetString(IOException, partedExnMessage);
            }
            else
                PyErr_Format(DiskException, "Could not check disk %s", out_disk->dev->path);

            return NULL;
        }

        ped_disk_destroy(out_disk);
    }
    else {
        return NULL;
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_print(PyObject *s, PyObject *args) {
    PyObject *in_disk = NULL;
    PedDisk *out_disk = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Disk_Type_obj, &in_disk)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk) {
        ped_disk_print(out_disk);
        ped_disk_destroy(out_disk);
    }
    else {
        return NULL;
    }

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_get_primary_partition_count(PyObject *s, PyObject *args) {
    PyObject *in_disk = NULL;
    PedDisk *out_disk = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Disk_Type_obj, &in_disk)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk) {
        ret = ped_disk_get_primary_partition_count(out_disk);
        ped_disk_destroy(out_disk);
    }
    else {
        return NULL;
    }

    return PyInt_FromLong(ret);
}

PyObject *py_ped_disk_get_last_partition_num(PyObject *s, PyObject *args) {
    PyObject *in_disk = NULL;
    PedDisk *out_disk = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Disk_Type_obj, &in_disk)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk) {
        ret = ped_disk_get_last_partition_num(out_disk);
        ped_disk_destroy(out_disk);
    }
    else {
        return NULL;
    }

    return PyInt_FromLong(ret);
}

PyObject *py_ped_disk_get_max_primary_partition_count(PyObject *s,
                                                      PyObject *args) {
    PyObject *in_disk = NULL;
    PedDisk *out_disk = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Disk_Type_obj, &in_disk)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk) {
        ret = ped_disk_get_max_primary_partition_count(out_disk);
        ped_disk_destroy(out_disk);
    }
    else {
        return NULL;
    }

    return PyInt_FromLong(ret);
}

PyObject *py_ped_partition_new(PyObject *s, PyObject *args) {
    PyObject *in_disk = NULL, *in_fs_type = NULL;
    PedDisk *out_disk = NULL;
    PedPartitionType type;
    PedFileSystemType *out_fs_type = NULL;
    PedSector start, end;
    PedPartition *pass_part = NULL;
    _ped_Partition *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!lO!ll", &_ped_Disk_Type_obj, &in_disk,
                          &type, &_ped_FileSystemType_Type_obj, &in_fs_type,
                          &start, &end)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk == NULL) {
        return NULL;
    }

    out_fs_type = _ped_FileSystemType2PedFileSystemType(in_fs_type);
    if (out_fs_type == NULL) {
        return NULL;
    }

    pass_part = ped_partition_new(out_disk, type, out_fs_type, start, end);
    if (pass_part == NULL) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(PartitionException, partedExnMessage);
        }
        else
            PyErr_Format(PartitionException, "Could not create new partition on device %s", out_disk->dev->path);

        return NULL;
    }

    ret = PedPartition2_ped_Partition(pass_part);
    if (ret == NULL) {
        return NULL;
    }

    ped_disk_destroy(out_disk);

    return (PyObject *) ret;
}

PyObject *py_ped_partition_destroy(PyObject *s, PyObject *args) {
    PyObject *in_partition = NULL;
    PedPartition *out_partition = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Partition_Type_obj,
                          &in_partition)) {
        return NULL;
    }

    out_partition = _ped_Partition2PedPartition(in_partition);
    if (out_partition) {
        ped_partition_destroy(out_partition);
    }
    else {
        return NULL;
    }

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_partition_is_active(PyObject *s, PyObject *args) {
    PyObject *in_partition = NULL;
    PedPartition *out_partition = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Partition_Type_obj,
                          &in_partition)) {
        return NULL;
    }

    out_partition = _ped_Partition2PedPartition(in_partition);
    if (out_partition) {
        ret = ped_partition_is_active(out_partition);
        ped_partition_destroy(out_partition);
    }
    else {
        return NULL;
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_partition_set_flag(PyObject *s, PyObject *args) {
    PyObject *in_part = NULL;
    int in_state = -1;
    PedPartition *out_part = NULL;
    PedPartitionFlag flag;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!li", &_ped_Partition_Type_obj, &in_part,
                          &flag, &in_state)) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part == NULL) {
        return NULL;
    }

    if (out_part && flag && in_state > -1) {
        ret = ped_partition_set_flag(out_part, flag, in_state);
        if (ret == 0) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException))
                    PyErr_SetString(PartitionException, partedExnMessage);
            }
            else
                PyErr_Format(PartitionException, "Could not set flag on partition %s%s", out_part->disk, out_part->num);

            return NULL;
        }

        ped_partition_destroy(out_part);
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_partition_get_flag(PyObject *s, PyObject *args) {
    PyObject *in_part = NULL;
    PedPartition *out_part = NULL;
    PedPartitionFlag flag;
    int ret = -1;

    if (!PyArg_ParseTuple(args, "O!l", &_ped_Partition_Type_obj, &in_part,
                          &flag)) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part == NULL) {
        return NULL;
    }

    ret = ped_partition_get_flag(out_part, flag);
    ped_partition_destroy(out_part);

    return PyInt_FromLong(ret);
}

PyObject *py_ped_partition_is_flag_available(PyObject *s, PyObject *args) {
    PyObject *in_part = NULL;
    PedPartition *out_part = NULL;
    PedPartitionFlag flag;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!l", &_ped_Partition_Type_obj, &in_part,
                          &flag)) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part == NULL) {
        return NULL;
    }

    ret = ped_partition_is_flag_available(out_part, flag);
    ped_partition_destroy(out_part);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_partition_set_system(PyObject *s, PyObject *args) {
    PyObject *in_part = NULL, *in_fstype = NULL;
    PedPartition *out_part = NULL;
    PedFileSystemType *out_fstype = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_Partition_Type_obj, &in_part,
                          &_ped_FileSystemType_Type_obj, &in_fstype)) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part == NULL) {
        return NULL;
    }

    out_fstype = _ped_FileSystemType2PedFileSystemType(in_fstype);
    if (out_fstype == NULL) {
        return NULL;
    }

    ret = ped_partition_set_system(out_part, out_fstype);
    if (ret == 0) {
        PyErr_Format(PartitionException, "Could not set system flag on partition %s%s", out_part->disk, out_part->num);
        return NULL;
    }

    ped_partition_destroy(out_part);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_partition_set_name(PyObject *s, PyObject *args) {
    PyObject *in_part = NULL;
    PedPartition *out_part = NULL;
    char *in_name = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!z", &_ped_Partition_Type_obj, &in_part,
                          &in_name)) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part) {
        ret = ped_partition_set_name(out_part, in_name);
        if (ret == 0) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException))
                    PyErr_SetString(PartitionException, partedExnMessage);
            }
            else
                PyErr_Format(PartitionException, "Could not set name on partition %s%s", out_part->disk, out_part->num);

            return NULL;
        }

        ped_partition_destroy(out_part);
    }
    else {
        return NULL;
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_partition_get_name(PyObject *s, PyObject *args) {
    PyObject *in_part = NULL;
    PedPartition *out_part = NULL;
    char *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Partition_Type_obj, &in_part)) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part) {
        ret = (char *) ped_partition_get_name(out_part);
        if (ret == NULL) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException))
                    PyErr_SetString(PartitionException, partedExnMessage);
            }
            else
                PyErr_Format(PartitionException, "Could not read name on partition %s%s", out_part->disk, out_part->num);

            return NULL;
        }

        ped_partition_destroy(out_part);
    }
    else {
        return NULL;
    }

    return PyString_FromString(ret);
}

PyObject *py_ped_partition_is_busy(PyObject *s, PyObject *args) {
    PyObject *in_part = NULL;
    PedPartition *out_part = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Partition_Type_obj, &in_part)) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part) {
        ret = ped_partition_is_busy(out_part);
        ped_partition_destroy(out_part);
    }
    else {
        return NULL;
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_partition_get_path(PyObject *s, PyObject *args) {
    PyObject *in_part = NULL;
    PedPartition *out_part = NULL;
    char *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Partition_Type_obj, &in_part)) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part) {
        ret = ped_partition_get_path(out_part);
        if (ret == NULL) {
            PyErr_Format(PartitionException, "Could not get path for partition %s%s", out_part->disk, out_part->num);
            return NULL;
        }

        ped_partition_destroy(out_part);
    }
    else {
        return NULL;
    }

    return PyString_FromString(ret);
}

PyObject *py_ped_partition_type_get_name(PyObject *s, PyObject *args) {
    PedPartitionType type;
    char *ret = NULL;

    if (!PyArg_ParseTuple(args, "l", &type)) {
        return NULL;
    }

    if (type) {
        ret = (char *) ped_partition_type_get_name(type);
    }

    return PyString_FromString(ret);
}

PyObject *py_ped_partition_flag_get_name(PyObject *s, PyObject *args) {
    PedPartitionFlag flag;
    char *ret = NULL;

    if (!PyArg_ParseTuple(args, "l", &flag)) {
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

    return PyString_FromString(ret);
}

PyObject *py_ped_partition_flag_get_by_name(PyObject *s, PyObject *args) {
    char *name = NULL;

    if (!PyArg_ParseTuple(args, "z", &name)) {
        return NULL;
    }

    return PyLong_FromLongLong(ped_partition_flag_get_by_name(name));
}

PyObject *py_ped_partition_flag_next(PyObject *s, PyObject *args) {
    PedPartitionFlag flag;

    if (!PyArg_ParseTuple(args, "l", &flag)) {
        return NULL;
    }

    return PyLong_FromLongLong(ped_partition_flag_next(flag));
}

PyObject *py_ped_disk_add_partition(PyObject *s, PyObject *args) {
    PyObject *in_disk = NULL, *in_part = NULL, *in_constraint = NULL;
    PedDisk *out_disk = NULL;
    PedPartition *out_part = NULL;
    PedConstraint *out_constraint = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!O!O!", &_ped_Disk_Type_obj, &in_disk,
                          &_ped_Partition_Type_obj, &in_part,
                          &_ped_Constraint_Type_obj, &in_constraint)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk == NULL) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part == NULL) {
        return NULL;
    }

    out_constraint = _ped_Constraint2PedConstraint(in_constraint);
    if (out_constraint == NULL) {
        return NULL;
    }

    ret = ped_disk_add_partition(out_disk, out_part, out_constraint);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(PartitionException, partedExnMessage);
        }
        else
            PyErr_Format(PartitionException, "Could not create partition %s%s", out_part->disk, out_part->num);

        return NULL;
    }

    ped_disk_destroy(out_disk);
    ped_partition_destroy(out_part);
    ped_constraint_destroy(out_constraint);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_remove_partition(PyObject *s, PyObject *args) {
    PyObject *in_disk = NULL, *in_part = NULL;
    PedDisk *out_disk = NULL;
    PedPartition *out_part = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_Disk_Type_obj, &in_disk,
                          &_ped_Partition_Type_obj, &in_part)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk == NULL) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part == NULL) {
        return NULL;
    }

    ret = ped_disk_remove_partition(out_disk, out_part);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(PartitionException, partedExnMessage);
        }
        else
            PyErr_Format(PartitionException, "Could not remove partition %s%s", out_part->disk, out_part->num);

        return NULL;
    }

    ped_disk_destroy(out_disk);
    ped_partition_destroy(out_part);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_delete_partition(PyObject *s, PyObject *args) {
    PyObject *in_disk = NULL, *in_part = NULL;
    PedDisk *out_disk = NULL;
    PedPartition *out_part = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_Disk_Type_obj, &in_disk,
                          &_ped_Partition_Type_obj, &in_part)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk == NULL) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part == NULL) {
        return NULL;
    }

    ret = ped_disk_delete_partition(out_disk, out_part);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(PartitionException, partedExnMessage);
        }
        else
            PyErr_Format(PartitionException, "Could not remove partition %s%s", out_part->disk, out_part->num);

        return NULL;
    }

    ped_disk_destroy(out_disk);
    ped_partition_destroy(out_part);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_delete_all(PyObject *s, PyObject *args) {
    PyObject *in_disk = NULL;
    PedDisk *out_disk = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Disk_Type_obj, &in_disk)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk) {
        ret = ped_disk_delete_all(out_disk);
        if (ret == 0) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException))
                    PyErr_SetString(PartitionException, partedExnMessage);
            }
            else
                PyErr_Format(PartitionException, "Could not remove all partitions on %s", out_disk->dev->path);

            return NULL;
        }

        ped_disk_destroy(out_disk);
    }
    else {
        return NULL;
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_set_partition_geom(PyObject *s, PyObject *args) {
    PyObject *in_disk = NULL, *in_part = NULL, *in_constraint = NULL;
    PedDisk *out_disk = NULL;
    PedPartition *out_part = NULL;
    PedConstraint *out_constraint = NULL;
    PedSector start, end;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!O!O!ll", &_ped_Disk_Type_obj, &in_disk,
                          &_ped_Partition_Type_obj, &in_part,
                          &_ped_Constraint_Type_obj, &in_constraint,
                          &start, &end)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk == NULL) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part == NULL) {
        return NULL;
    }

    out_constraint = _ped_Constraint2PedConstraint(in_constraint);
    if (out_constraint == NULL) {
        return NULL;
    }

    ret = ped_disk_set_partition_geom(out_disk, out_part, out_constraint,
                                      start, end);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(PartitionException, partedExnMessage);
        }
        else
            PyErr_Format(PartitionException, "Could not set geometry on %s%s", out_disk->dev->path, out_part->num);

        return NULL;
    }

    ped_disk_destroy(out_disk);
    ped_partition_destroy(out_part);
    ped_constraint_destroy(out_constraint);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_maximize_partition(PyObject *s, PyObject *args) {
    PyObject *in_disk = NULL, *in_part = NULL, *in_constraint = NULL;
    PedDisk *out_disk = NULL;
    PedPartition *out_part = NULL;
    PedConstraint *out_constraint = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!O!O!", &_ped_Disk_Type_obj, &in_disk,
                          &_ped_Partition_Type_obj, &in_part,
                          &_ped_Constraint_Type_obj, &in_constraint)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk == NULL) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part == NULL) {
        return NULL;
    }

    out_constraint = _ped_Constraint2PedConstraint(in_constraint);
    if (out_constraint == NULL) {
        return NULL;
    }

    ret = ped_disk_maximize_partition(out_disk, out_part, out_constraint);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(PartitionException, partedExnMessage);
        }
        else
            PyErr_Format(PartitionException, "Could not maximize partition size for %s%s", out_disk->dev->path, out_part->num);

        return NULL;
    }

    ped_disk_destroy(out_disk);
    ped_partition_destroy(out_part);
    ped_constraint_destroy(out_constraint);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_get_max_partition_geometry(PyObject *s, PyObject *args) {
    PyObject *in_disk = NULL, *in_part = NULL, *in_constraint = NULL;
    PedDisk *out_disk = NULL;
    PedPartition *out_part = NULL;
    PedConstraint *out_constraint = NULL;
    PedGeometry *pass_geom = NULL;
    _ped_Geometry *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!O!O!", &_ped_Disk_Type_obj, &in_disk,
                          &_ped_Partition_Type_obj, &in_part,
                          &_ped_Constraint_Type_obj, &in_constraint)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk == NULL) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part == NULL) {
        return NULL;
    }

    out_constraint = _ped_Constraint2PedConstraint(in_constraint);
    if (out_constraint == NULL) {
        return NULL;
    }

    pass_geom = ped_disk_get_max_partition_geometry(out_disk, out_part,
                                                    out_constraint);
    if (pass_geom == NULL) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(PartitionException, partedExnMessage);
        }
        else
            PyErr_Format(PartitionException, "Could not get maximum partition size for %s%s", out_disk->dev->path, out_part->num);

        return NULL;
    }

    ret = PedGeometry2_ped_Geometry(pass_geom);
    if (ret == NULL) {
        return NULL;
    }

    ped_geometry_destroy(pass_geom);
    ped_disk_destroy(out_disk);
    ped_partition_destroy(out_part);
    ped_constraint_destroy(out_constraint);

    return (PyObject *) ret;
}

PyObject *py_ped_disk_minimize_extended_partition(PyObject *s, PyObject *args) {
    PyObject *in_disk = NULL;
    PedDisk *out_disk = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Disk_Type_obj, &in_disk)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk) {
        ret = ped_disk_minimize_extended_partition(out_disk);
        if (ret == 0) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException))
                    PyErr_SetString(PartitionException, partedExnMessage);
            }
            else
                PyErr_Format(PartitionException, "Could not shrink extended partition on %s", out_disk->dev->path);

            return NULL;
        }

        ped_disk_destroy(out_disk);
    }
    else {
        return NULL;
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_next_partition(PyObject *s, PyObject *args) {
    PyObject *in_disk = NULL, *in_part = NULL;
    PedDisk *out_disk = NULL;
    PedPartition *out_part = NULL;
    PedPartition *pass_part = NULL;
    _ped_Partition *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_Disk_Type_obj, &in_disk,
                          &_ped_Partition_Type_obj, &in_part)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk == NULL) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part == NULL) {
        return NULL;
    }

    pass_part = ped_disk_next_partition(out_disk, out_part);
    if (pass_part == NULL) {
        PyErr_SetNone(PyExc_IndexError);
        return NULL;
    }

    ret = PedPartition2_ped_Partition(pass_part);
    if (ret == NULL) {
        return NULL;
    }

    ped_disk_destroy(out_disk);
    ped_partition_destroy(out_part);
    ped_partition_destroy(pass_part);

    return (PyObject *) ret;
}

PyObject *py_ped_disk_get_partition(PyObject *s, PyObject *args) {
    PyObject *in_disk = NULL;
    int num;
    PedDisk *out_disk = NULL;
    PedPartition *pass_part = NULL;
    _ped_Partition *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!i", &_ped_Disk_Type_obj, &in_disk, &num)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk) {
        pass_part = ped_disk_get_partition(out_disk, num);
        if (pass_part == NULL) {
            PyErr_SetString(PartitionException, "Partition does not exist");
            return NULL;
        }

        ret = PedPartition2_ped_Partition(pass_part);
        if (ret == NULL) {
            return NULL;
        }

        ped_disk_destroy(out_disk);
        ped_partition_destroy(pass_part);
    }
    else {
        return NULL;
    }

    return (PyObject *) ret;
}

PyObject *py_ped_disk_get_partition_by_sector(PyObject *s, PyObject *args) {
    PyObject *in_disk = NULL;
    PedDisk *out_disk = NULL;
    PedSector sector;
    PedPartition *pass_part = NULL;
    _ped_Partition *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!l", &_ped_Disk_Type_obj, &in_disk, &sector)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk == NULL) {
        return NULL;
    }

    pass_part = ped_disk_get_partition_by_sector(out_disk, sector);
    if (pass_part == NULL) {
        PyErr_SetString(PartitionException, "Partition does not exist");
        return NULL;
    }

    ret = PedPartition2_ped_Partition(pass_part);
    if (ret == NULL) {
        return NULL;
    }

    ped_disk_destroy(out_disk);
    ped_partition_destroy(pass_part);

    return (PyObject *) ret;
}

PyObject *py_ped_disk_extended_partition(PyObject *s, PyObject *args) {
    PyObject *in_disk = NULL;
    PedDisk *out_disk = NULL;
    PedPartition *pass_part = NULL;
    _ped_Partition *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Disk_Type_obj, &in_disk)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk) {
        pass_part = ped_disk_extended_partition(out_disk);
        if (pass_part == NULL) {
            PyErr_SetString(PartitionException, "Extended partition does not exist");
            return NULL;
        }

        ret = PedPartition2_ped_Partition(pass_part);
        if (ret == NULL) {
            return NULL;
        }

        ped_disk_destroy(out_disk);
        ped_partition_destroy(pass_part);
    }
    else {
        return NULL;
    }

    return (PyObject *) ret;
}
