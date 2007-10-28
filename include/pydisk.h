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

/* _ped.PartitionType type is the Python equivalent of PedPartitionType
 * in libparted */
typedef struct {
    PyObject_HEAD

    /* a PedPartitionType is an enumerated type, so store a long here */
    long val;
} _ped_PartitionType;

static PyMemberDef _ped_PartitionType_members[] = {
    {"val", T_LONG, offsetof(_ped_PartitionType, val), 0, NULL},
    {NULL}
};

static PyMethodDef _ped_PartitionType_methods[] = {
    {NULL}
};

void _ped_PartitionType_dealloc(_ped_PartitionType *self);
PyObject *_ped_PartitionType_new(PyTypeObject *type, PyObject *args,
                                 PyObject *kwds);
int _ped_PartitionType_init(_ped_PartitionType *self, PyObject *args,
                            PyObject *kwds);

static PyGetSetDef _ped_PartitionType_getset[] = {
    {NULL}  /* Sentinel */
};

static PyTypeObject _ped_PartitionType_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    0,                                         /* ob_size */
    "_ped.PartitionType",                      /* tp_name */
    sizeof(_ped_PartitionType),                /* tp_basicsize */
    0,                                         /* tp_itemsize */
    (destructor) _ped_PartitionType_dealloc,   /* tp_dealloc */
    0,                                         /* tp_print */
    0,                                         /* tp_getattr */
    0,                                         /* tp_setattr */
    0,                                         /* tp_compare */
    0,                                         /* tp_repr */
    0,                                         /* tp_as_number */
    0,                                         /* tp_as_sequence */
    0,                                         /* tp_as_mapping */
    0,                                         /* tp_hash */
    0,                                         /* tp_call */
    0,                                         /* tp_str */
    0,                                         /* tp_getattro */
    0,                                         /* tp_setattro */
    0,                                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,  /* tp_flags */
    "PedPartitionType objects",                /* tp_doc */
    0,                                         /* tp_traverse */
    0,                                         /* tp_clear */
    0,                                         /* tp_richcompare */
    0,                                         /* tp_weaklistoffset */
    0,                                         /* tp_iter */
    0,                                         /* tp_iternext */
    _ped_PartitionType_methods,                /* tp_methods */
    _ped_PartitionType_members,                /* tp_members */
    _ped_PartitionType_getset,                 /* tp_getset */
    0,                                         /* tp_base */
    0,                                         /* tp_dict */
    0,                                         /* tp_descr_get */
    0,                                         /* tp_descr_set */
    0,                                         /* tp_dictoffset */
    (initproc) _ped_PartitionType_init,        /* tp_init */
    0,                                         /* tp_alloc */
    _ped_PartitionType_new,                    /* tp_new */
};

/* _ped.PartitionFlag type is the Python equivalent of PedPartitionFlag
 * in libparted */
typedef struct {
    PyObject_HEAD

    /* a PedPartitionFlag is a long, so store a long long here */
    long val;
} _ped_PartitionFlag;

static PyMemberDef _ped_PartitionFlag_members[] = {
    {"val", T_LONG, offsetof(_ped_PartitionFlag, val), 0, NULL},
    {NULL}
};

static PyMethodDef _ped_PartitionFlag_methods[] = {
    {NULL}
};

void _ped_PartitionFlag_dealloc(_ped_PartitionFlag *self);
PyObject *_ped_PartitionFlag_new(PyTypeObject *type, PyObject *args,
                                 PyObject *kwds);
int _ped_PartitionFlag_init(_ped_PartitionFlag *self, PyObject *args,
                            PyObject *kwds);

static PyGetSetDef _ped_PartitionFlag_getset[] = {
    {NULL}  /* Sentinel */
};

