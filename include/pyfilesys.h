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

PyDoc_STRVAR(file_system_type_register_doc,
"register(self) -> None\n\n");

PyDoc_STRVAR(file_system_type_unregister_doc,
"unregister(self) -> None\n\n");

PyDoc_STRVAR(file_system_type_get_doc,
"get(self, string) -> _ped.FileSystemType\n\n"
"Get a FileSystemType by its name, or raise _ped.UnknownTypeException if no\n"
"type by that name exists.");

PyDoc_STRVAR(file_system_type_get_next_doc,
"get_next(self) -> _ped.FileSystemType\n\n"
"Get the next FileSystemType in parted's list after self, or raise IndexError\n"
"if there are no more types.");

PyDoc_STRVAR(file_system_clobber_doc,
"clobber(self) -> boolean\n\n"
"This method erases any file system signatures found in the region given by\n"
"self.geom, effectively removing the file system from the partition.  After\n"
"calling this method, _ped.file_system_probe() won't detect any filesystem.\n"
"This method is called by self.create() before creating a new filesystem.\n"
"Raises _ped.IOException on any internal parted errors or\n"
"_ped.FileSystemException if no filesystem exists in self.geom");

PyDoc_STRVAR(file_system_open_doc,
"open(self) -> _ped.FileSystem\n\n"
"Open and return the file system in the region given by self.geom, if one\n"
"exists.  If no file system is found, _ped.FileSystemException is raised.\n"
"For all other error conditions, _ped.IOException is raised.  This method is\n"
"not necessarily implemented for all filesystem types parted understands.");

PyDoc_STRVAR(file_system_create_doc,
"create(self, timer=None) -> _ped.FileSystem\n\n"
"Initialize a new filesystem of type self.type on the region given by\n"
"self.geom and return that new filesystem.  If the filesystem cannot be\n"
"created, a _ped.FileSystemException is rasied.  For all other error\n"
"conditions, _ped.IOException is raised.  This method is not necesssarily\n"
"implemented for all filesystem types parted understands.");

PyDoc_STRVAR(file_system_close_doc,
"close(self) -> boolean\n\n"
"Close the filesystem, raising _ped.FileSystemException on error.");

PyDoc_STRVAR(file_system_check_doc,
"check(self, timer=None) -> boolean\n\n"
"Check the filesystem for errors, returning False if any are found.  This\n"
"method is not necessarily implemented for all filesystem types parted\n"
"understands.");

PyDoc_STRVAR(file_system_copy_doc,
"copy(self, Geometry, timer=None) -> _ped.FileSystem\n\n"
"Create and return a new filesystem of the same type on the region given by\n"
"Geometry, and copy the contents of the existing filesystem into the new\n"
"one.  If an error occurrs creating or copying the new filesystem,\n"
"_ped.FileSystemException is raised.  This method is not necessarily\n"
"implemented for all filesystem types parted understands\n");

PyDoc_STRVAR(file_system_resize_doc,
"resize(self, Geometry, timer=None) -> boolean\n\n"
"Resize self to the new region described by Geometry.  It is highly\n"
"recommended that Geometry satisfy self.get_resize_constraint(), though\n"
"parted does not enforce this recommendation.  In this case, the resize\n"
"operation will most likely fail.  On error, _ped.FileSystemException is\n"
"raised.  This method is not necessarily implemented for all filesystem\n"
"types parted understands.");

PyDoc_STRVAR(file_system_get_create_constraint_doc,
"get_create_constraint(self, Device) -> Constraint\n\n"
"Return a constraint that all filesystems of type self.type that are created\n"
"on Device must satisfy.  This includes restrictions on the minimum or\n"
"maximum size of a given filesystem type, or where it must be created.");

PyDoc_STRVAR(file_system_get_resize_constraint_doc,
"get_resize_constraint(self) -> Constraint\n\n"
"Return a constraint that represents all possible ways self can be resized\n"
"with self.resize().  This takes into account the amount of space already\n"
"in use on the filesystem.");

PyDoc_STRVAR(file_system_get_copy_constraint_doc,
"get_copy_constraint(self, Device) -> Constraint\n\n"
"Return a constraint on copying self to somewhere on Device using\n"
"self.copy()");

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
                 METH_VARARGS, file_system_type_register_doc},
    {"unregister", (PyCFunction) py_ped_file_system_type_unregister,
                   METH_VARARGS, file_system_type_unregister_doc},
    {"get", (PyCFunction) py_ped_file_system_type_get, METH_VARARGS,
             file_system_type_get_doc},
    {"get_next", (PyCFunction) py_ped_file_system_type_get_next,
                 METH_VARARGS, file_system_type_get_next_doc},
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
             "The name of the FileSystemType.", "name"},
    {NULL}  /* Sentinel */
};

PyDoc_STRVAR(_ped_FileSystemType_doc,
"A _ped.FileSystemType object gives a name to a single filesystem that parted\n"
"understands.  parted maintains a list of these objects which can be\n"
"traversed with the self.get_next method or accessed directly via self.get().\n"
"File systems types are typically registered and unregistered automatically,\n"
"so the user does not need to worry about doing it.");

