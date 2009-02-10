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
#                    David Cantrell <dcantrell@redhat.com>
#

import math
import _ped
import parted

class Geometry(object):
    """Geometry()

       Geometry represents a region on a device in the system - a disk or
       partition.  It is expressed in terms of a starting sector and a length.
       Many methods (read and write methods in particular) throughout pyparted
       take in a Geometry object as an argument."""
    def __init__(self, device=None, start=None, length=None, end=None,
                 PedGeometry=None):
        """Create a new Geometry object for the given _ped.Device that extends
           for length sectors from the start sector.  Optionally, an end sector
           can also be provided."""
        if PedGeometry:
            self.__geometry = PedGeometry
        elif not end:
            self.__geometry = _ped.Geometry(device.getPedDevice(), start, length)
        elif not length and (end > start):
            self.__geometry = _ped.Geometry(device.getPedDevice(), start, (end - start), end)
        elif start and length and end and (end > start):
            self.__geometry = _ped.Geometry(device.getPedDevice(), start, length, end=end)
        else:
            raise parted.GeometryException, "must specify PedGeometry or (device, start, length) or (device, start, end) or (device, start, length, end)"

    def __readOnly(self, property):
        raise parted.ReadOnlyProperty, property

    start = property(lambda s: s.__geometry.start, lambda s, v: s.__geometry.set_start(v))
    end = property(lambda s: s.__geometry.end, lambda s, v: s.__geometry.set_end(v))
    length = property(lambda s: s.__geometry.length, lambda s, v: s.__geometry.set(s.__geometry.start, v))
    device = property(lambda s: parted.Device(PedDevice=s.__geometry.dev), lambda s, v: s.__readOnly("device"))

    # FIXME:  Get rid of buf and size from the public API here.
    def check(self, buf, size, offset, granularity, count, timer=None):
        """Check the region described by self for errors on the disk.
           buffer -- A temporary storage space used internally by check.  Do
                     not rely on its contents.
           size -- The size of buffer in sectors.
           offset -- The beginning of the region to check, in sectors from the
                     start of the geometry.
           granularity -- How sectors should be grouped together
           count -- How many sectors from the region to check."""
        if not timer:
            return self.__geometry.check(buf, size, offset, granularity, count)
        else:
            return self.__geometry.check(buf, size, offset, granularity, count,
                                         timer)

    def contains(self, b):
        """Return whether Geometry b is contained entirely within self and on
           the same physical device."""
        return self.__geometry.test_inside(b.getPedGeometry())

    def containsSector(self, sector):
        """Return whether the sectory is contained entirely within self."""
        return self.__geometry.test_sector_inside(sector)

    def equal(self, b):
        """Return whether self and Geometry b are on the same device and
           describe the same region."""
        return self.__geometry.equal(b.getPedGeometry())

    def getSize(self, unit="MB"):
        """Return the size of the geometry in the unit specified.  The unit
           is given as a string corresponding to one of the following
           abbreviations:  b (bytes), KB (kilobytes), MB (megabytes), GB
           (gigabytes), TB (terabytes).  An invalid unit string will raise a
           SyntaxError exception.  The default unit is MB."""
        lunit = unit.lower()
        physicalSectorSize = self.device.physicalSectorSize
        size = self.length * physicalSectorSize

        if lunit not in parted._exponent.keys():
            raise SyntaxError, "invalid unit %s given" % (unit,)

        return (size / math.pow(1024.0, parted._exponent[lunit]))

    def intersect(self, b):
        """Return a new Geometry describing the region common to both self
           and Geometry b.  Raises ArithmeticError if the regions do not
           intersect."""
        return Geometry(PedGeometry=self.__geometry.intersect(b.getPedGeometry()))

    def map(self, src, sector):
        """Given a Geometry src that overlaps with self and a sector inside src,
           this method translates the address of the sector into an address
           inside self.  If self does not contain sector, ArithmeticError will
           be raised."""
        return parted.Geometry(PedGeometry=self.__geometry.map(src.getPedGeometry(), sector))

    def overlapsWith(self, b):
        """Return whether self and b are on the same device and share at least
           some of the same region."""
        try:
            self.__geometry.intersect(b.getPedGeometry())
            return True
        except ArithmeticError:
            return False

    def read(self, offset, count):
        """Read data from the region described by self.
           offset -- The number of sectors from the beginning of the region
                     (not the beginning of the disk) to read.
           count  -- The number of sectors to read."""
        return self.__geometry.read(offset, count)

    def sync(self, fast=False):
        """Flushes all caches on the device described by self.  If fast is
           True, the flush will be quicked by cache coherency is not
           guaranteed."""
        if fast:
            return self.__geometry.sync_fast()
        else:
            return self.__geometry.sync()

    def write(self, buf, offset, count):
        """Write data into the region described by self.
           buf    -- The data to be written.
           offset -- Where to start writing to region, expressed as the number
                     of sectors from the start of the region (not the disk).
           count  -- How many sectors of buf to write out."""
        return self.__geometry.write(buf, offset, count)

    def getPedGeometry(self):
        """Return the _ped.Geometry object contained in this Geometry.
           For internal module use only."""
        return self.__geometry
