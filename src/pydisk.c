/*
 * pydisk.c
 *
 * Copyright (C) 2007  Red Hat, Inc.
 * All rights reserved.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions of
 * the GNU General Public License v.2, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY expressed or implied, including the implied warranties of
 * MERCHANTABILITY or FITNESS FOR A * PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.  You should have received a copy of the
 * GNU General Public License along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.  Any Red Hat trademarks that are incorporated in the
 * source code or documentation are not subject to the GNU General Public
 * License and may only be used or replicated with the express permission of
 * Red Hat, Inc.
 *
 * Red Hat Author(s): David Cantrell <dcantrell@redhat.com>
 */

#include <Python.h>

#include "convert.h"
#include "pydisk.h"

/* _ped.PartitionType functions */
void _ped_PartitionType_dealloc(_ped_PartitionType *self) {
    PyObject_Del(self);
}

PyObject *_ped_PartitionType_new(PyTypeObject *type, PyObject *args,
                                 PyObject *kwds) {
    _ped_PartitionType *self;

    self = PyObject_New(_ped_PartitionType, &_ped_PartitionType_Type_obj);
    return (PyObject *) self;
}

int _ped_PartitionType_init(_ped_PartitionType *self, PyObject *args,
                            PyObject *kwds) {
    /* XXX */
    return 0;
}

/* _ped.PartitionFlag functions */
void _ped_PartitionFlag_dealloc(_ped_PartitionFlag *self) {
    PyObject_Del(self);
}

PyObject *_ped_PartitionFlag_new(PyTypeObject *type, PyObject *args,
                                 PyObject *kwds) {
    _ped_PartitionFlag *self;

    self = PyObject_New(_ped_PartitionFlag, &_ped_PartitionFlag_Type_obj);
    return (PyObject *) self;
}

int _ped_PartitionFlag_init(_ped_PartitionFlag *self, PyObject *args,
                            PyObject *kwds) {
    /* XXX */
    return 0;
}

/* _ped.Partition functions */
void _ped_Partition_dealloc(_ped_Partition *self) {
    PyObject_Del(self);
}

PyObject *_ped_Partition_new(PyTypeObject *type, PyObject *args,
                             PyObject *kwds) {
    _ped_Partition *self;

    self = PyObject_New(_ped_Partition, &_ped_Partition_Type_obj);
    return (PyObject *) self;
}

int _ped_Partition_init(_ped_Partition *self, PyObject *args, PyObject *kwds) {
    /* XXX */
    return 0;
}

/* _ped.Disk functions */
void _ped_Disk_dealloc(_ped_Disk *self) {
    PyObject_Del(self);
}

PyObject *_ped_Disk_new(PyTypeObject *type, PyObject *args,
                             PyObject *kwds) {
    _ped_Disk *self;

    self = (_ped_Disk *) type->tp_alloc(type, 0);
    return (PyObject *) self;
}

int _ped_Disk_init(_ped_Disk *self, PyObject *args, PyObject *kwds) {
    /* XXX */
    return 0;
}

/* _ped.DiskType functions */
void _ped_DiskType_dealloc(_ped_DiskType *self) {
    PyObject_Del(self);
}

PyObject *_ped_DiskType_new(PyTypeObject *type, PyObject *args,
                             PyObject *kwds) {
    _ped_DiskType *self;

    self = (_ped_DiskType *) type->tp_alloc(type, 0);
    return (PyObject *) self;
}

int _ped_DiskType_init(_ped_DiskType *self, PyObject *args, PyObject *kwds) {
    /* XXX */
    return 0;
}

/* _ped.DiskTypeFeature functions */
void _ped_DiskTypeFeature_dealloc(_ped_DiskTypeFeature *self) {
    PyObject_Del(self);
}

PyObject *_ped_DiskTypeFeature_new(PyTypeObject *type, PyObject *args,
                             PyObject *kwds) {
    _ped_DiskTypeFeature *self;

    self = (_ped_DiskTypeFeature *) type->tp_alloc(type, 0);
    return (PyObject *) self;
}

int _ped_DiskTypeFeature_init(_ped_DiskTypeFeature *self, PyObject *args,
                              PyObject *kwds) {
    /* XXX */
    return 0;
}

