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
# Author(s): Chris Lumens <clumens@redhat.com>
#            David Cantrell <dcantrell@redhat.com>
#            Alex Skinner <alex@lx.lc>
#

import parted
import _ped

from parted.decorators import localeC

class Constraint(object):
    """Constraint()

       A Constraint object describes a set of restrictions on other pyparted
       operations.  Constraints can restrict the location and alignment of the
       start and end of a partition, and its minimum and maximum size.  Most
       constraint operations can raise CreateException if creating temporary
       objects fails, or ArithmeticError if an error occurs during
       calculations."""
    @localeC
    def __init__(self, *args, **kwargs):
        """Create a new Constraint object.  There are many different ways to
           create a Constraint, all depending on the parameters passed to
           __init__.  If minGeom and maxGeom are supplied, the constraint will
           be created to satisfy both.  If only one of minGeom or maxGeom are
           supplied, the constraint is only guaranteed to solve the given
           parameter.  If exactGeom is given, the constraint will only be
           satisfied by the given geometry.  If device is given, any region
           on that device will satisfy the constraint.

           If none of the previously mentioned parameters are supplied, all of
           startAlign, EndAlign, startRange, endRange, minSize, and maxSize
           must be given."""
        if "PedConstraint" in kwargs:
            self.__constraint = kwargs.get("PedConstraint")
        elif "minGeom" in kwargs and "maxGeom" in kwargs:
            ming = kwargs.get("minGeom").getPedGeometry()
            maxg = kwargs.get("maxGeom").getPedGeometry()
            self.__constraint = _ped.constraint_new_from_min_max(ming, maxg)
        elif "minGeom" in kwargs:
            ming = kwargs.get("minGeom").getPedGeometry()
            self.__constraint = _ped.constraint_new_from_min(ming)
        elif "maxGeom" in kwargs:
            maxg = kwargs.get("maxGeom").getPedGeometry()
            self.__constraint = _ped.constraint_new_from_max(maxg)
        elif "exactGeom" in kwargs:
            exact = kwargs.get("exactGeom").getPedGeometry()
            self.__constraint = _ped.constraint_exact(exact)
        elif "device" in kwargs:
            dev = kwargs.get("device").getPedDevice()
            self.__constraint = _ped.constraint_any(dev)
        elif "startAlign" in kwargs and "endAlign" in kwargs and \
             "startRange" in kwargs and "endRange" in kwargs and \
             "minSize" in kwargs and "maxSize" in kwargs:
            starta = kwargs.get("startAlign").getPedAlignment()
            enda = kwargs.get("endAlign").getPedAlignment()
            startr = kwargs.get("startRange").getPedGeometry()
            endr = kwargs.get("endRange").getPedGeometry()
            mins = kwargs.get("minSize")
            maxs = kwargs.get("maxSize")
            self.__constraint = _ped.Constraint(starta, enda, startr, endr,
                                                mins, maxs)
        else:
            raise parted.ConstraintException("missing initialization parameters")

    def __eq__(self, other):
        return not self.__ne__(other)

    def __ne__(self, other):
        if hash(self) == hash(other):
            return False

        if not isinstance(self, other.__class__):
            return True

        c1 = self.getPedConstraint()
        c2 = other.getPedConstraint()

        return self.minSize != other.minSize \
                or self.maxSize != other.maxSize \
                or c1.start_align != c2.start_align \
                or c1.end_align != c2.end_align \
                or c1.start_range != c2.start_range \
                or c1.end_range != c2.end_range

    startAlign = property(
            lambda s: parted.Alignment(PedAlignment=s.__constraint.start_align),
            lambda s, v: setattr(s.__constraint, "start_align", v.getPedAlignment()))

    endAlign = property(
            lambda s: parted.Alignment(PedAlignment=s.__constraint.end_align),
            lambda s, v: setattr(s.__constraint, "end_align", v.getPedAlignment()))

    startRange = property(
            lambda s: parted.Geometry(PedGeometry=s.__constraint.start_range),
            lambda s, v: setattr(s.__constraint, "start_range", v.getPedGeometry()))

    endRange = property(
            lambda s: parted.Geometry(PedGeometry=s.__constraint.end_range),
            lambda s, v: setattr(s.__constraint, "end_range", v.getPedGeometry()))

    minSize = property(
            lambda s: s.__constraint.min_size,
            lambda s, v: setattr(s.__constraint, "min_size", v))

    maxSize = property(
            lambda s: s.__constraint.max_size,
            lambda s, v: setattr(s.__constraint, "max_size", v))

    def __str__(self):
        s = ("parted.Constraint instance --\n"
             "  startAlign: %(startAlign)r  endAlign: %(endAlign)r\n"
             "  startRange: %(startRange)r  endRange: %(endRange)r\n"
             "  minSize: %(minSize)s  maxSize: %(maxSize)s\n"
             "  PedConstraint: %(ped)r" %
             {"startAlign": self.startAlign, "endAlign": self.endAlign,
              "startRange": self.startRange, "endRange": self.endRange,
              "minSize": self.minSize, "maxSize": self.maxSize,
              "ped": self.__constraint})
        return s

    @localeC
    def intersect(self, b):
        """Return a new constraint that is the intersection of self and the
           provided constraint b.  The returned constraint will therefore be
           more restrictive than either input as it will have to satisfy
           both."""
        return parted.Constraint(PedConstraint=self.__constraint.intersect(b.getPedConstraint()))

    @localeC
    def solveMax(self):
        """Return a new geometry that is the largest region satisfying self.
           There may be more than one solution, and there are no guarantees as
           to which solution will be returned."""
        return parted.Geometry(PedGeometry=self.__constraint.solve_max())

    @localeC
    def solveNearest(self, geom):
        """Return a new geometry that is the nearest region to geom that
           satisfies self.  This function does not guarantee any specific
           meaning of 'nearest'."""
        return parted.Geometry(PedGeometry=self.__constraint.solve_nearest(geom.getPedGeometry()))

    @localeC
    def isSolution(self, geom):
        """Does geom satisfy this constraint?"""
        return self.__constraint.is_solution(geom.getPedGeometry())

    def getPedConstraint(self):
        """Return the _ped.Constraint object contained in this Constraint.
           For internal module use only."""
        return self.__constraint
