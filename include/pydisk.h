/*
 * pydisk.h
 * pyparted type definitions for pydisk.c
 *
 * Copyright (C) 2007, 2008 Red Hat, Inc.
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
 *                    Chris Lumens <clumens@redhat.com>
 */

#ifndef PYDISK_H_INCLUDED
#define PYDISK_H_INCLUDED

#include <Python.h>
#include <structmember.h>

#include <parted/parted.h>

/* 1:1 function mappings for disk.h in libparted */
PyObject *py_ped_disk_type_register(PyObject *s, PyObject *args);
PyObject *py_ped_disk_type_unregister(PyObject *s, PyObject *args);
PyObject *py_ped_disk_type_get_next(PyObject *s, PyObject *args);
PyObject *py_ped_disk_type_get(PyObject *s, PyObject *args);
PyObject *py_ped_disk_type_check_feature(PyObject *s, PyObject *args);
PyObject *py_ped_disk_probe(PyObject *s, PyObject *args);
PyObject *py_ped_disk_clobber(PyObject *s, PyObject *args);
PyObject *py_ped_disk_clobber_exclude(PyObject *s, PyObject *args);
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

PyDoc_STRVAR(partition_destroy_doc,
"destroy(self) -> None\n\n"
"Destroys the Partition object.");

PyDoc_STRVAR(partition_is_active_doc,
"is_active(self) -> boolean\n\n"
"Return whether self is active or not.");

PyDoc_STRVAR(partition_set_flag_doc,
"set_flag(self, flag, state) -> boolean\n\n"
"Sets the state of the given flag on self .  Flags have different types of\n"
"different types of disk labels, and are not guaranteed to exist on all disk\n"
"label types.  If provided with an invalid flag for the disk's label,\n"
"_ped.PartitionException is raised.");

PyDoc_STRVAR(partition_get_flag_doc,
"get_flag(self, flag) -> integer\n\n"
"Return the state of the given flag on self.  There is no check for invalid\n"
"flag types, so these will always return 0.  It is therefore recommended to\n"
"call self.is_flag_available() first to make sure.");

PyDoc_STRVAR(partition_is_flag_available_doc,
"is_flag_available(self, flag) -> boolean\n\n"
"Return whether the given flag is valid for self.");

PyDoc_STRVAR(partition_set_system_doc,
"set_system(self, FileSystemType) -> boolean\n\n"
"Set the system type on self to FileSystemType.  On error,\n"
"_ped.PartitionException is raised.");

PyDoc_STRVAR(partition_set_name_doc,
"set_name(self, string) -> boolean\n\n"
"On disk labels that support it, this method sets the partition's name.\n"
"Before attempting this operation, DiskType.check_feature() can be used to\n"
"determine if it is even supported.  On error, _ped.PartitionException will\n"
"be raised.");

PyDoc_STRVAR(partition_get_name_doc,
"get_name(self) -> string\n\n"
"On disk labels that support it, this method returns the partition's name.  On\n"
"all other disk labels, _ped.PartitionException will be raised.  Before calling\n"
"this method, DiskType.check_feature() can be called to check for support.");

PyDoc_STRVAR(partition_is_busy_doc,
"is_busy(self) -> boolean\n\n"
"Return whether self is busy or not.  The most likely reason for a partition\n"
"to be busy is because it's mounted.  Additionally, extended partitions are\n"
"busy if any of their logical partitions are busy.");

PyDoc_STRVAR(partition_get_path_doc,
"get_path(self) -> string\n\n"
"Return a path that could be used for addressing self at an operating system\n"
"level.  For instance, on Linux this could return '/dev/sda' for a partition.\n"
"If an error occurs, _ped.PartitionException is raised.");

/* _ped.Partition type is the Python equivalent of PedPartition
 * in libparted */
typedef struct {
    PyObject_HEAD

    /* PedPartition members */
    PyObject *disk;                /* _ped.Disk */
    PyObject *geom;                /* _ped.Geometry */
    int num;
    long type;                     /* PedPartitionType */
    PyObject *fs_type;             /* _ped.FileSystemType */
} _ped_Partition;

static PyMemberDef _ped_Partition_members[] = {
    {"disk", T_OBJECT, offsetof(_ped_Partition, disk), 0,
             "The _ped.Disk this Partition exists on."},
    {"geom", T_OBJECT, offsetof(_ped_Partition, geom), 0,
             "A _ped.Geometry object describing the region this Partition occupies."},
    {"fs_type", T_OBJECT, offsetof(_ped_Partition, fs_type), 0,
                "A _ped.FileSystemType object describing the filesystem on this Partition."},
    {NULL}
};

