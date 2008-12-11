/*
 * pyconstraint.h
 * pyparted type definitions for pyconstraint.c
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

#ifndef PYCONSTRAINT_H_INCLUDED
#define PYCONSTRAINT_H_INCLUDED

#include <Python.h>
#include <structmember.h>

#include <parted/parted.h>

/* 1:1 function mappings for constraint.h in libparted */
PyObject *py_ped_constraint_new_from_min_max(PyObject *s, PyObject *args);
PyObject *py_ped_constraint_new_from_min(PyObject *s, PyObject *args);
PyObject *py_ped_constraint_new_from_max(PyObject *s, PyObject *args);
PyObject *py_ped_constraint_duplicate(PyObject *s, PyObject *args);
PyObject *py_ped_constraint_destroy(PyObject *s, PyObject *args);
PyObject *py_ped_constraint_intersect(PyObject *s, PyObject *args);
PyObject *py_ped_constraint_solve_max(PyObject *s, PyObject *args);
PyObject *py_ped_constraint_solve_nearest(PyObject *s, PyObject *args);
PyObject *py_ped_constraint_is_solution(PyObject *s, PyObject *args);
PyObject *py_ped_constraint_any(PyObject *s, PyObject *args);
PyObject *py_ped_constraint_exact(PyObject *s, PyObject *args);

PyDoc_STRVAR(constraint_duplicate_doc,
"duplicate(Constraint) -> Constraint\n\n"
"Return a new Constraint that is a copy of the given Constraint.");

PyDoc_STRVAR(constraint_destroy_doc,
"destroy() -> None\n\n"
"Destroy this Constraint object.");

PyDoc_STRVAR(constraint_intersect_doc,
"intersect(Constraint) -> Constraint\n\n"
"Return a Constraint that requires a region to satisfy both this\n"
"Constraint object and the one passed in to the method.  Any\n"
"region satisfying both Constraints will also satisfy the returned\n"
"Constraint.");

PyDoc_STRVAR(constraint_solve_max_doc,
"solve_max() -> Constraint\n\n"
"Find the largest region that satisfies this Constraint object and\n"
"return a new Constraint.  There may be more than one solution.\n"
"There are no guarantees about which solution will be returned.\n");

PyDoc_STRVAR(constraint_solve_nearest_doc,
"solve_nearest(Geometry) -> Constraint\n\n"
"Return the nearest region to Geometry that will satisfy this\n"
"Constraint object.  This function does not guarantee what nearest\n"
"means.");

PyDoc_STRVAR(constraint_is_solution_doc,
"is_solution(Geometry) -> bool\n\n"
"Return True if Geometry satisfies this Constraint, False otherwise.");

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
    {"destroy", (PyCFunction) py_ped_constraint_destroy, METH_VARARGS,
                constraint_destroy_doc},
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

void _ped_Constraint_dealloc(_ped_Constraint *self);
int _ped_Constraint_traverse(_ped_Constraint *self, visitproc visit, void *arg);
int _ped_Constraint_clear(_ped_Constraint *self);
int _ped_Constraint_init(_ped_Constraint *self, PyObject *args, PyObject *kwds);
PyObject *_ped_Constraint_get(_ped_Constraint *self, void *closure);
int _ped_Constraint_set(_ped_Constraint *self, PyObject *value, void *closure);

static PyGetSetDef _ped_Constraint_getset[] = {
    {"min_size", (getter) _ped_Constraint_get,
                 (setter) _ped_Constraint_set,
                 "The mimimum size in _ped.Sectors of the partition.", "min_size"},
    {"max_size", (getter) _ped_Constraint_get,
                 (setter) _ped_Constraint_set,
                 "The maximum size in _ped.Sectors of the partition.", "max_size"},
    {NULL}  /* Sentinel */
};

PyDoc_STRVAR(_ped_Constraint_doc,
"A _ped.Constraint object describes a set of restrictions on other pyparted\n"
"operations.  Constraints can restrict the location and alignment of the start\n"
"and end of a partition, and its minimum and maximum size.  Constraint\n"
"operations include various methods of creating constraints, intersecting,\n"
"and solving sets of constraints.\n\n"
"Most constraint operations can raise _ped.CreateException if creating\n"
"temporary objects fails, or ArithmeticError if an error occurrs during\n"
"calculations.");

extern PyTypeObject _ped_Constraint_Type_obj;

#endif /* PYCONSTRAINT_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4
 */
