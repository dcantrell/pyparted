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
    /* FIXME */
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
    /* FIXME */
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
    /* FIXME */
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
    /* FIXME */
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
    /* FIXME */
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
    /* FIXME */
    return 0;
}

/* 1:1 function mappings for disk.h in libparted */
PyObject *py_ped_disk_type_register(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_type_unregister(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_type_get_next(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_type_get(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_type_check_feature(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_probe(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_clobber(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_clobber_exclude(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_new(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_new_fresh(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_duplicate(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_destroy(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_commit(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_commit_to_dev(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_commit_to_os(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_check(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_print(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_get_primary_partition_count(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_get_last_partition_num(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_get_max_primary_partition_count(PyObject *s,
                                                      PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_partition_new(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_partition_destroy(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_partition_is_active(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_partition_set_flag(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_partition_get_flag(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_partition_is_flag_available(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_partition_set_system(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_partition_set_name(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_partition_get_name(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_partition_is_busy(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_partition_get_path(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_partition_type_get_name(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_partition_flag_get_name(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_partition_flag_get_by_name(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_partition_flag_next(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_add_partition(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_remove_partition(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_delete_partition(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_delete_all(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_set_partition_geom(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_maximize_partition(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_get_max_partition_geometry(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_minimize_extended_partition(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_next_partition(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_get_partition(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_get_partition_by_sector(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_extended_partition(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}