static PyTypeObject _ped_FileSystemType_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    .tp_name = "_ped.FileSystemType",
    .tp_basicsize = sizeof(_ped_FileSystemType),
 /* .tp_itemsize = XXX */
    .tp_dealloc = (destructor) _ped_FileSystemType_dealloc,
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
 /* .tp_getattro = XXX */
 /* .tp_setattro = XXX */
 /* .tp_as_buffer = XXX */
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_CHECKTYPES |
                Py_TPFLAGS_BASETYPE,
    .tp_doc = _ped_FileSystemType_doc,
 /* .tp_traverse = XXX */
 /* .tp_clear = XXX */
 /* .tp_richcompare = XXX */
 /* .tp_weaklistoffset = XXX */
 /* .tp_iter = XXX */
 /* .tp_iternext = XXX */
    .tp_methods = _ped_FileSystemType_methods,
    .tp_members = _ped_FileSystemType_members,
    .tp_getset = _ped_FileSystemType_getset,
 /* .tp_base = XXX */
 /* .tp_dict = XXX */
 /* .tp_descr_get = XXX */
 /* .tp_descr_set = XXX */
 /* .tp_dictoffset = XXX */
    .tp_init = (initproc) _ped_FileSystemType_init,
 /* .tp_alloc = XXX */
    .tp_new = _ped_FileSystemType_new,
 /* .tp_free = XXX */
 /* .tp_is_gc = XXX */
 /* .tp_bases = XXX */
 /* .tp_mro = XXX */
 /* .tp_cache = XXX */
 /* .tp_subclasses = XXX */
 /* .tp_weaklist = XXX */
 /* .tp_del = XXX */
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
    {"type", T_OBJECT, offsetof(_ped_FileSystem, type), 0,
             "A _ped.FileSystemType object describing the filesystem on self.geom."},
    {"geom", T_OBJECT, offsetof(_ped_FileSystem, geom), 0,
             "The on-disk region where this FileSystem object exists."},
    {NULL}
};

static PyMethodDef _ped_FileSystem_methods[] = {
    {"clobber", (PyCFunction) py_ped_file_system_clobber, METH_VARARGS,
                file_system_clobber_doc},
    {"open", (PyCFunction) py_ped_file_system_open, METH_VARARGS,
             file_system_open_doc},
    {"create", (PyCFunction) py_ped_file_system_create, METH_VARARGS,
               file_system_create_doc},
    {"close", (PyCFunction) py_ped_file_system_close, METH_VARARGS,
              file_system_close_doc},
    {"check", (PyCFunction) py_ped_file_system_check, METH_VARARGS,
              file_system_check_doc},
    {"copy", (PyCFunction) py_ped_file_system_copy, METH_VARARGS,
             file_system_copy_doc},
    {"resize", (PyCFunction) py_ped_file_system_resize, METH_VARARGS,
               file_system_resize_doc},
    {"get_create_constraint", (PyCFunction)
                              py_ped_file_system_get_create_constraint,
                              METH_VARARGS, file_system_get_create_constraint_doc},
    {"get_resize_constraint", (PyCFunction)
                              py_ped_file_system_get_resize_constraint,
                              METH_VARARGS, file_system_get_resize_constraint_doc},
    {"get_copy_constraint", (PyCFunction)
                            py_ped_file_system_get_copy_constraint,
                            METH_VARARGS, file_system_get_copy_constraint_doc},
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
                "Has the filesystem been checked prior to calling copy or resize?",
                "checked"},
    {NULL}  /* Sentinel */
};

PyDoc_STRVAR(_ped_FileSystem_doc,
"A _ped.FileSystem object describes a filesystem that exists in a given\n"
"region on a device.  The region is given by a _ped.Geometry object, and\n"
"the filesystem is further described by a _ped.FileSystemType object.\n\n"
"It is recommended that self.check() be called before any of the create,\n"
"resize, or copy operations are called.\n\n"
"Filesystem operations are especially prone to failures, and pyparted raises\n"
"a variety of exceptions when error conditions are encountered.  The most\n"
"common is _ped.FileSystemException, though _ped.IOException and\n"
"_ped.CreateException may also be raised.\n\n"
"parted knows about a variety of filesystems, but supports them to varying\n"
"degrees.  For some filesystems, it supports the full range of reading,\n"
"copying, resizing, and checking operations.  Other filesystems may only\n"
"support reading but no write operations, or all operations but resize.\n"
"If an operation is not supported, a NotImplementedError will be raised.");

static PyTypeObject _ped_FileSystem_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    .tp_name = "_ped.FileSystem",
    .tp_basicsize = sizeof(_ped_FileSystem),
 /* .tp_itemsize = XXX */
    .tp_dealloc = (destructor) _ped_FileSystem_dealloc,
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
 /* .tp_getattro = XXX */
 /* .tp_setattro = XXX */
 /* .tp_as_buffer = XXX */
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_CHECKTYPES |
                Py_TPFLAGS_BASETYPE,
    .tp_doc = _ped_FileSystem_doc,
 /* .tp_traverse = XXX */
 /* .tp_clear = XXX */
 /* .tp_richcompare = XXX */
 /* .tp_weaklistoffset = XXX */
 /* .tp_iter = XXX */
 /* .tp_iternext = XXX */
    .tp_methods = _ped_FileSystem_methods,
    .tp_members = _ped_FileSystem_members,
    .tp_getset = _ped_FileSystem_getset,
 /* .tp_base = XXX */
 /* .tp_dict = XXX */
 /* .tp_descr_get = XXX */
 /* .tp_descr_set = XXX */
 /* .tp_dictoffset = XXX */
    .tp_init = (initproc) _ped_FileSystem_init,
 /* .tp_alloc = XXX */
    .tp_new = _ped_FileSystem_new,
 /* .tp_free = XXX */
 /* .tp_is_gc = XXX */
 /* .tp_bases = XXX */
 /* .tp_mro = XXX */
 /* .tp_cache = XXX */
 /* .tp_subclasses = XXX */
 /* .tp_weaklist = XXX */
 /* .tp_del = XXX */
};

#endif /* PYFILESYS_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4
 */
