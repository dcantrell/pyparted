/*
 * docstrings/pynatmath.h
 * pyparted docstrings for for pynatmath.c
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

#ifndef DOCSTRINGS_PYNATMATH_H_INCLUDED
#define DOCSTRINGS_PYNATMATH_H_INCLUDED

#include <Python.h>

PyDoc_STRVAR(alignment_duplicate_doc,
"duplicate(self) -> _ped.Alignment\n\n"
"Create an identical copy of self.  Raises _ped.CreateException if the\n"
"operation fails");

PyDoc_STRVAR(alignment_intersect_doc,
"intersect(self, Alignment) -> _ped.Alignment\n\n"
"Create a new Alignment that describes the intersection of self and\n"
"Alignment.  A sector will satisfy the new Alignment if and only if it\n"
"satisfies both of the original alignments, where 'satisfy' is determined\n"
"by is_aligned().  The proof of this is relatively complicated and is\n"
"described thoroughly in the libparted source.  This method raises\n"
"ArithmeticError if no intersection can be found.");

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

PyDoc_STRVAR(_ped_Alignment_doc,
"A _ped.Alignment object describes constraints on how sectors and Geometry\n"
"objects are aligned.  It includes a variety of methods for aligning sectors\n"
"and calculating the intersection of two Alignment objects.  Most methods on\n"
"this object can raise _ped.CreateException if creating temporary objects\n"
"fails and ArithmeticError if calculating alignments and intersections fails.");

#endif /* DOCSTRINGS_PYNATMATH_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4
 */
