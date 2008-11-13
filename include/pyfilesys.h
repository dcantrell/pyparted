/*
 * pyfilesys.h
 * pyparted type definitions for pyfilesys.c
 *
 * Copyright (C) 2007  Red Hat, Inc.
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

#ifndef PYFILESYS_H_INCLUDED
#define PYFILESYS_H_INCLUDED

#include <Python.h>
#include <structmember.h>

#include <parted/parted.h>

/* These functions need to be added to libparted.  Remove when that's done. */
#define ped_file_system_destroy(fs)

/* 1:1 function mappings for filesys.h in libparted */
PyObject *py_ped_file_system_type_register(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_type_unregister(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_type_get(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_type_get_next(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_probe_specific(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_probe(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_clobber(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_open(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_create(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_close(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_check(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_copy(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_resize(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_get_create_constraint(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_get_resize_constraint(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_get_copy_constraint(PyObject *s, PyObject *args);

/* _ped.FileSystemType type is the Python equivalent of PedFileSystemType
 * in libparted */
typedef struct {
    PyObject_HEAD

    /* PedFileSystemType members */
    char *name;
} _ped_FileSystemType;

static PyMemberDef _ped_FileSystemType_members[] = {
    {NULL}
};

static PyMethodDef _ped_FileSystemType_methods[] = {
    {"register", (PyCFunction) py_ped_file_system_type_register,
                 METH_VARARGS, NULL},
    {"unregister", (PyCFunction) py_ped_file_system_type_unregister,
                   METH_VARARGS, NULL},
    {"get", (PyCFunction) py_ped_file_system_type_get, METH_VARARGS, NULL},
    {"get_next", (PyCFunction) py_ped_file_system_type_get_next,
                 METH_VARARGS, NULL},
    {NULL}
};

void _ped_FileSystemType_dealloc(_ped_FileSystemType *self);
PyObject *_ped_FileSystemType_new(PyTypeObject *type, PyObject *args,
                                  PyObject *kwds);
int _ped_FileSystemType_init(_ped_FileSystemType *self, PyObject *args,
                             PyObject *kwds);
PyObject *_ped_FileSystemType_get(_ped_FileSystemType *self, void *closure);
int _ped_FileSystemType_set(_ped_FileSystemType *self, PyObject *value,
                            void *closure);

static PyGetSetDef _ped_FileSystemType_getset[] = {
    {"name", (getter) _ped_FileSystemType_get,
             (setter) _ped_FileSystemType_set,
             "FileSystemType name", "name"},
    {NULL}  /* Sentinel */
};

static PyTypeObject _ped_FileSystemType_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    .tp_name = "_ped.FileSystemType",
    .tp_basicsize = sizeof(_ped_FileSystemType),
    .tp_dealloc = (destructor) _ped_FileSystemType_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_CHECKTYPES |
                Py_TPFLAGS_BASETYPE,
    .tp_doc = "PedFileSystemType objects",
    .tp_methods = _ped_FileSystemType_methods,
    .tp_members = _ped_FileSystemType_members,
    .tp_getset = _ped_FileSystemType_getset,
    .tp_init = (initproc) _ped_FileSystemType_init,
    .tp_new = _ped_FileSystemType_new,
};

/* _ped.FileSystem type is the Python equiv of PedFileSystem in libparted */
typedef struct {
    PyObject_HEAD

    /* PedFileSystem members */
    PyObject *type;              /* _ped.FileSystemType */
    PyObject *geom;              /* _ped.Geometry */
    int checked;
} _ped_FileSystem;

static PyMemberDef _ped_FileSystem_members[] = {
    {"type", T_OBJECT, offsetof(_ped_FileSystem, type), 0, NULL},
    {"geom", T_OBJECT, offsetof(_ped_FileSystem, geom), 0, NULL},
    {NULL}
};

static PyMethodDef _ped_FileSystem_methods[] = {
    {"probe_specific", (PyCFunction) py_ped_file_system_probe_specific,
                       METH_VARARGS, NULL},
    {"probe", (PyCFunction) py_ped_file_system_probe, METH_VARARGS, NULL},
    {"clobber", (PyCFunction) py_ped_file_system_clobber, METH_VARARGS, NULL},
    {"open", (PyCFunction) py_ped_file_system_open, METH_VARARGS, NULL},
    {"create", (PyCFunction) py_ped_file_system_create, METH_VARARGS, NULL},
    {"close", (PyCFunction) py_ped_file_system_close, METH_VARARGS, NULL},
    {"check", (PyCFunction) py_ped_file_system_check, METH_VARARGS, NULL},
    {"copy", (PyCFunction) py_ped_file_system_copy, METH_VARARGS, NULL},
    {"resize", (PyCFunction) py_ped_file_system_resize, METH_VARARGS, NULL},
    {"get_create_constraint", (PyCFunction)
                              py_ped_file_system_get_create_constraint,
                              METH_VARARGS, NULL},
    {"get_resize_constraint", (PyCFunction)
                              py_ped_file_system_get_resize_constraint,
                              METH_VARARGS, NULL},
    {"get_copy_constraint", (PyCFunction)
                            py_ped_file_system_get_copy_constraint,
                            METH_VARARGS, NULL},
    {NULL}
};

void _ped_FileSystem_dealloc(_ped_FileSystem *self);
PyObject *_ped_FileSystem_new(PyTypeObject *type, PyObject *args,
                              PyObject *kwds);
int _ped_FileSystem_init(_ped_FileSystem *self, PyObject *args, PyObject *kwds);
PyObject *_ped_FileSystem_get(_ped_FileSystem *self, void *closure);
int _ped_FileSystem_set(_ped_FileSystem *self, PyObject *value, void *closure);

static PyGetSetDef _ped_FileSystem_getset[] = {
    {"checked", (getter) _ped_FileSystem_get,
                (setter) _ped_FileSystem_set,
                "FileSystem checked", "checked"},
    {NULL}  /* Sentinel */
};

static PyTypeObject _ped_FileSystem_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    .tp_name = "_ped.FileSystem",
    .tp_basicsize = sizeof(_ped_FileSystem),
    .tp_dealloc = (destructor) _ped_FileSystem_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_CHECKTYPES |
                Py_TPFLAGS_BASETYPE,
    .tp_doc = "PedFileSystem objects",
    .tp_methods = _ped_FileSystem_methods,
    .tp_members = _ped_FileSystem_members,
    .tp_getset = _ped_FileSystem_getset,
    .tp_init = (initproc) _ped_FileSystem_init,
    .tp_new = _ped_FileSystem_new,
};

#endif /* PYFILESYS_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4 */