static PyTypeObject _ped_PartitionFlag_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    0,                                         /* ob_size */
    "_ped.PartitionFlag",                      /* tp_name */
    sizeof(_ped_PartitionFlag),                /* tp_basicsize */
    0,                                         /* tp_itemsize */
    (destructor) _ped_PartitionFlag_dealloc,   /* tp_dealloc */
    0,                                         /* tp_print */
    0,                                         /* tp_getattr */
    0,                                         /* tp_setattr */
    0,                                         /* tp_compare */
    0,                                         /* tp_repr */
    0,                                         /* tp_as_number */
    0,                                         /* tp_as_sequence */
    0,                                         /* tp_as_mapping */
    0,                                         /* tp_hash */
    0,                                         /* tp_call */
    0,                                         /* tp_str */
    0,                                         /* tp_getattro */
    0,                                         /* tp_setattro */
    0,                                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,  /* tp_flags */
    "PedPartitionFlag objects",                /* tp_doc */
    0,                                         /* tp_traverse */
    0,                                         /* tp_clear */
    0,                                         /* tp_richcompare */
    0,                                         /* tp_weaklistoffset */
    0,                                         /* tp_iter */
    0,                                         /* tp_iternext */
    _ped_PartitionFlag_methods,                /* tp_methods */
    _ped_PartitionFlag_members,                /* tp_members */
    _ped_PartitionFlag_getset,                 /* tp_getset */
    0,                                         /* tp_base */
    0,                                         /* tp_dict */
    0,                                         /* tp_descr_get */
    0,                                         /* tp_descr_set */
    0,                                         /* tp_dictoffset */
    (initproc) _ped_PartitionFlag_init,        /* tp_init */
    0,                                         /* tp_alloc */
    _ped_PartitionFlag_new,                    /* tp_new */
};

/* _ped.Partition type is the Python equivalent of PedPartition
 * in libparted */
typedef struct {
    PyObject_HEAD

    /* PedPartition members */
    PyObject *prev;                /* _ped.Partition */
    PyObject *next;                /* _ped.Partition */
    PyObject *disk;                /* _ped.Disk */
    PyObject *geom;                /* _ped.Geometry */
    int num;
    PyObject *type;                /* _ped.PartitionType */
    PyObject *fs_type;             /* _ped.FileSystemType */
    PyObject *part_list;           /* _ped.Partition */
    void *disk_specific;
} _ped_Partition;

static PyMemberDef _ped_Partition_members[] = {
    {"prev", T_OBJECT, offsetof(_ped_Partition, prev), 0, NULL},
    {"next", T_OBJECT, offsetof(_ped_Partition, next), 0, NULL},
    {"disk", T_OBJECT, offsetof(_ped_Partition, disk), 0, NULL},
    {"geom", T_OBJECT, offsetof(_ped_Partition, geom), 0, NULL},
    {"num", T_INT, offsetof(_ped_Partition, num), 0, NULL},
    {"type", T_OBJECT, offsetof(_ped_Partition, type), 0, NULL},
    {"fs_type", T_OBJECT, offsetof(_ped_Partition, fs_type), 0, NULL},
    {"part_list", T_OBJECT, offsetof(_ped_Partition, part_list), 0, NULL},
    {NULL}
};

static PyMethodDef _ped_Partition_methods[] = {
    {NULL}
};

void _ped_Partition_dealloc(_ped_Partition *self);
PyObject *_ped_Partition_new(PyTypeObject *type, PyObject *args,
                             PyObject *kwds);
int _ped_Partition_init(_ped_Partition *self, PyObject *args, PyObject *kwds);

static PyGetSetDef _ped_Partition_getset[] = {
    {NULL}  /* Sentinel */
};

static PyTypeObject _ped_Partition_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    0,                                         /* ob_size */
    "_ped.Partition",                          /* tp_name */
    sizeof(_ped_Partition),                    /* tp_basicsize */
    0,                                         /* tp_itemsize */
    (destructor) _ped_Partition_dealloc,       /* tp_dealloc */
    0,                                         /* tp_print */
    0,                                         /* tp_getattr */
    0,                                         /* tp_setattr */
    0,                                         /* tp_compare */
    0,                                         /* tp_repr */
    0,                                         /* tp_as_number */
    0,                                         /* tp_as_sequence */
    0,                                         /* tp_as_mapping */
    0,                                         /* tp_hash */
    0,                                         /* tp_call */
    0,                                         /* tp_str */
    0,                                         /* tp_getattro */
    0,                                         /* tp_setattro */
    0,                                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,  /* tp_flags */
    "PedPartition objects",                    /* tp_doc */
    0,                                         /* tp_traverse */
    0,                                         /* tp_clear */
    0,                                         /* tp_richcompare */
    0,                                         /* tp_weaklistoffset */
    0,                                         /* tp_iter */
    0,                                         /* tp_iternext */
    _ped_Partition_methods,                    /* tp_methods */
    _ped_Partition_members,                    /* tp_members */
    _ped_Partition_getset,                     /* tp_getset */
    0,                                         /* tp_base */
    0,                                         /* tp_dict */
    0,                                         /* tp_descr_get */
    0,                                         /* tp_descr_set */
    0,                                         /* tp_dictoffset */
    (initproc) _ped_Partition_init,            /* tp_init */
    0,                                         /* tp_alloc */
    _ped_Partition_new,                        /* tp_new */
};

