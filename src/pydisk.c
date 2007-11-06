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

#include <stdlib.h>

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
    /* do not free out_disktype here because it's now in libparted's list */

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
    free(out_disktype);

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
        free(out_type);
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
        free(out_type);
    }

    return (PyObject *) ret;
}

PyObject *py_ped_disk_type_check_feature(PyObject *s, PyObject *args) {
    PyObject *in_disktype, *in_feature;
    PedDiskType *out_disktype = NULL;
    PedDiskTypeFeature out_feature = -1;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "OO", &in_disktype, &in_feature)) {
        return NULL;
    }

    out_disktype = _ped_DiskType2PedDiskType(in_disktype);
    out_feature = _ped_DiskTypeFeature2PedDiskTypeFeature(in_feature);
    if (out_disktype && out_feature) {
        ret = ped_disk_type_check_feature(out_disktype, out_feature);
        free(out_disktype);
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
        free(out_type);
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
        ped_device_destroy(out_device);
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_clobber_exclude(PyObject *s, PyObject *args) {
    PyObject *in_device, *in_disktype;
    PedDevice *out_device = NULL;
    PedDiskType *out_disktype = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "OO", &in_device, &in_disktype)) {
        return NULL;
    }

    out_device = _ped_Device2PedDevice(in_device);
    out_disktype = _ped_DiskType2PedDiskType(in_disktype);

    if (out_device && out_disktype) {
        ret = ped_disk_clobber_exclude(out_device, out_disktype);
        ped_device_destroy(out_device);
        free(out_disktype);
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
        ped_device_destroy(pass_device);
        ped_disk_destroy(out_disk);
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

        ped_device_destroy(pass_device);
        free(pass_disktype);
        ped_disk_destroy(out_disk);
    }

    return (PyObject *) ret;
}

PyObject *py_ped_disk_duplicate(PyObject *s, PyObject *args) {
    PyObject *in_disk;
    PedDisk *out_disk = NULL, *pass_disk = NULL;
    _ped_Disk *ret = NULL;

    if (!PyArg_ParseTuple(args, "O", &in_disk)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk) {
        pass_disk = ped_disk_duplicate(out_disk);
        ret = PedDisk2_ped_Disk(pass_disk);

        ped_disk_destroy(out_disk);
        ped_disk_destroy(pass_disk);
    }

    return (PyObject *) ret;
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
        ped_disk_destroy(out_disk);
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
        ped_disk_destroy(out_disk);
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
        ped_disk_destroy(out_disk);
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
        ped_disk_destroy(out_disk);
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_print(PyObject *s, PyObject *args) {
    PyObject *in_disk;
    PedDisk *out_disk = NULL;

    if (!PyArg_ParseTuple(args, "O", &in_disk)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk) {
        ped_disk_print(out_disk);
        ped_disk_destroy(out_disk);
    }

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_get_primary_partition_count(PyObject *s, PyObject *args) {
    PyObject *in_disk;
    PedDisk *out_disk = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O", &in_disk)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk) {
        ret = ped_disk_get_primary_partition_count(out_disk);
        ped_disk_destroy(out_disk);
    }

    return PyInt_FromLong(ret);
}

PyObject *py_ped_disk_get_last_partition_num(PyObject *s, PyObject *args) {
    PyObject *in_disk;
    PedDisk *out_disk = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O", &in_disk)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk) {
        ret = ped_disk_get_last_partition_num(out_disk);
        ped_disk_destroy(out_disk);
    }

    return PyInt_FromLong(ret);
}

PyObject *py_ped_disk_get_max_primary_partition_count(PyObject *s,
                                                      PyObject *args) {
    PyObject *in_disk;
    PedDisk *out_disk = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O", &in_disk)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk) {
        ret = ped_disk_get_max_primary_partition_count(out_disk);
        ped_disk_destroy(out_disk);
    }

    return PyInt_FromLong(ret);
}

