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
 * Red Hat Author(s): David Cantrell
 */

#include <Python.h>

#include "pydisk.h"

/* _ped.PartitionType functions */
void _ped_PartitionType_dealloc(_ped_PartitionType *self) {
    self->ob_type->tp_free((PyObject *) self);
}

PyObject *_ped_PartitionType_new(PyTypeObject *type, PyObject *args,
                                 PyObject *kwds) {
    _ped_PartitionType *self;

    self = (_ped_PartitionType *) type->tp_alloc(type, 0);
    return (PyObject *) self;
}

int _ped_PartitionType_init(_ped_PartitionType *self, PyObject *args,
                            PyObject *kwds) {
    /* FIXME */
    return 0;
}

/* _ped.PartitionFlag functions */
void _ped_PartitionFlag_dealloc(_ped_PartitionFlag *self) {
    self->ob_type->tp_free((PyObject *) self);
}

PyObject *_ped_PartitionFlag_new(PyTypeObject *type, PyObject *args,
                                 PyObject *kwds) {
    _ped_PartitionFlag *self;

    self = (_ped_PartitionFlag *) type->tp_alloc(type, 0);
    return (PyObject *) self;
}

int _ped_PartitionFlag_init(_ped_PartitionFlag *self, PyObject *args,
                            PyObject *kwds) {
    /* FIXME */
    return 0;
}

/* _ped.Partition functions */
void _ped_Partition_dealloc(_ped_Partition *self) {
    self->ob_type->tp_free((PyObject *) self);
}

PyObject *_ped_Partition_new(PyTypeObject *type, PyObject *args,
                             PyObject *kwds) {
    _ped_Partition *self;

    self = (_ped_Partition *) type->tp_alloc(type, 0);
    return (PyObject *) self;
}

int _ped_Partition_init(_ped_Partition *self, PyObject *args, PyObject *kwds) {
    /* FIXME */
    return 0;
}

/* _ped.Disk functions */
void _ped_Disk_dealloc(_ped_Disk *self) {
    self->ob_type->tp_free((PyObject *) self);
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
    self->ob_type->tp_free((PyObject *) self);
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
    self->ob_type->tp_free((PyObject *) self);
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
