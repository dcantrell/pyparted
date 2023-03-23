#
# geometry.py
# Python bindings for libparted (built on top of the _ped Python module).
#
# Copyright The pyparted Project Authors
# SPDX-License-Identifier: GPL-2.0-or-later
#

import parted
import _ped

from parted.decorators import localeC


class Alignment(object):
    """Alignment()

    An Alignment object describes constraints on how sectors and Geometry
    objects are aligned.  Being aligned means that the sector be located at
    a specific sector multiple on a device, or that a geometry must start
    and end at sectors at those specific multiples.  Most methods on this
    object raise ArithmeticError if calculating alignments fails."""

    @localeC
    def __init__(self, *args, **kwargs):
        """Create a new Alignment object from the sectors offset and
        grainSize."""
        if "PedAlignment" in kwargs:
            self.__alignment = kwargs.get("PedAlignment")
        elif "offset" in kwargs and "grainSize" in kwargs:
            self.__alignment = _ped.Alignment(
                kwargs.get("offset"), kwargs.get("grainSize")
            )
        else:
            raise parted.AlignmentException(
                "no offset+grainSize or PedAlignment specified"
            )

    offset = property(
        lambda s: s.__alignment.offset, lambda s, v: setattr(s.__alignment, "offset", v)
    )
    grainSize = property(
        lambda s: s.__alignment.grain_size,
        lambda s, v: setattr(s.__alignment, "grain_size", v),
    )

    def __eq__(self, other):
        return not self.__ne__(other)

    def __ne__(self, other):
        if not isinstance(self, other.__class__):
            return True

        return self.offset != other.offset or self.grainSize != other.grainSize

    def __str__(self):
        s = (
            "parted.Alignment instance --\n"
            "  offset: %(offset)s  grainSize: %(grainSize)s\n"
            "  PedAlignment: %(ped)r"
            % {
                "offset": self.offset,
                "grainSize": self.grainSize,
                "ped": self.__alignment,
            }
        )
        return s

    @localeC
    def intersect(self, b):
        """Create and return a new Alignment that describes the intersection of
        self and alignment b.  A sector will satisfy the new alignment iff
        it satisfies both of the original alignments.  Whether a sector
        satisfies a given alignment is determined by is_aligned()."""
        return parted.Alignment(
            PedAlignment=self.__alignment.intersect(b.getPedAlignment())
        )

    @localeC
    def alignUp(self, geom, sector):
        """Return the closest sector to the provided sector that lies inside
        geom and satisfies the alignment constraint self.  This method
        prefers, but does not guarantee, that the result is beyond sector.
        If no such sector can be found, an ArithmeticError is raised."""
        return self.__alignment.align_up(geom.getPedGeometry(), sector)

    @localeC
    def alignDown(self, geom, sector):
        """Return the closest sector to the provided sector that lies inside
        geom and satisfies the alignment constraint self.  This method
        prefers, but does not guarantee, that the result is below sector.
        If no such sector can be found, an ArithmeticError is raised."""
        return self.__alignment.align_down(geom.getPedGeometry(), sector)

    @localeC
    def alignNearest(self, geom, sector):
        """Return the closest sector to the input sector that lies inside
        geom and satisfies the alignment constraint self.  If no such sector
        can be found, an ArithmeticError is raised."""
        return self.__alignment.align_nearest(geom.getPedGeometry(), sector)

    @localeC
    def isAligned(self, geom, sector):
        """Determine whether sector lies inside geom and satisfies the
        alignment constraint self."""
        if not geom:
            raise TypeError("missing parted.Geometry parameter")

        if sector is None:
            raise TypeError("missing sector parameter")

        return self.__alignment.is_aligned(geom.getPedGeometry(), sector)

    def getPedAlignment(self):
        """Return the _ped.Alignment object contained in this Alignment.
        For internal module use only."""
        return self.__alignment
