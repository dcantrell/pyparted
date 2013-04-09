/*
 * pyconstraint.h
 * pyparted type definitions for pyconstraint.c
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

/* vim:tw=78:ts=4:et:sw=4
 */
