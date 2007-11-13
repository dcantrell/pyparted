/*
 * pydisk.h
 * pyparted type definitions for pydisk.c
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

#ifndef PYDISK_H_INCLUDED
#define PYDISK_H_INCLUDED

#include <Python.h>
#include <structmember.h>

#include <parted/parted.h>

/* _ped.Partition type is the Python equivalent of PedPartition
 * in libparted */
typedef struct {
    PyObject_HEAD

    /* PedPartition members */
    PyObject *disk;                /* _ped.Disk */
    PyObject *geom;                /* _ped.Geometry */
    int num;
    long long type;                /* PedPartitionType */
    PyObject *fs_type;             /* _ped.FileSystemType */
} _ped_Partition;

static PyMemberDef _ped_Partition_members[] = {
    {"disk", T_OBJECT, offsetof(_ped_Partition, disk), 0, NULL},
    {"geom", T_OBJECT, offsetof(_ped_Partition, geom), 0, NULL},
    {"fs_type", T_OBJECT, offsetof(_ped_Partition, fs_type), 0, NULL},
    {NULL}
};

static PyMethodDef _ped_Partition_methods[] = {
    {NULL}
};

void _ped_Partition_dealloc(_ped_Partition *self);
PyObject *_ped_Partition_new(PyTypeObject *type, PyObject *args,
                             PyObject *kwds);
int _ped_Partition_init(_ped_Partition *self, PyObject *args, PyObject *kwds);
PyObject *_ped_Partition_get(_ped_Partition *self, char *member);

static PyGetSetDef _ped_Partition_getset[] = {
    {"num", (getter) _ped_Partition_get, NULL,
            "PedPartition num", "num"},
    {"type", (getter) _ped_Partition_get, NULL,
            "PedPartition type", "type"},
    {NULL}  /* Sentinel */
};

static PyTypeObject _ped_Partition_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    .tp_name = "_ped.Partition",
    .tp_basicsize = sizeof(_ped_Partition),
    .tp_dealloc = (destructor) _ped_Partition_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_CHECKTYPES |
                Py_TPFLAGS_BASETYPE,
    .tp_doc = "PedPartition objects",
    .tp_methods = _ped_Partition_methods,
    .tp_members = _ped_Partition_members,
    .tp_getset = _ped_Partition_getset,
    .tp_init = (initproc) _ped_Partition_init,
    .tp_new = _ped_Partition_new,
};

/* _ped.Disk type is the Python equivalent of PedDisk in libparted */
typedef struct {
    PyObject_HEAD

    /* PedDisk members */
    PyObject *dev;             /* _ped.Device */
    PyObject *type;            /* _ped.DiskType */
} _ped_Disk;

static PyMemberDef _ped_Disk_members[] = {
    {"dev", T_OBJECT, offsetof(_ped_Disk, dev), 0, NULL},
    {"type", T_OBJECT, offsetof(_ped_Disk, type), 0, NULL},
    {NULL}
};

static PyMethodDef _ped_Disk_methods[] = {
    {NULL}
};

void _ped_Disk_dealloc(_ped_Disk *self);
PyObject *_ped_Disk_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
int _ped_Disk_init(_ped_Disk *self, PyObject *args, PyObject *kwds);

static PyGetSetDef _ped_Disk_getset[] = {
    {NULL}  /* Sentinel */
};

static PyTypeObject _ped_Disk_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    .tp_name = "_ped.Disk",
    .tp_basicsize = sizeof(_ped_Disk),
    .tp_dealloc = (destructor) _ped_Disk_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_doc = "PedDisk objects",
    .tp_methods = _ped_Disk_methods,
    .tp_members = _ped_Disk_members,
    .tp_getset = _ped_Disk_getset,
    .tp_init = (initproc) _ped_Disk_init,
    .tp_new = _ped_Disk_new,
};

/* _ped.DiskType type is the Python equivalent of PedDiskType in libparted */
typedef struct {
    PyObject_HEAD

    /* PedDiskType members */
    char *name;
    long long features;        /* PedDiskTypeFeature */
} _ped_DiskType;

static PyMemberDef _ped_DiskType_members[] = {
    {NULL}
};

static PyMethodDef _ped_DiskType_methods[] = {
    {NULL}
};

void _ped_DiskType_dealloc(_ped_DiskType *self);
PyObject *_ped_DiskType_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
int _ped_DiskType_init(_ped_DiskType *self, PyObject *args, PyObject *kwds);
PyObject *_ped_DiskType_get(_ped_DiskType *self, char *member);

static PyGetSetDef _ped_DiskType_getset[] = {
    {"name", (getter) _ped_DiskType_get, NULL,
             "DiskType name", "name"},
    {"features", (getter) _ped_DiskType_get, NULL,
             "DiskType features", "features"},
    {NULL}  /* Sentinel */
};

