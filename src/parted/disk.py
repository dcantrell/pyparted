#
# disk.py
# Python bindings for libparted (built on top of the _ped Python module).
#
# Copyright (C) 2009  Red Hat, Inc.
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
# Red Hat Author(s): David Cantrell <dcantrell@redhat.com>
#

import _ped
import parted
from parted.partition import *

class Disk(object):
    """Disk()

       A Disk object describes a type of device in the system.  Disks
       can hold partitions.  A Disk is a basic operating system-specific
       object."""
    def __init__(self, device=None, type=None, PedDisk=None):
        """Create a new Disk object from the device and type specified.  The
           device is a Device object and type is a string matching a key in
           the diskType hash."""
        if PedDisk:
            self.__disk = PedDisk
        else:
            if device is None or type is None:
                raise _ped.PartedException, "no type or _ped.Device specified"

            self._device = device
            self._type = type
            self.__disk = _ped.Disk(self._device.getPedDevice(), self._type)

        self.partitions = {}

        i = 1
        while i <= self.lastPartitionNumber:
            self.partitions[i] = Partition(self.__disk.get_partition(i))

    def __readOnly(self, property):
        raise parted.ReadOnlyProperty, property

    number = property(lambda s: s.__disk.num, lambda s, v: setattr(s.__disk, "num", v))
    type = property(lambda s: s.__disk.type, lambda s, v: setattr(s.__disk, "type", v))
    primaryPartitionCount = property(lambda s: s.__disk.get_primary_partition_count(), lambda s, v: s.__readOnly("primaryPartitionCount"))
    lastPartitionNumber = property(lambda s: s.__disk.get_last_partition_num(), lambda s, v: s.__readOnly("lastPartitionNumber"))
    maxPrimaryPartitionCount = property(lambda s: s.__disk.get_max_primary_partition_count(), lambda s, v: s.__readOnly("maxPrimaryPartitionCount"))

    def clobber(self, type=None):
        """Remove all identifying signatures of the partition table.  If type
           is not None, remove all identifying signatures of the partition
           table, except for partition tables of that type.  type must be a
           string matching a valid key in the diskType hash."""
        if type is None:
            return self.__disk.clobber()
        else:
            return self.__disk.clobber_exclude(diskType[type])

    def duplicate(self):
        """Make a deep copy of this Disk."""
        return self.__disk.duplicate()

    def destroy(self):
        """Closes the Disk ensuring all outstanding writes are flushed."""
        return self.__disk.destroy()

    def commit(self):
        """Writes in-memory changes to a partition table to disk and
           informs the operating system of the changes.  Equivalent to
           calling self.commitToDevice() then self.commitToOS()."""
        return self.__disk.commit()

    def commitToDevice(self):
        """Write the changes made to the in-memory description of a
           partition table to the device."""
        return self.__disk.commit_to_dev()

    def commitToOS(self):
        """Tell the operating system kernel about the partition table
           layout of this Disk."""
        return self.__disk.commit_to_os()

    def check(self):
        """Perform a sanity check on the partition table of this Disk."""
        return self.__disk.check()

    def addPartition(self, partition=None, constraint=None):
        """Add a new Partition to this Disk with the given Constraint."""
        return self.__disk.add_partition(partition, constraint)

    def removePartition(self, partition=None):
        """Removes specified Partition from this Disk.  NOTE:  If the
           Partition is an extended partition, it must not contain any
           logical partitions.  Also note that the partition is not
           actually destroyed unless you use the deletePartition()
           method."""
        return self.__disk.remove_partition(partition)

    def deletePartition(self, partition):
        """Removes specified Partition from this Disk under the same
           conditions as removePartition(), but also destroy the
           removed Partition."""
        return self.__disk.delete_partition(partition)

    def deleteAllPartitions(self):
        """Removes and destroys all Partitions in this Disk."""
        return self.__disk.delete_all()

    def setPartitionGeometry(self, partition=None, constraint=None, start=None, end=None):
        """Sets the Geometry of the specified Partition using the given
           Constraint and start and end sectors.  Note that this method
           does not modify the partition contents, just the partition
           table."""
        return self.__disk.set_partition_geom(partition, constraint, start, end)

    def maximizePartition(self, partition=None, constraint=None):
        """Grow the Partition's Geometry to the maximum possible subject
           to Constraint."""
        return self.__disk.maximize_partition(partition, constraint)

    def calculateMaxPartitionGeometry(self, partition=None, constraint=None):
        """Get the maximum Geometry the Partition can be grown to,
           subject to the given Constraint."""
        return self.__disk.get_max_partition_geometry(partition, constraint)

    def minimizeExtendedPartition(self):
        """Reduce the size of the extended partition to a minimum while
           still wrapping its logical partitions.  If there are no logical
           partitions, remove the extended partition."""
        return self.__disk.minimize_extended_partition()

    def getPartitionBySector(self, sector):
        """Returns the Partition that contains the sector.  If the sector
           lies within a logical partition, then the logical partition is
           returned (not the extended partition)."""
        return self.__disk.get_partition_by_sector(sector)

    def getExtendedPartition(self):
        """Return the extended Partition, if any, on this Disk."""
        return self.__disk.extended_partition()

    def getPedDisk(self):
        """Return the _ped.Disk object contained in this Disk.  For internal
           module use only."""
        return self.__disk

# collect all disk types and store them in a hash
diskType = {}
__type = _ped.disk_type_get_next()
diskType[__type.name] = __type

while True:
    try:
        __type = _ped.disk_type_get_next(__type)
        diskType[__type.name] = __type
    except:
        break
