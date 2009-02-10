#
# constraint.py
# Python bindings for libparted (built on top of the _ped Python module).
#
# Copyright (C) 2009 Red Hat, Inc.
#
# This copyrighted material is made available to anyone wishing to use,
# modify, copy, or redistribute it subject to the terms and conditions of
# the GNU General Public License v.2, or (at your option) any later version.
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY expressed or implied, including the implied warranties of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
# Public License for more details.  You should have received a copy of the
# GNU General Public License along with this program; if not, write to the
# Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
# 02110-1301, USA.  Any Red Hat trademarks that are incorporated in the
# source code or documentation are not subject to the GNU General Public
# License and may only be used or replicated with the express permission of
# Red Hat, Inc.
#
# Red Hat Author(s): Chris Lumens <clumens@redhat.com>
#

import parted
import _ped

class Constraint(object):
    """Constraint()

       A Constraint object describes a set of restrictions on other pyparted
       operations.  Constraints can restrict the location and alignment of the
       start and end of a partition, and its minimum and maximum size.  Most
       constraint operations can raise CreateException if creating temporary
       objects fails, or ArithmeticError if an error occurs during
       calculations."""
    def __init__(self, minGeom=None, maxGeom=None, exactGeom=None,
                 device=None, startAlign=None, endAlign=None, startRange=None,
                 endRange=None, minSize=None, maxSize=None, PedConstraint=None):
        """Create a new Constraint object.  There are many different ways to
           create a Constraint, all depending on the parameters passed to
           __init__.  If minGeom and maxGeom are supplied, the constraint will
           be created to satisfy both.  If only one of minGeom or maxGeom are
           supplied, the constraint is only guaranteed to solve the given
           paramter.  If exactGeom is given, the constraint will only be
           satisfied by the given geometry.  If device is given, any region
           on that device will satisfy the constraint.

           If none of the previously mentioned parameters are supplied, all of
           startAlign, EndAlign, startRange, endRange, minSize, and maxSize
           must be given."""
        if PedConstraint:
            self.__constraint = PedConstraint
        elif minGeom and maxGeom:
            self.__constraint = _ped.constraint_new_from_min_max(minGeom.getPedGeometry(), maxGeom.getPedGeometry())
        elif minGeom:
            self.__constraint = _ped.constraint_new_from_min(minGeom.getPedGeometry())
        elif maxGeom:
            self.__constraint = _ped.constraint_new_from_max(maxGeom.getPedGeometry())
        elif exactGeom:
            self.__constraint = _ped.constraint_exact(exactGeom.getPedGeometry())
        elif device:
            self.__constraint = _ped.constraint_any(device.getPedDevice())
        else:
            self.__constraint = _ped.Constraint(startAlign.getPedAlignment(),
                                                endAlign.getPedAlignment(),
                                                startRange.getPedGeometry(),
                                                endRange.getPedGeometry(),
                                                minSize, maxSize)

    def __eq__(self, other):
        return not self.__ne__(other)

    def __ne__(self, other):
        if hash(self) == hash(other):
            return False

        c1 = self.getPedConstraint()
        c2 = other.getPedConstraint()

        return self.minSize != other.minSize or self.maxSize != other.maxSize or c1.start_align != c2.start_align or c1.end_align != c2.end_align or c1.start_range != c2.start_range or c1.end_range != c2.end_range

    minSize = property(lambda s: s.__constraint.min_size, lambda s, v: setattr(s.__constraint, "min_size", v))
    maxSize = property(lambda s: s.__constraint.max_size, lambda s, v: setattr(s.__constraint, "max_size", v))

    def intersect(self, b):
        """Return a new constraint that is the intersection of self and the
           provided constraint b.  The returned constraint will therefore be
           more restrictive than either input as it will have to satisfy
           both."""
        return parted.Constraint(PedConstraint=self.__constraint.intersect(b.getPedConstraint()))

    def solveMax(self):
        """Return a new geometry that is the largest region satisfying self.
           There may be more than one solution, and there are no guarantees as
           to which solution will be returned."""
        return parted.Geometry(PedGeometry=self.__constraint.solve_max())

    def solveNearest(self, geom):
        """Return a new geometry that is the nearest region to geom that
           satisfies self.  This function does not guarantee any specific
           meaning of 'nearest'."""
        return parted.Geometry(PedGeometry=self.__constraint.solve_nearest(geom.getPedGeometry()))

    def isSolution(self, geom):
        """Does geom satisfy this constraint?"""
        return self.__constraint.is_solution(geom.getPedGeometry())

    def getPedConstraint(self):
        """Return the _ped.Constraint object contained in this Constraint.
           For internal module use only."""
        return self.__constraint