PyObject *py_ped_partition_new(PyObject *s, PyObject *args) {
    PyObject *in_disk, *in_type, *in_fs_type, *in_start, *in_end;
    PedDisk *out_disk = NULL;
    PedPartitionType out_type;
    PedFileSystemType *out_fs_type = NULL;
    PedSector out_start, out_end;
    PedPartition *pass_part = NULL;
    _ped_Partition *ret = NULL;

    if (!PyArg_ParseTuple(args, "OOOOO", &in_disk, &in_type, &in_fs_type,
                          &in_start, &in_end)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    out_type = _ped_PartitionType2PedPartitionType(in_type);
    out_fs_type = _ped_FileSystemType2PedFileSystemType(in_fs_type);
    out_start = _ped_Sector2PedSector(in_start);
    out_end = _ped_Sector2PedSector(in_end);

    if (out_disk && out_type && out_fs_type && out_start && out_end) {
        pass_part = ped_partition_new(out_disk, out_type, out_fs_type,
                                      out_start, out_end);
        ret = PedPartition2_ped_Partition(pass_part);

        ped_disk_destroy(out_disk);
        ped_file_system_type_destroy(out_fs_type);
    }

    return (PyObject *) ret;
}

PyObject *py_ped_partition_destroy(PyObject *s, PyObject *args) {
    PyObject *in_partition;
    PedPartition *out_partition = NULL;

    if (!PyArg_ParseTuple(args, "O", &in_partition)) {
        return NULL;
    }

    out_partition = _ped_Partition2PedPartition(in_partition);
    if (out_partition) {
        ped_partition_destroy(out_partition);
    }

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_partition_is_active(PyObject *s, PyObject *args) {
    PyObject *in_partition;
    PedPartition *out_partition = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O", &in_partition)) {
        return NULL;
    }

    out_partition = _ped_Partition2PedPartition(in_partition);
    if (out_partition) {
        ret = ped_partition_is_active(out_partition);
        ped_partition_destroy(out_partition);
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_partition_set_flag(PyObject *s, PyObject *args) {
    PyObject *in_part, *in_flag;
    int in_state = -1;
    PedPartition *out_part = NULL;
    PedPartitionFlag out_flag;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "OOi", &in_part, &in_flag, &in_state)) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    out_flag = _ped_PartitionFlag2PedPartitionFlag(in_flag);

    if (out_part && out_flag && in_state > -1) {
        ret = ped_partition_set_flag(out_part, out_flag, in_state);

        ped_partition_destroy(out_part);
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_partition_get_flag(PyObject *s, PyObject *args) {
    PyObject *in_part, *in_flag;
    PedPartition *out_part = NULL;
    PedPartitionFlag out_flag;
    int ret = -1;

    if (!PyArg_ParseTuple(args, "OO", &in_part, &in_flag)) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    out_flag = _ped_PartitionFlag2PedPartitionFlag(in_flag);

    if (out_part && out_flag) {
        ret = ped_partition_get_flag(out_part, out_flag);

        ped_partition_destroy(out_part);
    }

    return PyInt_FromLong(ret);
}

PyObject *py_ped_partition_is_flag_available(PyObject *s, PyObject *args) {
    PyObject *in_part, *in_flag;
    PedPartition *out_part = NULL;
    PedPartitionFlag out_flag;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "OO", &in_part, &in_flag)) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    out_flag = _ped_PartitionFlag2PedPartitionFlag(in_flag);

    if (out_part && out_flag) {
        ret = ped_partition_is_flag_available(out_part, out_flag);

        ped_partition_destroy(out_part);
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_partition_set_system(PyObject *s, PyObject *args) {
    PyObject *in_part, *in_fstype;
    PedPartition *out_part = NULL;
    PedFileSystemType *out_fstype = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "OO", &in_part, &in_fstype)) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    out_fstype = _ped_FileSystemType2PedFileSystemType(in_fstype);

    if (out_part && out_fstype) {
        ret = ped_partition_set_system(out_part, out_fstype);

        ped_partition_destroy(out_part);
        ped_file_system_type_destroy(out_fstype);
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_partition_set_name(PyObject *s, PyObject *args) {
    PyObject *in_part;
    PedPartition *out_part = NULL;
    char *in_name = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "Os", &in_part, &in_name)) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part) {
        ret = ped_partition_set_name(out_part, in_name);
        ped_partition_destroy(out_part);
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_partition_get_name(PyObject *s, PyObject *args) {
    PyObject *in_part;
    PedPartition *out_part = NULL;
    char *ret = NULL;

    if (!PyArg_ParseTuple(args, "O", &in_part)) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part) {
        ret = (char *) ped_partition_get_name(out_part);
        ped_partition_destroy(out_part);
    }

    return PyString_FromString(ret);
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
        ped_partition_destroy(out_part);
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
        ped_partition_destroy(out_part);
    }

    return PyString_FromString(ret);
}

PyObject *py_ped_partition_type_get_name(PyObject *s, PyObject *args) {
    PyObject *in_type;
    PedPartitionType out_type;
    char *ret = NULL;

    if (!PyArg_ParseTuple(args, "O", &in_type)) {
        return NULL;
    }

    out_type = _ped_PartitionType2PedPartitionType(in_type);
    if (out_type) {
        ret = (char *) ped_partition_type_get_name(out_type);
    }

    return PyString_FromString(ret);
}

