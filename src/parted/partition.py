#
# partition.py
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
import string

# XXX: add docstrings

class Partition(object):
    def __init__(self, disk=None, type=None, fs=None, geometry=None, PedPartition=None):
        if PedPartition is None:
            if disk is None:
                raise _ped.PartedException, "no disk specified"
            elif type is None:
                raise _ped.PartedException, "no type specified"
            elif geometry is None:
                raise _ped.PartedException, "no geometry specified"
            else:
                raise _ped.PartedException, "no PedPartition specified"

            self._disk = disk
            self._fileSystem = fs
            self._geometry = geometry

            if fs is None:
                self.__partition = _ped.Partition(disk, type, geometry.start, geometry.end)
            else:
                self.__partition = _ped.Partition(disk, type, geometry.start, geometry.end, fs.type)
        else:
            self.__partition = PedPartition
            self._disk = self.__partition.disk
            # XXX: self._fileSystem = XXX
            self._geometry = self.__partition.geom

    def __readOnly(self, property):
        raise parted.ReadOnlyProperty, property

    def __writeOnly(self, property):
        raise parted.WriteOnlyProperty, property

    active = property(lambda s: s.__partition.is_active(), lambda s, v: s.__readOnly("active"))
    busy = property(lambda s: s.__partition.is_busy(), lambda s, v: s.__readOnly("busy"))

    #
    # XXX: disk, fileSystem, and geometry need to set the property in
    # s.__partition as writing to the properties that we have here...maybe?
    # figure this out.
    #
    disk = property(lambda s: s._disk, lambda s, v: s.__readOnly("disk"))
    fileSystem = property(lambda s: s._fileSystem, lambda s, v: setattr(s, "_fileSystem", v))
    geometry = property(lambda s: s._geometry, lambda s, v: setattr(s, "_geometry", v))

    number = property(lambda s: s.__partition.num, lambda s, v: setattr(s.__partition, "num", v))
    path = property(lambda s: s.__partition.get_path(), lambda s, v: s.__readOnly("path"))
    system = property(lambda s: s.__writeOnly("system"), lambda s, v: s.__partition.set_system(v))
    type = property(lambda s: s.__partition.type, lambda s, v: setattr(s.__partition, "type", v))

    def getFlag(self, flag):
        """Get the value of a particular flag on the partition.  Valid flags
           are the _ped.PARTITION_* constants.  See _ped.flag_get_name() and
           _ped.flag_get_by_name() for more help working with partition flags.
        """
        return self.__partition.get_flag(flag)

    def getMaxGeometry(self, constraint):
        """Given a constraint, return the maximum Geometry that self can be
           grown to.  Raises Partitionexception on error."""
        return self.disk.get_max_partition_geometry(self.__partition, constraint)

    def setFlag(self, flag, value):
        """Set the flag on a partition to the provided value.  On error, a
           PartitionException will be raised.  See getFlag() for more help on
           working with partition flags."""
        return self.__partition.set_flag(flag, value)

    def getSize(self, unit="MB"):
        """Return the size of the partition in the unit specified.  The unit
           is given as a string corresponding to one of the following
           abbreviations:  b (bytes), KB (kilobytes), MB (megabytes), GB
           (gigabytes), TB (terabytes).  An invalid unit string will raise a
           SyntaxError exception.  The default unit is MB."""
        lunit = unit.lower()
        physicalSectorSize = self.geometry.device.physicalSectorSize
        size = self.geometry.length * physicalSectorSize

        if lunit not in parted._exponent.keys():
            raise SyntaxError, "invalid unit %s given" % (unit,)

        return (size / math.pow(1024.0, parted._exponent[lunit]))

    def getFlagsAsString(self):
        """Return a comma-separated string representing the flags
           on this partition."""
        flags = []

        for flag in partitionFlag.keys():
            if self.getFlag(flag):
                flags.append(partitionFlag[flag])

        return string.join(flags, ', ')

    def getMaxAvailableSize(self, unit="MB"):
        """Return the maximum size this Partition can grow to by looking
           at contiguous freespace partitions.  The size is returned in
           the unit specified (default is megabytes).  The unit is a
           string corresponding to one of the following abbreviations:
           b (bytes), KB (kilobytes), MB (megabytes), GB (gigabytes),
           TB (terabytes).  An invalid unit string will raise a
           SyntaxError exception."""
        lunit = unit.lower()

        if lunit not in parted._exponent.keys():
            raise SyntaxError, "invalid unit %s given" % (unit,)

        maxLength = self.geometry.length
        nextPartition = self.disk.nextPartition(self)
        physicalSectorSize = self.geometry.device.physicalSectorSize

        while nextPartition:
            if nextPartition.type & parted.PARTITION_FREESPACE:
                maxLength += nextPartition.geometry.length
            else:
                break

            nextPartition = self.disk.nextPartition(nextPartition)

        return math.floor(maxLength * math.pow(physicalSectorSize, parted._exponent[lunit]))

    def getPedPartition(self):
        """Return the _ped.Partition object contained in this Partition.
           For internal module use only."""
        return self.__partition

# collect all partition flags and store them in a hash
partitionFlag = {}
__flag = _ped.partition_flag_next(0)
partitionFlag[__flag] = _ped.partition_flag_get_name(__flag)

while True:
    try:
        __flag = _ped.partition_flag_next(__flag)
        partitionFlag[__flag] = _ped.partition_flag_get_name(__flag)
    except:
        break
