#
# geometry.py
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

import _ped

class Alignment(object):
    """Alignment()

       An Alignment object describes constraints on how sectors and Geometry
       objects are aligned.  Being aligned means that the sector be located at
       a specific sector multiple on a device, or that a geometry must start
       and end at sectors at those specific multiples.  Most methods on this
       object raise ArithmeticError if calculating alignments fails."""
    def __init__(self, offset, grainSize, PedAlignment=None):
        """Create a new Alignment object from the sectors offset and
           grainSize."""
        if PedAlignment:
            self.__alignment = PedAlignment
        else:
            self.__alignment = _ped.Alignment(offset, grainSize)

    offset = property(lambda s: s.__alignment.offset, lambda s, v: s.__alignment.offset("offset", v))
    grainSize = property(lambda s: s.__alignment.grainSize, lambda s, v: s.__alignment.offset("grainSize", v))

    def intersect(self, b):
        """Create and return a new Alignment that describes the intersection of
           self and alignment b.  A sector will satisfy the new alignment iff
           it satisfies both of the original alignments.  Whether a sector
           satisfies a given alignment is determined by is_aligned()."""
        return self.__alignment.intersect(b)

    def alignUp(self, geom, sector):
        """Return the closest sector to the provided sector that lies inside
           geom and satisfies the alignment constraint self.  This method
           prefers, but does not guarantee, that the result is beyond sector.
           If no such sector can be found, an ArithmeticError is raised."""
        return self.__alignment.align_up(geom, sector)

    def alignDown(self, geom, sector):
        """Return the closest sector to the provided sector that lies inside
           geom and satisfies the alignment constraint self.  This method
           prefers, but does not guarantee, that the result is below sector.
           If no such sector can be found, an ArithmeticError is raised."""
        return self.__alignment.align_down(geom, sector)

    def alignNearest(self, geom, sector):
        """Return toe closest sector to the input sector that lies inside
        geom and satisfies the alignment constraint self.  If no such sector
        can be found, an ArithmeticError is raised."""
        return self.__alignment.align_nearest(geom, sector)

    def isAligned(self, geom, sector):
        """Determine whether sector lies inside geom and satisfies the
           alignment constraint self."""
        return self.__alignment.is_aligned(geom, sector)

    def getPedAlignment(self):
        """Return the _ped.Alignment object contained in this Alignment.
           For internal module use only."""
        return self.__alignment