/* 1:1 function mappings for disk.h in libparted */
PyObject *py_ped_disk_type_register(PyObject *s, PyObject *args) {
    PyObject *in_disktype;
    PedDiskType *out_disktype;

    if (!PyArg_ParseTuple(args, "O", &in_disktype)) {
        return NULL;
    }

    out_disktype = _ped_DiskType2PedDiskType(in_disktype);
    ped_disk_type_register(out_disktype);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_type_unregister(PyObject *s, PyObject *args) {
    PyObject *in_disktype;
    PedDiskType *out_disktype;

    if (!PyArg_ParseTuple(args, "O", &in_disktype)) {
        return NULL;
    }

    out_disktype = _ped_DiskType2PedDiskType(in_disktype);
    ped_disk_type_unregister(out_disktype);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_type_get_next(PyObject *s, PyObject *args) {
    PyObject *in_type;
    PedDiskType *out_type = NULL, *ret_type = NULL;
    _ped_DiskType *ret = NULL;

    if (!PyArg_ParseTuple(args, "O", &in_type)) {
        return NULL;
    }

    out_type = _ped_DiskType2PedDiskType(in_type);
    if (out_type) {
        ret_type = ped_disk_type_get_next(out_type);
        ret = PedDiskType2_ped_DiskType(ret_type);
    }

    return (PyObject *) ret;
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
        ret = PedDiskType2_ped_DiskType(out_type);
    }

    return (PyObject *) ret;
}

PyObject *py_ped_disk_type_check_feature(PyObject *s, PyObject *args) {
    PyObject *in_disktype, *in_feature;
    PedDiskType *out_disktype = NULL;
    PedDiskTypeFeature *out_feature = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "OO", &in_disktype, &in_feature)) {
        return NULL;
    }

    out_disktype = _ped_DiskType2PedDiskType(in_disktype);
    out_feature = _ped_DiskTypeFeature2PedDiskTypeFeature(in_feature);
    if (out_disktype && out_feature) {
        ret = ped_disk_type_check_feature(out_disktype, *out_feature);
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_probe(PyObject *s, PyObject *args) {
    PyObject *in_device;
    PedDevice *out_device;
    PedDiskType *out_type = NULL;
    _ped_DiskType *ret = NULL;

    if (!PyArg_ParseTuple(args, "O", &in_device)) {
        return NULL;
    }

    out_device = _ped_Device2PedDevice(in_device);
    if (in_device) {
        out_type = ped_disk_probe(out_device);
        ret = PedDiskType2_ped_DiskType(out_type);
    }

    return (PyObject *) ret;
}

PyObject *py_ped_disk_clobber(PyObject *s, PyObject *args) {
    PyObject *in_device;
    PedDevice *out_device = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O", &in_device)) {
        return NULL;
    }

    out_device = _ped_Device2PedDevice(in_device);
    if (out_device) {
        ret = ped_disk_clobber(out_device);
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_clobber_exclude(PyObject *s, PyObject *args) {
    PyObject *in_device, *in_disktype;
    PedDevice *out_device = NULL;
    PedDiskType *out_disktype = NULL;
    int ret = 0;

    if (!PyArg_ParseType(args, "OO", &in_device, &in_disktype)) {
        return NULL;
    }

    out_device = _ped_Device2PedDevice(in_device);
    out_disktype = _ped_DiskType2PedDiskType(in_disktype);

    if (out_device && out_disktype) {
        ret = ped_disk_clobber_exclude(out_device, out_disktype);
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_new(PyObject *s, PyObject *args) {
    PyObject *in_device;
    PedDevice *pass_device = NULL;
    PedDisk *out_disk = NULL;
    _ped_Disk *ret = NULL;

    if (!PyArg_ParseTuple(args, "O", &in_device)) {
        return NULL;
    }

    pass_device = _ped_Device2PedDevice(in_device);
    if (pass_device) {
        out_disk = ped_disk_new(pass_device);
        ret = PedDisk2_ped_Disk(out_disk);
    }

    return (PyObject *) ret;
}

PyObject *py_ped_disk_new_fresh(PyObject *s, PyObject *args) {
    PyObject *in_device, *in_disktype;
    PedDevice *pass_device = NULL;
    PedDiskType *pass_disktype = NULL;
    PedDisk *out_disk = NULL;
    _ped_Disk *ret = NULL;

    if (!PyArg_ParseTuple(args, "OO", &in_device, &in_disktype)) {
        return NULL;
    }

    pass_device = _ped_Device2PedDevice(in_device);
    pass_disktype = _ped_DiskType2PedDiskType(in_disktype);
    if (pass_device && pass_disktype) {
        out_disk = ped_disk_new_fresh(pass_device, pass_disktype);
        ret = PedDisk2_ped_Disk(out_disk);
    }

    return (PyObject *) ret;
}

PyObject *py_ped_disk_duplicate(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_disk_destroy(PyObject *s, PyObject *args) {
    PyObject *in_disk;
    PedDisk *out_disk = NULL;

    if (!PyArg_ParseTuple(args, "O", &in_disk)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk) {
        ped_disk_destroy(out_disk);
    }

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_commit(PyObject *s, PyObject *args) {
    PyObject *in_disk;
    PedDisk *out_disk = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O", &in_disk)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk) {
        ret = ped_disk_commit(out_disk);
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_commit_to_dev(PyObject *s, PyObject *args) {
    PyObject *in_disk;
    PedDisk *out_disk = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O", &in_disk)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk) {
        ret = ped_disk_commit_to_dev(out_disk);
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_commit_to_os(PyObject *s, PyObject *args) {
    PyObject *in_disk;
    PedDisk *out_disk = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O", &in_disk)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk) {
        ret = ped_disk_commit_to_os(out_disk);
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_check(PyObject *s, PyObject *args) {
    PyObject *in_disk;
    PedDisk *out_disk = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O", &in_disk)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk) {
        ret = ped_disk_check(out_disk);
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_print(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_disk_get_primary_partition_count(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_disk_get_last_partition_num(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_disk_get_max_primary_partition_count(PyObject *s,
                                                      PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_partition_new(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_partition_destroy(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_partition_is_active(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_partition_set_flag(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_partition_get_flag(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_partition_is_flag_available(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_partition_set_system(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_partition_set_name(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_partition_get_name(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_partition_is_busy(PyObject *s, PyObject *args) {
    PyObject *in_part;
    PedPartition *out_part = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O", &in_part)) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part) {
        ret = ped_partition_is_busy(out_part);
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_partition_get_path(PyObject *s, PyObject *args) {
    PyObject *in_part;
    PedPartition *out_part = NULL;
    char *ret = NULL;

    if (!PyArg_ParseTuple(args, "O", &in_part)) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part) {
        ret = ped_partition_get_path(out_part);
    }

    return PyString_FromString(ret);
}

PyObject *py_ped_partition_type_get_name(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_partition_flag_get_name(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_partition_flag_get_by_name(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_partition_flag_next(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_disk_add_partition(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_disk_remove_partition(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_disk_delete_partition(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_disk_delete_all(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_disk_set_partition_geom(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_disk_maximize_partition(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_disk_get_max_partition_geometry(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_disk_minimize_extended_partition(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_disk_next_partition(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_disk_get_partition(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_disk_get_partition_by_sector(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_disk_extended_partition(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}
