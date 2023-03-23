/*
 * pyconstraint.h
 * pyparted docstrings for pyconstraint.c
 *
 * Copyright The pyparted Project Authors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef DOCSTRINGS_PYCONSTRAINT_H_INCLUDED
#define DOCSTRINGS_PYCONSTRAINT_H_INCLUDED

#include <Python.h>

PyDoc_STRVAR(constraint_duplicate_doc,
"duplicate(Constraint) -> Constraint\n\n"
"Return a new Constraint that is a copy of the given Constraint.");

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

PyDoc_STRVAR(_ped_Constraint_doc,
"A _ped.Constraint object describes a set of restrictions on other pyparted\n"
"operations.  Constraints can restrict the location and alignment of the start\n"
"and end of a partition, and its minimum and maximum size.  Constraint\n"
"operations include various methods of creating constraints, intersecting,\n"
"and solving sets of constraints.\n\n"
"Most constraint operations can raise _ped.CreateException if creating\n"
"temporary objects fails, or ArithmeticError if an error occurrs during\n"
"calculations.");

#endif /* DOCSTRINGS_PYCONSTRAINT_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4
 */
