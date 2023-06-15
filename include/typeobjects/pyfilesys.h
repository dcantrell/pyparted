/*
 * pyfilesys.h
 * pyparted type objects for pyfilesys.c
 *
 * Copyright The pyparted Project Authors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef TYPEOBJECTS_PYFILESYS_H_INCLUDED
#define TYPEOBJECTS_PYFILESYS_H_INCLUDED

#include <Python.h>
#include <structmember.h>

/* _ped.FileSystemType type object */
static PyMemberDef _ped_FileSystemType_members[] = {
    {NULL}
};

static PyMethodDef _ped_FileSystemType_methods[] = {
    {NULL}
};

static PyGetSetDef _ped_FileSystemType_getset[] = {
    {"name", (getter) _ped_FileSystemType_get, NULL,
             "The name of the FileSystemType.", "name"},
    {NULL}  /* Sentinel */
};

PyTypeObject _ped_FileSystemType_Type_obj = {
    PyVarObject_HEAD_INIT(&PyType_Type,0)
    .tp_name = "_ped.FileSystemType",
    .tp_basicsize = sizeof(_ped_FileSystemType),
 /* .tp_itemsize = XXX */
    .tp_dealloc = (destructor) _ped_FileSystemType_dealloc,
 /* .tp_getattr = XXX */
 /* .tp_setattr = XXX */
 /* .tp_repr = XXX */
 /* .tp_as_number = XXX */
 /* .tp_as_sequence = XXX */
 /* .tp_as_mapping = XXX */
    .tp_hash = PyObject_HashNotImplemented,
    .tp_call = NULL,
    .tp_str = (reprfunc) _ped_FileSystemType_str,
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_setattro = PyObject_GenericSetAttr,
 /* .tp_as_buffer = XXX */
    .tp_flags = TP_FLAGS,
    .tp_doc = _ped_FileSystemType_doc,
    .tp_traverse = (traverseproc) _ped_FileSystemType_traverse,
    .tp_clear = (inquiry) _ped_FileSystemType_clear,
    .tp_richcompare = (richcmpfunc) _ped_FileSystemType_richcompare,
 /* .tp_weaklistoffset = XXX */
 /* .tp_iter = XXX */
 /* .tp_iternext = XXX */
    .tp_methods = _ped_FileSystemType_methods,
    .tp_members = _ped_FileSystemType_members,
    .tp_getset = _ped_FileSystemType_getset,
    .tp_base = NULL,
    .tp_dict = NULL,
 /* .tp_descr_get = XXX */
 /* .tp_descr_set = XXX */
 /* .tp_dictoffset = XXX */
    .tp_init = NULL,
    .tp_alloc = PyType_GenericAlloc,
    .tp_new = NULL,
 /* .tp_free = XXX */
 /* .tp_is_gc = XXX */
    .tp_bases = NULL,
 /* .tp_del = XXX */
};

/* _ped.FileSystem type object */
static PyMemberDef _ped_FileSystem_members[] = {
    {"type", T_OBJECT, offsetof(_ped_FileSystem, type), READONLY,
             "A _ped.FileSystemType object describing the filesystem on self.geom."},
    {"geom", T_OBJECT, offsetof(_ped_FileSystem, geom), READONLY,
             "The on-disk region where this FileSystem object exists."},
    {NULL}
};

PyTypeObject _ped_FileSystem_Type_obj = {
    PyVarObject_HEAD_INIT(&PyType_Type,0)
    .tp_name = "_ped.FileSystem",
    .tp_basicsize = sizeof(_ped_FileSystem),
 /* .tp_itemsize = XXX */
    .tp_dealloc = (destructor) _ped_FileSystem_dealloc,
 /* .tp_getattr = XXX */
 /* .tp_setattr = XXX */
 /* .tp_repr = XXX */
 /* .tp_as_number = XXX */
 /* .tp_as_sequence = XXX */
 /* .tp_as_mapping = XXX */
    .tp_hash = PyObject_HashNotImplemented,
    .tp_call = NULL,
    .tp_str = (reprfunc) _ped_FileSystem_str,
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_setattro = PyObject_GenericSetAttr,
 /* .tp_as_buffer = XXX */
    .tp_flags = TP_FLAGS,
    .tp_doc = _ped_FileSystem_doc,
    .tp_traverse = (traverseproc) _ped_FileSystem_traverse,
    .tp_clear = (inquiry) _ped_FileSystem_clear,
    .tp_richcompare = (richcmpfunc) _ped_FileSystem_richcompare,
 /* .tp_weaklistoffset = XXX */
 /* .tp_iter = XXX */
 /* .tp_iternext = XXX */
 /* .tp_methods = XXX */
    .tp_members = _ped_FileSystem_members,
 /* .tp_getset = XXX */
    .tp_base = NULL,
    .tp_dict = NULL,
 /* .tp_descr_get = XXX */
 /* .tp_descr_set = XXX */
 /* .tp_dictoffset = XXX */
    .tp_init = (initproc) _ped_FileSystem_init,
    .tp_alloc = PyType_GenericAlloc,
    .tp_new = PyType_GenericNew,
 /* .tp_free = XXX */
 /* .tp_is_gc = XXX */
    .tp_bases = NULL,
 /* .tp_del = XXX */
};

#endif /* TYPEOBJECTS_PYFILESYS_H_INCLUDED */
