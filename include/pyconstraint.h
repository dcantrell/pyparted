/*
 * pyconstraint.h
 * pyparted type definitions for pyconstraint.c
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

#ifndef PYCONSTRAINT_H_INCLUDED
#define PYCONSTRAINT_H_INCLUDED

#include <Python.h>
#include <structmember.h>

#include <parted/parted.h>

/* _ped.Constraint type is the Python equiv of PedConstraint in libparted */
typedef struct {
    PyObject_HEAD

    /* PedConstraint members */
    PyObject *start_align;              /* _ped.Alignment */
    PyObject *end_align;                /* _ped.Alignment */
    PyObject *start_range;              /* _ped.Geometry */
    PyObject *end_range;                /* _ped.Geometry */
    PyObject *min_size;                 /* _ped.Sector */
    PyObject *max_size;                 /* _ped.Sector */
} _ped_Constraint;

static PyMemberDef _ped_Constraint_members[] = {
    {"start_align", T_OBJECT, offsetof(_ped_Constraint, start_align), 0, NULL},
    {"end_align", T_OBJECT, offsetof(_ped_Constraint, end_align), 0, NULL},
    {"start_range", T_OBJECT, offsetof(_ped_Constraint, start_range), 0, NULL},
    {"end_range", T_OBJECT, offsetof(_ped_Constraint, end_range), 0, NULL},
    {"min_size", T_OBJECT, offsetof(_ped_Constraint, min_size), 0, NULL},
    {"max_size", T_OBJECT, offsetof(_ped_Constraint, max_size), 0, NULL},
    {NULL}
};

static PyMethodDef _ped_Constraint_methods[] = {
    {NULL}
};

void _ped_Constraint_dealloc(_ped_Constraint *self);
PyObject *_ped_Constraint_new(PyTypeObject *type, PyObject *args,
                              PyObject *kwds);
int _ped_Constraint_init(_ped_Constraint *self, PyObject *args, PyObject *kwds);

static PyGetSetDef _ped_Constraint_getset[] = {
    {NULL}  /* Sentinel */
};

static PyTypeObject _ped_Constraint_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    0,                                         /* ob_size */
    "_ped.Constraint",                         /* tp_name */
    sizeof(_ped_Constraint),                   /* tp_basicsize */
    0,                                         /* tp_itemsize */
    (destructor) _ped_Constraint_dealloc,      /* tp_dealloc */
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
    "PedConstraint objects",                   /* tp_doc */
    0,                                         /* tp_traverse */
    0,                                         /* tp_clear */
    0,                                         /* tp_richcompare */
    0,                                         /* tp_weaklistoffset */
    0,                                         /* tp_iter */
    0,                                         /* tp_iternext */
    _ped_Constraint_methods,                   /* tp_methods */
    _ped_Constraint_members,                   /* tp_members */
    _ped_Constraint_getset,                    /* tp_getset */
    0,                                         /* tp_base */
    0,                                         /* tp_dict */
    0,                                         /* tp_descr_get */
    0,                                         /* tp_descr_set */
    0,                                         /* tp_dictoffset */
    (initproc) _ped_Constraint_init,           /* tp_init */
    0,                                         /* tp_alloc */
    _ped_Constraint_new,                       /* tp_new */
};

/* 1:1 function mappings for constraint.h in libparted */
PyObject *py_ped_constraint_init(PyObject *s, PyObject *args);
PyObject *py_ped_constraint_new(PyObject *s, PyObject *args);
PyObject *py_ped_constraint_new_from_min_max(PyObject *s, PyObject *args);
PyObject *py_ped_constraint_new_from_min(PyObject *s, PyObject *args);
PyObject *py_ped_constraint_new_from_max(PyObject *s, PyObject *args);
PyObject *py_ped_constraint_duplicate(PyObject *s, PyObject *args);
PyObject *py_ped_constraint_done(PyObject *s, PyObject *args);
PyObject *py_ped_constraint_destroy(PyObject *s, PyObject *args);
PyObject *py_ped_constraint_intersect(PyObject *s, PyObject *args);
PyObject *py_ped_constraint_solve_max(PyObject *s, PyObject *args);
PyObject *py_ped_constraint_solve_nearest(PyObject *s, PyObject *args);
PyObject *py_ped_constraint_is_solution(PyObject *s, PyObject *args);
PyObject *py_ped_constraint_any(PyObject *s, PyObject *args);
PyObject *py_ped_constraint_exact(PyObject *s, PyObject *args);

#endif /* PYCONSTRAINT_H_INCLUDED */
