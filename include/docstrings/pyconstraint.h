/*
 * pyconstraint.h
 * pyparted docstrings for pyconstraint.c
 *
 * Copyright (C) 2007, 2008 Red Hat, Inc.
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
 * Red Hat Author(s): David Cantrell <dcantrell@redhat.com>
 *                    Chris Lumens <clumens@redhat.com>
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
