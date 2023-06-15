/*
 * pyconstraint.h
 * pyparted type definitions for pyconstraint.c
 *
 * Copyright The pyparted Project Authors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef PYCONSTRAINT_H_INCLUDED
#define PYCONSTRAINT_H_INCLUDED

#include <Python.h>

#include <parted/parted.h>

/* 1:1 function mappings for constraint.h in libparted */
PyObject *py_ped_constraint_new_from_min_max(PyObject *, PyObject *);
PyObject *py_ped_constraint_new_from_min(PyObject *, PyObject *);
PyObject *py_ped_constraint_new_from_max(PyObject *, PyObject *);
PyObject *py_ped_constraint_duplicate(PyObject *, PyObject *);
PyObject *py_ped_constraint_intersect(PyObject *, PyObject *);
PyObject *py_ped_constraint_solve_max(PyObject *, PyObject *);
PyObject *py_ped_constraint_solve_nearest(PyObject *, PyObject *);
PyObject *py_ped_constraint_is_solution(PyObject *, PyObject *);
PyObject *py_ped_constraint_any(PyObject *, PyObject *);
PyObject *py_ped_constraint_exact(PyObject *, PyObject *);

/* _ped.Constraint type is the Python equiv of PedConstraint in libparted */
typedef struct {
    PyObject_HEAD

    /* PedConstraint members */
    PyObject *start_align;              /* _ped.Alignment */
    PyObject *end_align;                /* _ped.Alignment */
    PyObject *start_range;              /* _ped.Geometry  */
    PyObject *end_range;                /* _ped.Geometry  */
    long long min_size;                 /* PedSector      */
    long long max_size;                 /* PedSector      */
} _ped_Constraint;

void _ped_Constraint_dealloc(_ped_Constraint *);
int _ped_Constraint_compare(_ped_Constraint *, PyObject *);
PyObject *_ped_Constraint_richcompare(_ped_Constraint *, PyObject *, int);
PyObject *_ped_Constraint_str(_ped_Constraint *);
int _ped_Constraint_traverse(_ped_Constraint *, visitproc, void *);
int _ped_Constraint_clear(_ped_Constraint *);
int _ped_Constraint_init(_ped_Constraint *, PyObject *, PyObject *);
PyObject *_ped_Constraint_get(_ped_Constraint *, void *);
int _ped_Constraint_set(_ped_Constraint *, PyObject *, void *);

extern PyTypeObject _ped_Constraint_Type_obj;

#endif /* PYCONSTRAINT_H_INCLUDED */