static PyTypeObject _ped_DiskType_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    .tp_name = "_ped.DiskType",
    .tp_basicsize = sizeof(_ped_DiskType),
    .tp_dealloc = (destructor) _ped_DiskType_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_CHECKTYPES |
                Py_TPFLAGS_BASETYPE,
    .tp_doc = "PedDiskType objects",
    .tp_methods = _ped_DiskType_methods,
    .tp_members = _ped_DiskType_members,
    .tp_getset = _ped_DiskType_getset,
    .tp_init = (initproc) _ped_DiskType_init,
    .tp_new = _ped_DiskType_new,
};

/* 1:1 function mappings for disk.h in libparted */
PyObject *py_ped_disk_type_register(PyObject *s, PyObject *args);
PyObject *py_ped_disk_type_unregister(PyObject *s, PyObject *args);
PyObject *py_ped_disk_type_get_next(PyObject *s, PyObject *args);
PyObject *py_ped_disk_type_get(PyObject *s, PyObject *args);
PyObject *py_ped_disk_type_check_feature(PyObject *s, PyObject *args);
PyObject *py_ped_disk_probe(PyObject *s, PyObject *args);
PyObject *py_ped_disk_clobber(PyObject *s, PyObject *args);
PyObject *py_ped_disk_clobber_exclude(PyObject *s, PyObject *args);
PyObject *py_ped_disk_new(PyObject *s, PyObject *args);
PyObject *py_ped_disk_new_fresh(PyObject *s, PyObject *args);
PyObject *py_ped_disk_duplicate(PyObject *s, PyObject *args);
PyObject *py_ped_disk_destroy(PyObject *s, PyObject *args);
PyObject *py_ped_disk_commit(PyObject *s, PyObject *args);
PyObject *py_ped_disk_commit_to_dev(PyObject *s, PyObject *args);
PyObject *py_ped_disk_commit_to_os(PyObject *s, PyObject *args);
PyObject *py_ped_disk_check(PyObject *s, PyObject *args);
PyObject *py_ped_disk_print(PyObject *s, PyObject *args);
PyObject *py_ped_disk_get_primary_partition_count(PyObject *s, PyObject *args);
PyObject *py_ped_disk_get_last_partition_num(PyObject *s, PyObject *args);
PyObject *py_ped_disk_get_max_primary_partition_count(PyObject *s,
                                                      PyObject *args);
PyObject *py_ped_partition_new(PyObject *s, PyObject *args);
PyObject *py_ped_partition_destroy(PyObject *s, PyObject *args);
PyObject *py_ped_partition_is_active(PyObject *s, PyObject *args);
PyObject *py_ped_partition_set_flag(PyObject *s, PyObject *args);
PyObject *py_ped_partition_get_flag(PyObject *s, PyObject *args);
PyObject *py_ped_partition_is_flag_available(PyObject *s, PyObject *args);
PyObject *py_ped_partition_set_system(PyObject *s, PyObject *args);
PyObject *py_ped_partition_set_name(PyObject *s, PyObject *args);
PyObject *py_ped_partition_get_name(PyObject *s, PyObject *args);
PyObject *py_ped_partition_is_busy(PyObject *s, PyObject *args);
PyObject *py_ped_partition_get_path(PyObject *s, PyObject *args);
PyObject *py_ped_partition_type_get_name(PyObject *s, PyObject *args);
PyObject *py_ped_partition_flag_get_name(PyObject *s, PyObject *args);
PyObject *py_ped_partition_flag_get_by_name(PyObject *s, PyObject *args);
PyObject *py_ped_partition_flag_next(PyObject *s, PyObject *args);
PyObject *py_ped_disk_add_partition(PyObject *s, PyObject *args);
PyObject *py_ped_disk_remove_partition(PyObject *s, PyObject *args);
PyObject *py_ped_disk_delete_partition(PyObject *s, PyObject *args);
PyObject *py_ped_disk_delete_all(PyObject *s, PyObject *args);
PyObject *py_ped_disk_set_partition_geom(PyObject *s, PyObject *args);
PyObject *py_ped_disk_maximize_partition(PyObject *s, PyObject *args);
PyObject *py_ped_disk_get_max_partition_geometry(PyObject *s, PyObject *args);
PyObject *py_ped_disk_minimize_extended_partition(PyObject *s, PyObject *args);
PyObject *py_ped_disk_next_partition(PyObject *s, PyObject *args);
PyObject *py_ped_disk_get_partition(PyObject *s, PyObject *args);
PyObject *py_ped_disk_get_partition_by_sector(PyObject *s, PyObject *args);
PyObject *py_ped_disk_extended_partition(PyObject *s, PyObject *args);

#endif /* PYDISK_H_INCLUDED */