PyObject *py_ped_partition_flag_get_name(PyObject *s, PyObject *args) {
    PyObject *in_flag;
    PedPartitionFlag out_flag;
    char *ret = NULL;

    if (!PyArg_ParseTuple(args, "O", &in_flag)) {
        return NULL;
    }

    out_flag = _ped_PartitionFlag2PedPartitionFlag(in_flag);
    if (out_flag) {
        ret = (char *) ped_partition_flag_get_name(out_flag);
    }

    return PyString_FromString(ret);
}

PyObject *py_ped_partition_flag_get_by_name(PyObject *s, PyObject *args) {
    char *in_name;
    PedPartitionFlag pass_flag;
    _ped_PartitionFlag *ret = NULL;

    if (!PyArg_ParseTuple(args, "s", &in_name)) {
        return NULL;
    }

    pass_flag = ped_partition_flag_get_by_name(in_name);
    ret = PedPartitionFlag2_ped_PartitionFlag(pass_flag);

    return (PyObject *) ret;
}

PyObject *py_ped_partition_flag_next(PyObject *s, PyObject *args) {
    PyObject *in_flag;
    PedPartitionFlag out_flag, pass_flag;
    _ped_PartitionFlag *ret = NULL;

    if (!PyArg_ParseTuple(args, "O", &in_flag)) {
        return NULL;
    }

    out_flag = _ped_PartitionFlag2PedPartitionFlag(in_flag);
    pass_flag = ped_partition_flag_next(out_flag);
    ret = PedPartitionFlag2_ped_PartitionFlag(pass_flag);

    return (PyObject *) ret;
}

