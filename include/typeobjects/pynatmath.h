/*
 * pynatmath.h
 * pyparted type objects for pynatmath.c
 *
 * Copyright The pyparted Project Authors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef TYPEOBJECTS_PYNATMATH_H_INCLUDED
#define TYPEOBJECTS_PYNATMATH_H_INCLUDED

#include <Python.h>
#include <structmember.h>

/* _ped.Alignment type object */
static PyMemberDef _ped_Alignment_members[] = {
    {NULL}
};

static PyMethodDef _ped_Alignment_methods[] = {
    {"duplicate", (PyCFunction) py_ped_alignment_duplicate, METH_VARARGS,
                  alignment_duplicate_doc},
    {"intersect", (PyCFunction) py_ped_alignment_intersect, METH_VARARGS,
                  alignment_intersect_doc},
    {"align_up", (PyCFunction) py_ped_alignment_align_up, METH_VARARGS,
                 alignment_align_up_doc},
    {"align_down", (PyCFunction) py_ped_alignment_align_down,
                   METH_VARARGS, alignment_align_down_doc},
    {"align_nearest", (PyCFunction) py_ped_alignment_align_nearest,
                      METH_VARARGS, alignment_align_nearest_doc},
    {"is_aligned", (PyCFunction) py_ped_alignment_is_aligned,
                   METH_VARARGS, alignment_is_aligned_doc},
    {NULL}
};

static PyGetSetDef _ped_Alignment_getset[] = {
    {"offset", (getter) _ped_Alignment_get,
               (setter) _ped_Alignment_set,
               "Offset in sectors from the start of a _ped.Geometry.", "offset"},
    {"grain_size", (getter) _ped_Alignment_get,
                   (setter) _ped_Alignment_set,
                   "Alignment grain_size", "grain_size"},
    {NULL}  /* Sentinel */
};

PyTypeObject _ped_Alignment_Type_obj = {
    PyVarObject_HEAD_INIT(&PyType_Type,0)
    .tp_name = "_ped.Alignment",
    .tp_basicsize = sizeof(_ped_Alignment),
 /* .tp_itemsize = XXX */
    .tp_dealloc = (destructor) _ped_Alignment_dealloc,
 /* .tp_getattr = XXX */
 /* .tp_setattr = XXX */
 /* .tp_repr = XXX */
 /* .tp_as_number = XXX */
 /* .tp_as_sequence = XXX */
 /* .tp_as_mapping = XXX */
    .tp_hash = PyObject_HashNotImplemented,
    .tp_call = NULL,
    .tp_str = (reprfunc) _ped_Alignment_str,
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_setattro = PyObject_GenericSetAttr,
 /* .tp_as_buffer = XXX */
    .tp_flags = TP_FLAGS,
    .tp_doc = _ped_Alignment_doc,
    .tp_traverse = (traverseproc) _ped_Alignment_traverse,
    .tp_clear = (inquiry) _ped_Alignment_clear,
    .tp_richcompare = (richcmpfunc) _ped_Alignment_richcompare,
 /* .tp_weaklistoffset = XXX */
 /* .tp_iter = XXX */
 /* .tp_iternext = XXX */
    .tp_methods = _ped_Alignment_methods,
    .tp_members = _ped_Alignment_members,
    .tp_getset = _ped_Alignment_getset,
    .tp_base = NULL,
    .tp_dict = NULL,
 /* .tp_descr_get = XXX */
 /* .tp_descr_set = XXX */
 /* .tp_dictoffset = XXX */
    .tp_init = (initproc) _ped_Alignment_init,
    .tp_alloc = PyType_GenericAlloc,
    .tp_new = PyType_GenericNew,
 /* .tp_free = XXX */
 /* .tp_is_gc = XXX */
    .tp_bases = NULL,
 /* .tp_del = XXX */
};

#endif /* TYPEOBJECTS_PYNATMATH_H_INCLUDED */