static PyMethodDef _ped_Partition_methods[] = {
    {"destroy", (PyCFunction) py_ped_partition_destroy, METH_VARARGS,
                partition_destroy_doc},
    {"is_active", (PyCFunction) py_ped_partition_is_active, METH_VARARGS,
                  partition_is_active_doc},
    {"set_flag", (PyCFunction) py_ped_partition_set_flag, METH_VARARGS,
                 partition_set_flag_doc},
    {"get_flag", (PyCFunction) py_ped_partition_get_flag, METH_VARARGS,
                 partition_get_flag_doc},
    {"is_flag_available", (PyCFunction) py_ped_partition_is_flag_available,
                          METH_VARARGS, partition_is_flag_available_doc},
    {"set_system", (PyCFunction) py_ped_partition_set_system,
                   METH_VARARGS, partition_set_system_doc},
    {"set_name", (PyCFunction) py_ped_partition_set_name, METH_VARARGS,
                 partition_set_name_doc},
    {"get_name", (PyCFunction) py_ped_partition_get_name, METH_VARARGS,
                 partition_get_name_doc},
    {"is_busy", (PyCFunction) py_ped_partition_is_busy, METH_VARARGS,
                partition_is_busy_doc},
    {"get_path", (PyCFunction) py_ped_partition_get_path, METH_VARARGS,
                 partition_get_path_doc},
    {NULL}
};

void _ped_Partition_dealloc(_ped_Partition *self);
int _ped_Partition_traverse(_ped_Partition *self, visitproc visit, void *arg);
int _ped_Partition_clear(_ped_Partition *self);
int _ped_Partition_init(_ped_Partition *self, PyObject *args, PyObject *kwds);
PyObject *_ped_Partition_get(_ped_Partition *self, void *closure);
int _ped_Partition_set(_ped_Partition *self, PyObject *value, void *closure);

static PyGetSetDef _ped_Partition_getset[] = {
    {"num", (getter) _ped_Partition_get, (setter) _ped_Partition_set,
            "The number of this Partition on self.disk.", "num"},
    {"type", (getter) _ped_Partition_get, (setter) _ped_Partition_set,
             "PedPartition type", "type"},
    {NULL}  /* Sentinel */
};

PyDoc_STRVAR(_ped_Partition_doc,
"A _ped.Partition object describes a single partition on a disk.  Operations\n"
"on Partition objects are limited to getting and setting flags, names, and\n"
"paths.  All other operations you may wish to do involving partitions are\n"
"done through a _ped.Disk or _ped.FileSystem object.  These objects all exist\n"
"as attributes of a Partition, though.\n\n"
"Valid flags for Partitions are given by the _ped.PARTITION_* constants,\n"
"though not all flags are valid for every disk label type.\n\n"
"For most errors involving a Partition object, _ped.PartitionException will\n"
"be raised.");

