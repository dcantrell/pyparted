/*
 * pyfilesys.h
 * pyparted type definitions for pyfilesys.c
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

#ifndef PYFILESYS_H_INCLUDED
#define PYFILESYS_H_INCLUDED

#include <Python.h>
#include <structmember.h>

#include <parted/parted.h>

/* _ped.FileSystemType type is the Python equivalent of PedFileSystemType
 * in libparted */
typedef struct {
    PyObject_HEAD

    /* PedFileSystemType members */
    PyObject *next;                       /* _ped.FileSystemType */
    char *name;
    int *block_sizes;
    PedFileSystemOps *ops;
} _ped_FileSystemType;

static PyMemberDef _ped_FileSystemType_members[] = {
    {"next", T_OBJECT, offsetof(_ped_FileSystemType, next), 0, NULL},
    {"name", T_STRING_INPLACE, offsetof(_ped_FileSystemType, name), 0, NULL},
    {"block_sizes", T_INT, offsetof(_ped_FileSystemType, block_sizes), 0, NULL},
    {NULL}
};

static PyMethodDef _ped_FileSystemType_methods[] = {
    {NULL}
};

void _ped_FileSystemType_dealloc(_ped_FileSystemType *self);
PyObject *_ped_FileSystemType_new(PyTypeObject *type, PyObject *args,
                                  PyObject *kwds);
int _ped_FileSystemType_init(_ped_FileSystemType *self, PyObject *args,
                             PyObject *kwds);

static PyGetSetDef _ped_FileSystemType_getset[] = {
    {NULL}  /* Sentinel */
};

static PyTypeObject _ped_FileSystemType_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    0,                                         /* ob_size */
    "_ped.FileSystemType",                     /* tp_name */
    sizeof(_ped_FileSystemType),               /* tp_basicsize */
    0,                                         /* tp_itemsize */
    (destructor) _ped_FileSystemType_dealloc,  /* tp_dealloc */
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
    "PedFileSystemType objects",               /* tp_doc */
    0,                                         /* tp_traverse */
    0,                                         /* tp_clear */
    0,                                         /* tp_richcompare */
    0,                                         /* tp_weaklistoffset */
    0,                                         /* tp_iter */
    0,                                         /* tp_iternext */
    _ped_FileSystemType_methods,               /* tp_methods */
    _ped_FileSystemType_members,               /* tp_members */
    _ped_FileSystemType_getset,                /* tp_getset */
    0,                                         /* tp_base */
    0,                                         /* tp_dict */
    0,                                         /* tp_descr_get */
    0,                                         /* tp_descr_set */
    0,                                         /* tp_dictoffset */
    (initproc) _ped_FileSystemType_init,       /* tp_init */
    0,                                         /* tp_alloc */
    _ped_FileSystemType_new,                   /* tp_new */
};

/* _ped.FileSystem type is the Python equiv of PedFileSystem in libparted */
typedef struct {
    PyObject_HEAD

    /* PedFileSystem members */
    PyObject *type;              /* _ped.FileSystemType */
    PyObject *geom;              /* _ped.Geometry */
    int checked;
    void *type_specific;
} _ped_FileSystem;

static PyMemberDef _ped_FileSystem_members[] = {
    {"type", T_OBJECT, offsetof(_ped_FileSystem, type), 0, NULL},
    {"geom", T_OBJECT, offsetof(_ped_FileSystem, geom), 0, NULL},
    {"checked", T_INT, offsetof(_ped_FileSystem, checked), 0, NULL},
    {NULL}
};

static PyMethodDef _ped_FileSystem_methods[] = {
    {NULL}
};

void _ped_FileSystem_dealloc(_ped_FileSystem *self);
PyObject *_ped_FileSystem_new(PyTypeObject *type, PyObject *args,
                              PyObject *kwds);
int _ped_FileSystem_init(_ped_FileSystem *self, PyObject *args, PyObject *kwds);

static PyGetSetDef _ped_FileSystem_getset[] = {
    {NULL}  /* Sentinel */
};

static PyTypeObject _ped_FileSystem_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    0,                                         /* ob_size */
    "_ped.FileSystem",                         /* tp_name */
    sizeof(_ped_FileSystem),                   /* tp_basicsize */
    0,                                         /* tp_itemsize */
    (destructor) _ped_FileSystem_dealloc,      /* tp_dealloc */
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
    "PedFileSystem objects",                   /* tp_doc */
    0,                                         /* tp_traverse */
    0,                                         /* tp_clear */
    0,                                         /* tp_richcompare */
    0,                                         /* tp_weaklistoffset */
    0,                                         /* tp_iter */
    0,                                         /* tp_iternext */
    _ped_FileSystem_methods,                   /* tp_methods */
    _ped_FileSystem_members,                   /* tp_members */
    _ped_FileSystem_getset,                    /* tp_getset */
    0,                                         /* tp_base */
    0,                                         /* tp_dict */
    0,                                         /* tp_descr_get */
    0,                                         /* tp_descr_set */
    0,                                         /* tp_dictoffset */
    (initproc) _ped_FileSystem_init,           /* tp_init */
    0,                                         /* tp_alloc */
    _ped_FileSystem_new,                       /* tp_new */
};

/* 1:1 function mappings for filesys.h in libparted */
PyObject *py_ped_file_system_type_register(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_type_unregister(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_type_get(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_type_get_next(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_probe_specific(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_probe(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_clobber(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_clobber_exclude(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_open(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_create(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_close(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_check(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_copy(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_resize(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_get_create_constraint(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_get_resize_constraint(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_get_copy_constraint(PyObject *s, PyObject *args);

#endif /* PYFILESYS_H_INCLUDED */