/* _ped.Disk type is the Python equivalent of PedDisk in libparted */
typedef struct {
    PyObject_HEAD

    /* PedDisk members */
    PyObject *dev;             /* _ped.Device */
    PyObject *type;            /* _ped.DiskType */
    int *block_sizes;
    PyObject *part_list;       /* _ped.Partition */
    void *disk_specific;
} _ped_Disk;

static PyMemberDef _ped_Disk_members[] = {
    {"dev", T_OBJECT, offsetof(_ped_Disk, dev), 0, NULL},
    {"type", T_OBJECT, offsetof(_ped_Disk, type), 0, NULL},
    {"block_sizes", T_INT, offsetof(_ped_Disk, block_sizes), 0, NULL},
    {"part_list", T_OBJECT, offsetof(_ped_Disk, part_list), 0, NULL},
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
    0,                                         /* ob_size */
    "_ped.Disk",                               /* tp_name */
    sizeof(_ped_Disk),                         /* tp_basicsize */
    0,                                         /* tp_itemsize */
    (destructor) _ped_Disk_dealloc,            /* tp_dealloc */
    0,                                         /* tp_print */
    0,                                         /* tp_getattr */
    0,                                         /* tp_setattr */
    0,                                         /* tp_compare */
    0,                                         /* tp_repr */
    0,                                         /* tp_as_number */
    0,                                         /* tp_as_sequence */
    0,                                         /* tp_as_mapping */
    0,                                         /* tp_hash */
    0,                                         /* tp_call */
    0,                                         /* tp_str */
    0,                                         /* tp_getattro */
    0,                                         /* tp_setattro */
    0,                                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,  /* tp_flags */
    "PedDisk objects",                         /* tp_doc */
    0,                                         /* tp_traverse */
    0,                                         /* tp_clear */
    0,                                         /* tp_richcompare */
    0,                                         /* tp_weaklistoffset */
    0,                                         /* tp_iter */
    0,                                         /* tp_iternext */
    _ped_Disk_methods,                         /* tp_methods */
    _ped_Disk_members,                         /* tp_members */
    _ped_Disk_getset,                          /* tp_getset */
    0,                                         /* tp_base */
    0,                                         /* tp_dict */
    0,                                         /* tp_descr_get */
    0,                                         /* tp_descr_set */
    0,                                         /* tp_dictoffset */
    (initproc) _ped_Disk_init,                 /* tp_init */
    0,                                         /* tp_alloc */
    _ped_Disk_new,                             /* tp_new */
};

/* _ped.DiskType type is the Python equivalent of PedDiskType in libparted */
typedef struct {
    PyObject_HEAD

    /* PedDiskType members */
    PyObject *next;            /* _ped.DiskType */
    char *name;
    PedDiskOps *ops;
    PyObject *features;        /* _ped.DiskTypeFeature */
} _ped_DiskType;

static PyMemberDef _ped_DiskType_members[] = {
    {"next", T_OBJECT, offsetof(_ped_DiskType, next), 0, NULL},
    {"name", T_STRING_INPLACE, offsetof(_ped_DiskType, name), 0, NULL},
    {"features", T_OBJECT, offsetof(_ped_DiskType, features), 0, NULL},
    {NULL}
};

static PyMethodDef _ped_DiskType_methods[] = {
    {NULL}
};

void _ped_DiskType_dealloc(_ped_DiskType *self);
PyObject *_ped_DiskType_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
int _ped_DiskType_init(_ped_DiskType *self, PyObject *args, PyObject *kwds);

static PyGetSetDef _ped_DiskType_getset[] = {
    {NULL}  /* Sentinel */
};

