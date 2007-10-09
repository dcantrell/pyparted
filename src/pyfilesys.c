/*
 * pyfilesys.c
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

#include "pyfilesys.h"

/* _ped.FileSystemType functions */
void _ped_FileSystemType_dealloc(_ped_FileSystemType *self) {
    PyObject_Del(self);
}

PyObject *_ped_FileSystemType_new(PyTypeObject *type, PyObject *args,
                                  PyObject *kwds) {
    _ped_FileSystemType *self;

    self = (_ped_FileSystemType *) type->tp_alloc(type, 0);
    return (PyObject *) self;
}

int _ped_FileSystemType_init(_ped_FileSystemType *self, PyObject *args,
                             PyObject *kwds) {
    /* FIXME */
    return 0;
}

/* _ped.FileSystem functions */
void _ped_FileSystem_dealloc(_ped_FileSystem *self) {
    PyObject_Del(self);
}

PyObject *_ped_FileSystem_new(PyTypeObject *type, PyObject *args,
                              PyObject *kwds) {
    _ped_FileSystem *self;

    self = (_ped_FileSystem *) type->tp_alloc(type, 0);
    return (PyObject *) self;
}

int _ped_FileSystem_init(_ped_FileSystem *self, PyObject *args,
                         PyObject *kwds) {
    /* FIXME */
    return 0;
}

/* 1:1 function mappings for filesys.h in libparted */
PyObject *py_ped_file_system_type_register(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_file_system_type_unregister(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_file_system_type_get(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_file_system_type_get_next(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_file_system_probe_specific(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_file_system_probe(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_file_system_clobber(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_file_system_clobber_exclude(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_file_system_open(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_file_system_create(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_file_system_close(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_file_system_check(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_file_system_copy(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_file_system_resize(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_file_system_get_create_constraint(PyObject *s,
                                                   PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_file_system_get_resize_constraint(PyObject *s,
                                                   PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_file_system_get_copy_constraint(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}
