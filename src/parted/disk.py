#
# disk.py
# Python bindings for libparted (built on top of the _ped Python module).
#
# Copyright (C) 2009-2013 Red Hat, Inc.
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
# Author(s): David Cantrell <dcantrell@redhat.com>
#            Alex Skinner <alex@lx.lc>
#

import _ped
import parted

from parted.cachedlist import CachedList
from parted.decorators import localeC

class Disk(object):
    """Disk()

       A Disk object describes a type of device in the system.  Disks
       can hold partitions.  A Disk is a basic operating system-specific
       object."""
    @localeC
    def __init__(self, device=None, PedDisk=None):
        """Create a new Disk object from the device and type specified.  The
           device is a Device object and type is a string matching a key in
           the diskType hash."""
        if PedDisk:
            self.__disk = PedDisk

            if device is None:
                self._device = parted.Device(PedDevice=self.__disk.dev)
            else:
                self._device = device
        elif device is None:
            raise parted.DiskException("no device specified")
        else:
            self.__disk = _ped.Disk(device.getPedDevice())
            self._device = device

        # pylint: disable=W0108
        self._partitions = CachedList(lambda : self.__getPartitions())

    def _hasSameParts(self, other):
        import six

        if len(self.partitions) != len(other.partitions):
            return False

        partIter = six.moves.zip(self.partitions, other.partitions)
        while True:
            try:
                (left, right) = next(partIter)
                if left != right:
                    return False
            except StopIteration:
                return True

    def __eq__(self, other):
        return not self.__ne__(other)

    def __ne__(self, other):
        if not isinstance(self, other.__class__):
            return True

        return self.device != other.device or not self._hasSameParts(other)

    def __str__(self):
        s = ("parted.Disk instance --\n"
             "  type: %(type)s  primaryPartitionCount: %(primaryCount)s\n"
             "  lastPartitionNumber: %(last)s  maxPrimaryPartitionCount: %(max)s\n"
             "  partitions: %(partitions)s\n"
             "  device: %(device)r\n"
             "  PedDisk: %(ped)r" %
             {"type": self.type, "primaryCount": self.primaryPartitionCount,
              "last": self.lastPartitionNumber, "max": self.maxPrimaryPartitionCount,
              "partitions": self.partitions, "device": self.device,
              "ped": self.__disk})
        return s

    def __getPartitions(self):
        """Construct a list of partitions on the disk.  This is called only as
           needed from the self.partitions property, which just happens to be
           a CachedList."""
        partitions = []
        partition = self.getFirstPartition()

        while partition:
            if partition.type & parted.PARTITION_FREESPACE or \
               partition.type & parted.PARTITION_METADATA or \
               partition.type & parted.PARTITION_PROTECTED:
                partition = partition.nextPartition()
                continue

            partitions.append(partition)
            partition = partition.nextPartition()

        return partitions

    @property
    @localeC
    def primaryPartitionCount(self):
        """The number of primary partitions on this disk."""
        return self.__disk.get_primary_partition_count()

    @property
    @localeC
    def lastPartitionNumber(self):
        """The last assigned partition number currently on this disk."""
        return self.__disk.get_last_partition_num()

    @property
    @localeC
    def maxPrimaryPartitionCount(self):
        """The maximum number of primary partitions allowed on this disk."""
        return self.__disk.get_max_primary_partition_count()

    @property
    @localeC
    def maxSupportedPartitionCount(self):
        """The maximum number of partitions allowed on this disk."""
        return self.__disk.get_max_supported_partition_count()

    @property
    @localeC
    def partitionAlignment(self):
        """Partition start address Alignment."""
        alignment = self.__disk.get_partition_alignment()
        return parted.Alignment(PedAlignment=alignment)

    @property
    @localeC
    def maxPartitionLength(self):
        """Max Partition Length the disk's label can represent."""
        return self.__disk.max_partition_length()

    @property
    @localeC
    def maxPartitionStartSector(self):
        """Max Partition Start Sector the disk's label can represent."""
        return self.__disk.max_partition_start_sector()

    @localeC
    def getFlag(self, flag):
        """Get the value of a particular flag on the disk.  Valid flags
           are the _ped.DISK_* constants.  See _ped.disk_flag_get_name() and
           _ped.disk_flag_get_by_name() for more help working with disk flags.
        """
        return self.__disk.get_flag(flag)

    @localeC
    def setFlag(self, flag):
        """Set the flag on this disk.  On error, an Exception will be raised.
           See getFlag() for more help on working with disk flags."""
        return self.__disk.set_flag(flag, 1)

    @localeC
    def unsetFlag(self, flag):
        """Unset the flag on this disk.  On error, an Exception will be raised.
           See getFlag() for more help on working with disk flags."""
        return self.__disk.set_flag(flag, 0)

    @localeC
    def isFlagAvailable(self, flag):
        """Return True if flag is available on this Disk, False
           otherwise."""
        return self.__disk.is_flag_available(flag)

    @property
    def partitions(self):
        """The list of partitions currently on this disk."""
        return self._partitions

    @property
    def device(self):
        """The underlying Device holding this disk and partitions."""
        return self._device

    type = property(lambda s: s.__disk.type.name, lambda s, v: setattr(s.__disk, "type", parted.diskType[v]))

    @localeC
    def duplicate(self):
        """Make a deep copy of this Disk."""
        return Disk(PedDisk=self.__disk.duplicate())

    @localeC
    def destroy(self):
        """Closes the Disk ensuring all outstanding writes are flushed."""
        return self.__disk.destroy()

    @localeC
    def commit(self):
        """Writes in-memory changes to a partition table to disk and
           informs the operating system of the changes.  Equivalent to
           calling self.commitToDevice() then self.commitToOS()."""
        self.partitions.invalidate()

        return self.__disk.commit()

    @localeC
    def commitToDevice(self):
        """Write the changes made to the in-memory description of a
           partition table to the device."""
        self.partitions.invalidate()

        return self.__disk.commit_to_dev()

    @localeC
    def commitToOS(self):
        """Tell the operating system kernel about the partition table
           layout of this Disk."""
        self.partitions.invalidate()

        return self.__disk.commit_to_os()

    @localeC
    def check(self):
        """Perform a sanity check on the partition table of this Disk."""
        return self.__disk.check()

    @localeC
    def supportsFeature(self, feature):
        """Check that the disk type supports the provided feature."""
        return self.__disk.type.check_feature(feature)

    @localeC
    def addPartition(self, partition=None, constraint=None):
        """Add a new Partition to this Disk with the given Constraint."""
        if constraint:
            result = self.__disk.add_partition(partition.getPedPartition(),
                                               constraint.getPedConstraint())
        elif not partition:
            raise parted.DiskException("no partition or constraint specified")
        else:
            result = self.__disk.add_partition(partition.getPedPartition())

        if result:
            partition.geometry = parted.Geometry(PedGeometry=partition.getPedPartition().geom)
            self.partitions.invalidate()
            return True
        else:
            return False

    @localeC
    def removePartition(self, partition=None):
        """Removes specified Partition from this Disk.  NOTE:  If the
           Partition is an extended partition, it must not contain any
           logical partitions.  Also note that the partition is not
           actually destroyed unless you use the deletePartition()
           method."""
        if not partition:
            raise parted.DiskException("no partition specified")

        if self.__disk.remove_partition(partition.getPedPartition()):
            self.partitions.invalidate()
            return True
        else:
            return False

    @localeC
    def deletePartition(self, partition):
        """Removes specified Partition from this Disk under the same
           conditions as removePartition(), but also destroy the
           removed Partition."""
        if self.__disk.delete_partition(partition.getPedPartition()):
            self.partitions.invalidate()
            return True
        else:
            return False

    @localeC
    def deleteAllPartitions(self):
        """Removes and destroys all Partitions in this Disk."""
        if self.__disk.delete_all():
            self.partitions.invalidate()
            return True
        else:
            return False

    @localeC
    def setPartitionGeometry(self, partition=None, constraint=None, start=None, end=None):
        """Sets the Geometry of the specified Partition using the given
           Constraint and start and end sectors.  Note that this method
           does not modify the partition contents, just the partition
           table."""
        if not partition or not constraint:
            raise parted.DiskException("no partition or constraint specified")

        if not start or not end:
            raise parted.DiskException("no start or end geometry specified")

        return self.__disk.set_partition_geom(partition.getPedPartition(),
                                              constraint.getPedConstraint(),
                                              start, end)

    @localeC
    def maximizePartition(self, partition=None, constraint=None):
        """Grow the Partition's Geometry to the maximum possible subject
           to Constraint."""
        if not partition:
            raise parted.DiskException("no partition specified")

        if constraint:
            return self.__disk.maximize_partition(partition.getPedPartition(),
                                                  constraint.getPedConstraint())
        else:
            return self.__disk.maximize_partition(partition.getPedPartition())

    @localeC
    def calculateMaxPartitionGeometry(self, partition=None, constraint=None):
        """Get the maximum Geometry the Partition can be grown to,
           subject to the given Constraint."""
        if not partition:
            raise parted.DiskException("no partition specified")

        if constraint:
            return parted.Geometry(PedGeometry=self.__disk.get_max_partition_geometry(partition.getPedPartition(), constraint.getPedConstraint()))
        else:
            return parted.Geometry(PedGeometry=self.__disk.get_max_partition_geometry(partition.getPedPartition()))

    @localeC
    def minimizeExtendedPartition(self):
        """Reduce the size of the extended partition to a minimum while
           still wrapping its logical partitions.  If there are no logical
           partitions, remove the extended partition."""
        ret = self.__disk.minimize_extended_partition()

        if ret:
            self.partitions.invalidate()

        return ret

    @localeC
    def getPartitionBySector(self, sector):
        """Returns the Partition that contains the sector.  If the sector
           lies within a logical partition, then the logical partition is
           returned (not the extended partition)."""
        return parted.Partition(disk=self, PedPartition=self.__disk.get_partition_by_sector(sector))

    @localeC
    def getExtendedPartition(self):
        """Return the extended Partition, if any, on this Disk."""
        try:
            return parted.Partition(disk=self, PedPartition=self.__disk.extended_partition())
        # pylint: disable=bare-except
        except:
            return None

    def __filterPartitions(self, fn):
        return [part for part in self.partitions if fn(part)]

    def getLogicalPartitions(self):
        """Return a list of logical Partitions on this Disk."""
        return self.__filterPartitions(lambda p: p.active and p.type & parted.PARTITION_LOGICAL)

    def getPrimaryPartitions(self):
        """Return a list of primary (or normal) Partitions on this Disk."""
        return self.__filterPartitions(lambda p: p.type == parted.PARTITION_NORMAL)

    def getRaidPartitions(self):
        """Return a list of RAID (or normal) Partitions on this Disk."""
        return self.__filterPartitions(lambda p: p.active and p.getFlag(parted.PARTITION_RAID))

    def getLVMPartitions(self):
        """Return a list of physical volume-type Partitions on this Disk."""
        return self.__filterPartitions(lambda p: p.active and p.getFlag(parted.PARTITION_LVM))

    @localeC
    def getFreeSpaceRegions(self):
        """Return a list of Geometry objects representing the available
           free space regions on this Disk."""
        freespace = []
        part = self.__disk.next_partition()

        while part:
            if part.type & parted.PARTITION_FREESPACE:
                freespace.append(parted.Geometry(PedGeometry=part.geom))

            part = self.__disk.next_partition(part)

        return freespace

    @localeC
    def getFreeSpacePartitions(self):
        """Return a list of Partition objects representing the available
           free space regions on this Disk."""
        freespace = []
        part = self.__disk.next_partition()

        while part:
            if part.type & parted.PARTITION_FREESPACE:
                freespace.append(parted.Partition(disk=self, PedPartition=part))

            part = self.__disk.next_partition(part)

        return freespace

    @localeC
    def getFirstPartition(self):
        """Return the first Partition object on the disk or None if
           there is not one."""
        return parted.Partition(disk=self, PedPartition=self.__disk.next_partition())

    @localeC
    def getPartitionByPath(self, path):
        """Return a Partition object associated with the partition device
           path, such as /dev/sda1.  Returns None if no partition is found."""
        for partition in self.partitions:
            if partition.path == path:
                return partition

        return None

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
    except (IndexError, TypeError, _ped.UnknownTypeException):
        break

# collect all disk flags and store them in a hash
diskFlag = {}
__flag = _ped.disk_flag_next(0)
diskFlag[__flag] = _ped.disk_flag_get_name(__flag)
__readFlags = True

while __readFlags:
    __flag = _ped.disk_flag_next(__flag)
    if not __flag:
        __readFlags = False
    else:
        diskFlag[__flag] = _ped.disk_flag_get_name(__flag)