static PyTypeObject _ped_Partition_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    .tp_name = "_ped.Partition",
    .tp_basicsize = sizeof(_ped_Partition),
 /* .tp_itemsize = XXX */
    .tp_dealloc = (destructor) _ped_Partition_dealloc,
 /* .tp_print = XXX */
 /* .tp_getattr = XXX */
 /* .tp_setattr = XXX */
 /* .tp_compare = XXX */
 /* .tp_repr = XXX */
 /* .tp_as_number = XXX */
 /* .tp_as_sequence = XXX */
 /* .tp_as_mapping = XXX */
 /* .tp_hash = XXX */
 /* .tp_call = XXX */
 /* .tp_str = XXX */
    .tp_getattro = PyObject_GenericGetAttr,
 /* .tp_setattro = XXX */
 /* .tp_as_buffer = XXX */
    .tp_flags = Py_TPFLAGS_HAVE_CLASS | Py_TPFLAGS_CHECKTYPES |
                Py_TPFLAGS_HAVE_GC | Py_TPFLAGS_BASETYPE,
    .tp_doc = _ped_Partition_doc,
    .tp_traverse = (traverseproc) _ped_Partition_traverse,
    .tp_clear = (inquiry) _ped_Partition_clear,
 /* .tp_richcompare = XXX */
 /* .tp_weaklistoffset = XXX */
 /* .tp_iter = XXX */
 /* .tp_iternext = XXX */
    .tp_methods = _ped_Partition_methods,
    .tp_members = _ped_Partition_members,
    .tp_getset = _ped_Partition_getset,
 /* .tp_base = XXX */
 /* .tp_dict = XXX */
 /* .tp_descr_get = XXX */
 /* .tp_descr_set = XXX */
 /* .tp_dictoffset = XXX */
    .tp_init = (initproc) _ped_Partition_init,
    .tp_alloc = PyType_GenericAlloc,
    .tp_new = PyType_GenericNew,
 /* .tp_free = XXX */
 /* .tp_is_gc = XXX */
 /* .tp_bases = XXX */
 /* .tp_mro = XXX */
 /* .tp_cache = XXX */
 /* .tp_subclasses = XXX */
 /* .tp_weaklist = XXX */
 /* .tp_del = XXX */
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
    {"probe", (PyCFunction) py_ped_disk_probe, METH_VARARGS, NULL},
    {"clobber", (PyCFunction) py_ped_disk_clobber, METH_VARARGS, NULL},
    {"clobber_exclude", (PyCFunction) py_ped_disk_clobber_exclude,
                        METH_VARARGS, NULL},
    {"duplicate", (PyCFunction) py_ped_disk_duplicate, METH_VARARGS, NULL},
    {"destroy", (PyCFunction) py_ped_disk_destroy, METH_VARARGS, NULL},
    {"commit", (PyCFunction) py_ped_disk_commit, METH_VARARGS, NULL},
    {"commit_to_dev", (PyCFunction) py_ped_disk_commit_to_dev,
                      METH_VARARGS, NULL},
    {"commit_to_os", (PyCFunction) py_ped_disk_commit_to_os,
                     METH_VARARGS, NULL},
    {"check", (PyCFunction) py_ped_disk_check, METH_VARARGS, NULL},
    {"print", (PyCFunction) py_ped_disk_print, METH_VARARGS, NULL},
    {"get_primary_partition_count", (PyCFunction)
                                    py_ped_disk_get_primary_partition_count,
                                    METH_VARARGS, NULL},
    {"get_last_partition_num", (PyCFunction)
                               py_ped_disk_get_last_partition_num,
                               METH_VARARGS, NULL},
    {"get_max_primary_partition_count", (PyCFunction)
                                   py_ped_disk_get_max_primary_partition_count,
                                   METH_VARARGS, NULL},
    {"add_partition", (PyCFunction) py_ped_disk_add_partition,
                      METH_VARARGS, NULL},
    {"remove_partition", (PyCFunction) py_ped_disk_remove_partition,
                         METH_VARARGS, NULL},
    {"delete_partition", (PyCFunction) py_ped_disk_delete_partition,
                         METH_VARARGS, NULL},
    {"delete_all", (PyCFunction) py_ped_disk_delete_all, METH_VARARGS, NULL},
    {"set_partition_geom", (PyCFunction) py_ped_disk_set_partition_geom,
                           METH_VARARGS, NULL},
    {"maximize_partition", (PyCFunction) py_ped_disk_maximize_partition,
                           METH_VARARGS, NULL},
    {"get_max_partition_geometry", (PyCFunction)
                                   py_ped_disk_get_max_partition_geometry,
                                   METH_VARARGS, NULL},
    {"minimize_extended_partition", (PyCFunction)
                                    py_ped_disk_minimize_extended_partition,
                                    METH_VARARGS, NULL},
    {"next_partition", (PyCFunction) py_ped_disk_next_partition,
                       METH_VARARGS, NULL},
    {"get_partition", (PyCFunction) py_ped_disk_get_partition,
                      METH_VARARGS, NULL},
    {"get_partition_by_sector", (PyCFunction)
                                py_ped_disk_get_partition_by_sector,
                                METH_VARARGS, NULL},
    {"extended_partition", (PyCFunction) py_ped_disk_extended_partition,
                           METH_VARARGS, NULL},
    {NULL}
};

void _ped_Disk_dealloc(_ped_Disk *self);
int _ped_Disk_traverse(_ped_Disk *self, visitproc visit, void *arg);
int _ped_Disk_clear(_ped_Disk *self);
int _ped_Disk_init(_ped_Disk *self, PyObject *args, PyObject *kwds);

static PyGetSetDef _ped_Disk_getset[] = {
    {NULL}  /* Sentinel */
};