static PyTypeObject _ped_DiskType_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    0,                                         /* ob_size */
    "_ped.DiskType",                           /* tp_name */
    sizeof(_ped_DiskType),                     /* tp_basicsize */
    0,                                         /* tp_itemsize */
    (destructor) _ped_DiskType_dealloc,        /* tp_dealloc */
    0,                                         /* tp_print */
    0,                                         /* tp_getattr */
    0,                                         /* tp_setattr */
    0,                                         /* tp_compare */
    0,                                         /* tp_repr */
    0,                                         /* tp_as_number */
    0,                                         /* tp_as_sequence */
    0,                                         /* tp_as_mapping */
    0,                                         /* tp_hash */
    0,                                         /* tp_call */
    0,                                         /* tp_str */
    0,                                         /* tp_getattro */
    0,                                         /* tp_setattro */
    0,                                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,  /* tp_flags */
    "PedDiskType objects",                     /* tp_doc */
    0,                                         /* tp_traverse */
    0,                                         /* tp_clear */
    0,                                         /* tp_richcompare */
    0,                                         /* tp_weaklistoffset */
    0,                                         /* tp_iter */
    0,                                         /* tp_iternext */
    _ped_DiskType_methods,                     /* tp_methods */
    _ped_DiskType_members,                     /* tp_members */
    _ped_DiskType_getset,                      /* tp_getset */
    0,                                         /* tp_base */
    0,                                         /* tp_dict */
    0,                                         /* tp_descr_get */
    0,                                         /* tp_descr_set */
    0,                                         /* tp_dictoffset */
    (initproc) _ped_DiskType_init,             /* tp_init */
    0,                                         /* tp_alloc */
    _ped_DiskType_new,                         /* tp_new */
};

/* _ped.DiskTypeFeature type is the Python equivalent of PedDiskTypeFeature
 * in libparted */
typedef struct {
    PyObject_HEAD

    /* a PedDiskTypeFeature is a long, so store a long here */
    long val;
} _ped_DiskTypeFeature;

static PyMemberDef _ped_DiskTypeFeature_members[] = {
    {"val", T_LONG, offsetof(_ped_DiskTypeFeature, val), 0, NULL},
    {NULL}
};

static PyMethodDef _ped_DiskTypeFeature_methods[] = {
    {NULL}
};

void _ped_DiskTypeFeature_dealloc(_ped_DiskTypeFeature *self);
PyObject *_ped_DiskTypeFeature_new(PyTypeObject *type, PyObject *args,
                                   PyObject *kwds);
int _ped_DiskTypeFeature_init(_ped_DiskTypeFeature *self, PyObject *args,
                              PyObject *kwds);

static PyGetSetDef _ped_DiskTypeFeature_getset[] = {
    {NULL}  /* Sentinel */
};

static PyTypeObject _ped_DiskTypeFeature_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    0,                                         /* ob_size */
    "_ped.DiskTypeFeature",                    /* tp_name */
    sizeof(_ped_DiskTypeFeature),              /* tp_basicsize */
    0,                                         /* tp_itemsize */
    (destructor) _ped_DiskTypeFeature_dealloc, /* tp_dealloc */
    0,                                         /* tp_print */
    0,                                         /* tp_getattr */
    0,                                         /* tp_setattr */
    0,                                         /* tp_compare */
    0,                                         /* tp_repr */
    0,                                         /* tp_as_number */
    0,                                         /* tp_as_sequence */
    0,                                         /* tp_as_mapping */
    0,                                         /* tp_hash */
    0,                                         /* tp_call */
    0,                                         /* tp_str */
    0,                                         /* tp_getattro */
    0,                                         /* tp_setattro */
    0,                                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,  /* tp_flags */
    "PedDiskTypeFeature objects",              /* tp_doc */
    0,                                         /* tp_traverse */
    0,                                         /* tp_clear */
    0,                                         /* tp_richcompare */
    0,                                         /* tp_weaklistoffset */
    0,                                         /* tp_iter */
    0,                                         /* tp_iternext */
    _ped_DiskTypeFeature_methods,              /* tp_methods */
    _ped_DiskTypeFeature_members,              /* tp_members */
    _ped_DiskTypeFeature_getset,               /* tp_getset */
    0,                                         /* tp_base */
    0,                                         /* tp_dict */
    0,                                         /* tp_descr_get */
    0,                                         /* tp_descr_set */
    0,                                         /* tp_dictoffset */
    (initproc) _ped_DiskTypeFeature_init,      /* tp_init */
    0,                                         /* tp_alloc */
    _ped_DiskTypeFeature_new,                  /* tp_new */
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
