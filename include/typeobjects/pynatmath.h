/*
 * pynatmath.h
 * pyparted type objects for pynatmath.c
 *
 * Copyright (C) 2007-2013 Red Hat, Inc.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions of
 * the GNU General Public License v.2, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY expressed or implied, including the implied warranties of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.  You should have received a copy of the
 * GNU General Public License along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.  Any Red Hat trademarks that are incorporated in the
 * source code or documentation are not subject to the GNU General Public
 * License and may only be used or replicated with the express permission of
 * Red Hat, Inc.
 *
 * Author(s): David Cantrell <dcantrell@redhat.com>
 *            Chris Lumens <clumens@redhat.com>
 *            Alex Skinner <alex@lx.lc>
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

/* vim:tw=78:ts=4:et:sw=4
 */