static PyTypeObject _ped_Disk_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    .tp_name = "_ped.Disk",
    .tp_basicsize = sizeof(_ped_Disk),
 /* .tp_itemsize = XXX */
    .tp_dealloc = (destructor) _ped_Disk_dealloc,
 /* .tp_print = XXX */
 /* .tp_getattr = XXX */
 /* .tp_setattr = XXX */
 /* .tp_compare = XXX */
 /* .tp_repr = XXX */
 /* .tp_as_number = XXX */
 /* .tp_as_sequence = XXX */
 /* .tp_as_mapping = XXX */
 /* .tp_hash = XXX */
 /* .tp_call = XXX */
 /* .tp_str = XXX */
    .tp_getattro = PyObject_GenericGetAttr,
 /* .tp_setattro = XXX */
 /* .tp_as_buffer = XXX */
    .tp_flags = Py_TPFLAGS_HAVE_CLASS | Py_TPFLAGS_BASETYPE |
                Py_TPFLAGS_HAVE_GC,
    .tp_doc = "PedDisk objects",
    .tp_traverse = (traverseproc) _ped_Disk_traverse,
    .tp_clear = (inquiry) _ped_Disk_clear,
 /* .tp_richcompare = XXX */
 /* .tp_weaklistoffset = XXX */
 /* .tp_iter = XXX */
 /* .tp_iternext = XXX */
    .tp_methods = _ped_Disk_methods,
    .tp_members = _ped_Disk_members,
    .tp_getset = _ped_Disk_getset,
 /* .tp_base = XXX */
 /* .tp_dict = XXX */
 /* .tp_descr_get = XXX */
 /* .tp_descr_set = XXX */
 /* .tp_dictoffset = XXX */
    .tp_init = (initproc) _ped_Disk_init,
    .tp_alloc = PyType_GenericAlloc,
    .tp_new = PyType_GenericNew,
 /* .tp_free = XXX */
 /* .tp_is_gc = XXX */
 /* .tp_bases = XXX */
 /* .tp_mro = XXX */
 /* .tp_cache = XXX */
 /* .tp_subclasses = XXX */
 /* .tp_weaklist = XXX */
 /* .tp_del = XXX */
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
    {"register", (PyCFunction) py_ped_disk_type_register, METH_VARARGS, NULL},
    {"unregister", (PyCFunction) py_ped_disk_type_unregister,
                   METH_VARARGS, NULL},
    {"get_next", (PyCFunction) py_ped_disk_type_get_next, METH_VARARGS, NULL},
    {"get", (PyCFunction) py_ped_disk_type_get, METH_VARARGS, NULL},
    {"check_feature", (PyCFunction) py_ped_disk_type_check_feature,
                      METH_VARARGS, NULL},
    {NULL}
};

void _ped_DiskType_dealloc(_ped_DiskType *self);
int _ped_DiskType_traverse(_ped_DiskType *self, visitproc visit, void *arg);
int _ped_DiskType_clear(_ped_DiskType *self);
PyObject *_ped_DiskType_get(_ped_DiskType *self, void *closure);
int _ped_DiskType_set(_ped_DiskType *self, PyObject *value, void *closure);

static PyGetSetDef _ped_DiskType_getset[] = {
    {"name", (getter) _ped_DiskType_get, (setter) _ped_DiskType_set,
             "DiskType name", "name"},
    {"features", (getter) _ped_DiskType_get, (setter) _ped_DiskType_set,
                 "DiskType features", "features"},
    {NULL}  /* Sentinel */
};

static PyTypeObject _ped_DiskType_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    .tp_name = "_ped.DiskType",
    .tp_basicsize = sizeof(_ped_DiskType),
 /* .tp_itemsize = XXX */
    .tp_dealloc = (destructor) _ped_DiskType_dealloc,
 /* .tp_print = XXX */
 /* .tp_getattr = XXX */
 /* .tp_setattr = XXX */
 /* .tp_compare = XXX */
 /* .tp_repr = XXX */
 /* .tp_as_number = XXX */
 /* .tp_as_sequence = XXX */
 /* .tp_as_mapping = XXX */
 /* .tp_hash = XXX */
 /* .tp_call = XXX */
 /* .tp_str = XXX */
    .tp_getattro = PyObject_GenericGetAttr,
 /* .tp_setattro = XXX */
 /* .tp_as_buffer = XXX */
    .tp_flags = Py_TPFLAGS_HAVE_CLASS | Py_TPFLAGS_CHECKTYPES |
                Py_TPFLAGS_HAVE_GC | Py_TPFLAGS_BASETYPE,
    .tp_doc = "PedDiskType objects",
    .tp_traverse = (traverseproc) _ped_DiskType_traverse,
    .tp_clear = (inquiry) _ped_DiskType_clear,
 /* .tp_richcompare = XXX */
 /* .tp_weaklistoffset = XXX */
 /* .tp_iter = XXX */
 /* .tp_iternext = XXX */
    .tp_methods = _ped_DiskType_methods,
    .tp_members = _ped_DiskType_members,
    .tp_getset = _ped_DiskType_getset,
 /* .tp_base = XXX */
 /* .tp_dict = XXX */
 /* .tp_descr_get = XXX */
 /* .tp_descr_set = XXX */
 /* .tp_dictoffset = XXX */
    .tp_init = NULL,
    .tp_alloc = PyType_GenericAlloc,
    .tp_new = NULL,
 /* .tp_free = XXX */
 /* .tp_is_gc = XXX */
 /* .tp_bases = XXX */
 /* .tp_mro = XXX */
 /* .tp_cache = XXX */
 /* .tp_subclasses = XXX */
 /* .tp_weaklist = XXX */
 /* .tp_del = XXX */
};

#endif /* PYDISK_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4
 */
