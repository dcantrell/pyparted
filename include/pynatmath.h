/*
 * pynatmath.h
 * pyparted type definitions for pynatmath.c
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

#ifndef PYNATMATH_H_INCLUDED
#define PYNATMATH_H_INCLUDED

#include <Python.h>
#include <structmember.h>

#include <parted/parted.h>

/* 1:1 function mappings for natmath.h in libparted */
PyObject *py_ped_round_up_to(PyObject *s, PyObject *args);
PyObject *py_ped_round_down_to(PyObject *s, PyObject *args);
PyObject *py_ped_round_to_nearest(PyObject *s, PyObject *args);
PyObject *py_ped_greatest_common_divisor(PyObject *s, PyObject *args);
PyObject *py_ped_alignment_destroy(PyObject *s, PyObject *args);
PyObject *py_ped_alignment_duplicate(PyObject *s, PyObject *args);
PyObject *py_ped_alignment_intersect(PyObject *s, PyObject *args);
PyObject *py_ped_alignment_align_up(PyObject *s, PyObject *args);
PyObject *py_ped_alignment_align_down(PyObject *s, PyObject *args);
PyObject *py_ped_alignment_align_nearest(PyObject *s, PyObject *args);
PyObject *py_ped_alignment_is_aligned(PyObject *s, PyObject *args);
PyObject *py_ped_div_round_up(PyObject *s, PyObject *args);
PyObject *py_ped_div_round_to_nearest(PyObject *s, PyObject *args);

PyDoc_STRVAR(alignment_destroy_doc,
"destroy(self) -> None\n\n"
"Destroys the Alignment object.");

PyDoc_STRVAR(alignment_duplicate_doc,
"duplicate(self) -> _ped.Alignment\n\n"
"Create an identical copy of self.  Raises _ped.CreateException if the\n"
"operation fails");

PyDoc_STRVAR(alignment_intersect_doc,
"intersect(self, Alignment) -> _ped.Alignment\n\n"
"Create a new Alignment that describes the intersection of self and\n"
"Alignment.  A sector will satisfy the new Alignment iff it satisfies both\n"
"of the original alignments, where 'satisfy' is determined by is_aligned().\n"
"The proof of this is relatively complicated and is described thoroughly\n"
"in the libparted source.  This method raises ArithmeticError if no\n"
"intersection can be found.");

PyDoc_STRVAR(alignment_align_up_doc,
"align_up(self, Geometry, Sector) -> Sector\n\n"
"Returns the closest Sector to the input Sector that lies inside Geometry\n"
"and satisfies the alignment constraint.  This method prefers, but does not\n"
"guarantee, that the result is beyond Sector.  If no such Sector can be\n"
"found, an ArithmeticError is raised.");

PyDoc_STRVAR(alignment_align_down_doc,
"align_down(self, Geometry, Sector) -> Sector\n\n"
"Returns the closest Sector to the input Sector that lies inside Geometry\n"
"and satisfies the alignment constraint.  This method prefers, but does not\n"
"guarantee, that the result is below Sector.  If no such Sector can be\n"
"found, an ArithmeticError is raised.");

PyDoc_STRVAR(alignment_align_nearest_doc,
"align_nearest(self, Geometry, Sector) -> Sector\n\n"
"Returns the closest Sector to the input Sector that lies inside Geometry\n"
"and satisfies the aligmnent constraint.  If no such Sector can be found,\n"
"an ArithmeticError is raised.");

PyDoc_STRVAR(alignment_is_aligned_doc,
"is_aligned(self, Geometry, Sector) -> boolean\n\n"
"Returns whether or not Sector lies inside Geometry and satisfies the\n"
"alignment constraint.  This method defines what 'satisfy' means for\n"
"intersection.");

/* _ped.Alignment type is the Python equivalent of PedAlignment in libparted */
typedef struct {
    PyObject_HEAD

    /* PedAlignment members */
    long long offset;              /* PedSector */
    long long grain_size;          /* PedSector */
} _ped_Alignment;

static PyMemberDef _ped_Alignment_members[] = {
    {NULL}
};

static PyMethodDef _ped_Alignment_methods[] = {
    {"destroy", (PyCFunction) py_ped_alignment_destroy, METH_VARARGS,
                 alignment_destroy_doc},
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

void _ped_Alignment_dealloc(_ped_Alignment *self);
int _ped_Alignment_traverse(_ped_Alignment *self, visitproc visit, void *arg);
int _ped_Alignment_clear(_ped_Alignment *self);
int _ped_Alignment_init(_ped_Alignment *self, PyObject *args, PyObject *kwds);
PyObject *_ped_Alignment_get(_ped_Alignment *self, void *closure);
int _ped_Alignment_set(_ped_Alignment *self, PyObject *value, void *closure);

static PyGetSetDef _ped_Alignment_getset[] = {
    {"offset", (getter) _ped_Alignment_get,
               (setter) _ped_Alignment_set,
               "Offset in sectors from the start of a _ped.Geometry.", "offset"},
    {"grain_size", (getter) _ped_Alignment_get,
                   (setter) _ped_Alignment_set,
                   "Alignment grain_size", "grain_size"},
    {NULL}  /* Sentinel */
};

PyDoc_STRVAR(_ped_Alignment_doc,
"A _ped.Alignment object describes constraints on how sectors and Geometry\n"
"objects are aligned.  It includes a variety of methods for aligning sectors\n"
"and calculating the intersection of two Alignment objects.  Most methods on\n"
"this object can raise _ped.CreateException if creating temporary objects\n"
"fails and ArithmeticError if calculating alignments and intersections fails.");

static PyTypeObject _ped_Alignment_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    .tp_name = "_ped.Alignment",
    .tp_basicsize = sizeof(_ped_Alignment),
 /* .tp_itemsize = XXX */
    .tp_dealloc = (destructor) _ped_Alignment_dealloc,
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
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE |
                Py_TPFLAGS_HAVE_GC,
    .tp_doc = _ped_Alignment_doc,
    .tp_traverse = (traverseproc) _ped_Alignment_traverse,
    .tp_clear = (inquiry) _ped_Alignment_clear,
 /* .tp_richcompare = XXX */
 /* .tp_weaklistoffset = XXX */
 /* .tp_iter = XXX */
 /* .tp_iternext = XXX */
    .tp_methods = _ped_Alignment_methods,
    .tp_members = _ped_Alignment_members,
    .tp_getset = _ped_Alignment_getset,
 /* .tp_base = XXX */
 /* .tp_dict = XXX */
 /* .tp_descr_get = XXX */
 /* .tp_descr_set = XXX */
 /* .tp_dictoffset = XXX */
    .tp_init = (initproc) _ped_Alignment_init,
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

#endif /* PYNATMATH_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4
 */