PyObject *py_ped_disk_add_partition(PyObject *s, PyObject *args) {
    PyObject *in_disk, *in_part, *in_constraint;
    PedDisk *out_disk = NULL;
    PedPartition *out_part = NULL;
    PedConstraint *out_constraint = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "OOO", &in_disk, &in_part, &in_constraint)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    out_part = _ped_Partition2PedPartition(in_part);
    out_constraint = _ped_Constraint2PedConstraint(in_constraint);

    if (out_disk && out_part && out_constraint) {
        ret = ped_disk_add_partition(out_disk, out_part, out_constraint);

        ped_disk_destroy(out_disk);
        ped_partition_destroy(out_part);
        ped_constraint_destroy(out_constraint);
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_remove_partition(PyObject *s, PyObject *args) {
    PyObject *in_disk, *in_part;
    PedDisk *out_disk = NULL;
    PedPartition *out_part = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "OO", &in_disk, &in_part)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    out_part = _ped_Partition2PedPartition(in_part);

    if (out_disk && out_part) {
        ret = ped_disk_remove_partition(out_disk, out_part);

        ped_disk_destroy(out_disk);
        ped_partition_destroy(out_part);
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_delete_partition(PyObject *s, PyObject *args) {
    PyObject *in_disk, *in_part;
    PedDisk *out_disk = NULL;
    PedPartition *out_part = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "OO", &in_disk, &in_part)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    out_part = _ped_Partition2PedPartition(in_part);

    if (out_disk && out_part) {
        ret = ped_disk_delete_partition(out_disk, out_part);

        ped_disk_destroy(out_disk);
        ped_partition_destroy(out_part);
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_delete_all(PyObject *s, PyObject *args) {
    PyObject *in_disk;
    PedDisk *out_disk = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O", &in_disk)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk) {
        ret = ped_disk_delete_all(out_disk);
        ped_disk_destroy(out_disk);
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_set_partition_geom(PyObject *s, PyObject *args) {
    PyObject *in_disk, *in_part, *in_constraint, *in_start, *in_end;
    PedDisk *out_disk = NULL;
    PedPartition *out_part = NULL;
    PedConstraint *out_constraint = NULL;
    PedSector out_start, out_end;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "OOOOO", &in_disk, &in_part, &in_constraint,
                          &in_start, &in_end)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    out_part = _ped_Partition2PedPartition(in_part);
    out_constraint = _ped_Constraint2PedConstraint(in_constraint);
    out_start = _ped_Sector2PedSector(in_start);
    out_end = _ped_Sector2PedSector(in_end);

    if (out_disk && out_part && out_constraint && out_start && out_end) {
        ret = ped_disk_set_partition_geom(out_disk, out_part, out_constraint,
                                          out_start, out_end);

        ped_disk_destroy(out_disk);
        ped_partition_destroy(out_part);
        ped_constraint_destroy(out_constraint);
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_maximize_partition(PyObject *s, PyObject *args) {
    PyObject *in_disk, *in_part, *in_constraint;
    PedDisk *out_disk = NULL;
    PedPartition *out_part = NULL;
    PedConstraint *out_constraint = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "OOO", &in_disk, &in_part, &in_constraint)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    out_part = _ped_Partition2PedPartition(in_part);
    out_constraint = _ped_Constraint2PedConstraint(in_constraint);

    if (out_disk && out_part && out_constraint) {
        ret = ped_disk_maximize_partition(out_disk, out_part, out_constraint);

        ped_disk_destroy(out_disk);
        ped_partition_destroy(out_part);
        ped_constraint_destroy(out_constraint);
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_get_max_partition_geometry(PyObject *s, PyObject *args) {
    PyObject *in_disk, *in_part, *in_constraint;
    PedDisk *out_disk = NULL;
    PedPartition *out_part = NULL;
    PedConstraint *out_constraint = NULL;
    PedGeometry *pass_geom = NULL;
    _ped_Geometry *ret = NULL;

    if (!PyArg_ParseTuple(args, "OOO", &in_disk, &in_part, &in_constraint)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    out_part = _ped_Partition2PedPartition(in_part);
    out_constraint = _ped_Constraint2PedConstraint(in_constraint);

    if (out_disk && out_part && out_constraint) {
        pass_geom = ped_disk_get_max_partition_geometry(out_disk, out_part,
                                                        out_constraint);
        ret = PedGeometry2_ped_Geometry(pass_geom);

        ped_geometry_destroy(pass_geom);
        ped_disk_destroy(out_disk);
        ped_partition_destroy(out_part);
        ped_constraint_destroy(out_constraint);
    }

    return (PyObject *) ret;
}

PyObject *py_ped_disk_minimize_extended_partition(PyObject *s, PyObject *args) {
    PyObject *in_disk;
    PedDisk *out_disk = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O", &in_disk)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk) {
        ret = ped_disk_minimize_extended_partition(out_disk);
        ped_disk_destroy(out_disk);
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_next_partition(PyObject *s, PyObject *args) {
    PyObject *in_disk, *in_part;
    PedDisk *out_disk = NULL;
    PedPartition *out_part = NULL;
    PedPartition *pass_part = NULL;
    _ped_Partition *ret = NULL;

    if (!PyArg_ParseTuple(args, "OO", &in_disk, &in_part)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    out_part = _ped_Partition2PedPartition(in_part);

    if (out_disk && out_part) {
        pass_part = ped_disk_next_partition(out_disk, out_part);
        ret = PedPartition2_ped_Partition(pass_part);

        ped_disk_destroy(out_disk);
        ped_partition_destroy(out_part);
        ped_partition_destroy(pass_part);
    }

    return (PyObject *) ret;
}

PyObject *py_ped_disk_get_partition(PyObject *s, PyObject *args) {
    PyObject *in_disk;
    int num;
    PedDisk *out_disk = NULL;
    PedPartition *pass_part = NULL;
    _ped_Partition *ret = NULL;

    if (!PyArg_ParseTuple(args, "Oi", &in_disk, &num)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk) {
        pass_part = ped_disk_get_partition(out_disk, num);
        ret = PedPartition2_ped_Partition(pass_part);

        ped_disk_destroy(out_disk);
        ped_partition_destroy(pass_part);
    }

    return (PyObject *) ret;
}

PyObject *py_ped_disk_get_partition_by_sector(PyObject *s, PyObject *args) {
    PyObject *in_disk, *in_sect;
    PedDisk *out_disk = NULL;
    PedSector out_sect;
    PedPartition *pass_part = NULL;
    _ped_Partition *ret = NULL;

    if (!PyArg_ParseTuple(args, "OO", &in_disk, &in_sect)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    out_sect = _ped_Sector2PedSector(in_sect);

    if (out_disk && out_sect) {
        pass_part = ped_disk_get_partition_by_sector(out_disk, out_sect);
        ret = PedPartition2_ped_Partition(pass_part);

        ped_disk_destroy(out_disk);
        ped_partition_destroy(pass_part);
    }

    return (PyObject *) ret;
}

PyObject *py_ped_disk_extended_partition(PyObject *s, PyObject *args) {
    PyObject *in_disk;
    PedDisk *out_disk = NULL;
    PedPartition *pass_part = NULL;
    _ped_Partition *ret = NULL;

    if (!PyArg_ParseTuple(args, "O", &in_disk)) {
        return NULL;
    }

    out_disk = _ped_Disk2PedDisk(in_disk);
    if (out_disk) {
        pass_part = ped_disk_extended_partition(out_disk);
        ret = PedPartition2_ped_Partition(pass_part);

        ped_disk_destroy(out_disk);
        ped_partition_destroy(pass_part);
    }

    return (PyObject *) ret;
}
