/*
 * pyconstraint.h
 * pyparted type objects for pyconstraint.c
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

#ifndef TYPEOBJECTS_PYCONSTRAINT_H_INCLUDED
#define TYPEOBJECTS_PYCONSTRAINT_H_INCLUDED

#include <Python.h>
#include <structmember.h>

/* _ped.Constraint type object */
static PyMemberDef _ped_Constraint_members[] = {
    {"start_align", T_OBJECT, offsetof(_ped_Constraint, start_align), 0,
                    "The _ped.Alignment describing the starting alignment constraints of the partition."},
    {"end_align", T_OBJECT, offsetof(_ped_Constraint, end_align), 0,
                  "The _ped.Alignment describing the ending alignment constraints of the partition."},
    {"start_range", T_OBJECT, offsetof(_ped_Constraint, start_range), 0,
                    "The _ped.Geometry describing the minimum size constraints of the partition."},
    {"end_range", T_OBJECT, offsetof(_ped_Constraint, end_range), 0,
                  "The _ped.Geometry describing the maximum size constraints of the partition."},
    {NULL}
};

static PyMethodDef _ped_Constraint_methods[] = {
    {"duplicate", (PyCFunction) py_ped_constraint_duplicate,
                  METH_VARARGS, constraint_duplicate_doc},
    {"intersect", (PyCFunction) py_ped_constraint_intersect,
                  METH_VARARGS, constraint_intersect_doc},
    {"solve_max", (PyCFunction) py_ped_constraint_solve_max,
                  METH_VARARGS, constraint_solve_max_doc},
    {"solve_nearest", (PyCFunction) py_ped_constraint_solve_nearest,
                      METH_VARARGS, constraint_solve_nearest_doc},
    {"is_solution", (PyCFunction) py_ped_constraint_is_solution,
                    METH_VARARGS, constraint_is_solution_doc},
    {NULL}
};

static PyGetSetDef _ped_Constraint_getset[] = {
    {"min_size", (getter) _ped_Constraint_get,
                 (setter) _ped_Constraint_set,
                 "The minimum size in _ped.Sectors of the partition.", "min_size"},
    {"max_size", (getter) _ped_Constraint_get,
                 (setter) _ped_Constraint_set,
                 "The maximum size in _ped.Sectors of the partition.", "max_size"},
    {NULL}  /* Sentinel */
};

PyTypeObject _ped_Constraint_Type_obj = {
    PyVarObject_HEAD_INIT(&PyType_Type,0)
    .tp_name = "_ped.Constraint",
    .tp_basicsize = sizeof(_ped_Constraint),
 /* .tp_itemsize = XXX */
    .tp_dealloc = (destructor) _ped_Constraint_dealloc,
 /* .tp_getattr = XXX */
 /* .tp_setattr = XXX */
 /* .tp_repr = XXX */
 /* .tp_as_number = XXX */
 /* .tp_as_sequence = XXX */
 /* .tp_as_mapping = XXX */
    .tp_hash = PyObject_HashNotImplemented,
    .tp_call = NULL,
    .tp_str = (reprfunc) _ped_Constraint_str,
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_setattro = PyObject_GenericSetAttr,
 /* .tp_as_buffer = XXX */
    .tp_flags = TP_FLAGS,
    .tp_doc = _ped_Constraint_doc,
    .tp_traverse = (traverseproc) _ped_Constraint_traverse,
    .tp_clear = (inquiry) _ped_Constraint_clear,
    .tp_richcompare = (richcmpfunc) _ped_Constraint_richcompare,
 /* .tp_weaklistoffset = XXX */
 /* .tp_iter = XXX */
 /* .tp_iternext = XXX */
    .tp_methods = _ped_Constraint_methods,
    .tp_members = _ped_Constraint_members,
    .tp_getset = _ped_Constraint_getset,
    .tp_base = NULL,
    .tp_dict = NULL,
 /* .tp_descr_get = XXX */
 /* .tp_descr_set = XXX */
 /* .tp_dictoffset = XXX */
    .tp_init = (initproc) _ped_Constraint_init,
    .tp_alloc = PyType_GenericAlloc,
    .tp_new = PyType_GenericNew,
 /* .tp_free = XXX */
 /* .tp_is_gc = XXX */
    .tp_bases = NULL,
 /* .tp_del = XXX */
};

#endif /* TYPEOBJECTS_PYCONSTRAINT_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4
 */
