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

class Disk(object):
    """Disk()

       A Disk object describes a type of device in the system.  Disks
       can hold partitions.  A Disk is a basic operating system-specific
       object."""
    def __init__(self, device=None, PedDisk=None):
        """Create a new Disk object from the device and type specified.  The
           device is a Device object and type is a string matching a key in
           the diskType hash."""
        if PedDisk:
            self.__disk = PedDisk
            self._device = parted.Device(PedDevice=self.__disk.dev)
        elif device is None:
            raise parted.DiskException, "no device specified"
        else:
            self.__disk = _ped.Disk(device.getPedDevice())
            self._device = device

    def __readOnly(self, property):
        raise parted.ReadOnlyProperty, property

    def __getPartitions(self):
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

    type = property(lambda s: s.__disk.type.name, lambda s, v: setattr(s.__disk, "type", parted.diskType[v]))
    primaryPartitionCount = property(lambda s: s.__disk.get_primary_partition_count(), lambda s, v: s.__readOnly("primaryPartitionCount"))
    lastPartitionNumber = property(lambda s: s.__disk.get_last_partition_num(), lambda s, v: s.__readOnly("lastPartitionNumber"))
    maxPrimaryPartitionCount = property(lambda s: s.__disk.get_max_primary_partition_count(), lambda s, v: s.__readOnly("maxPrimaryPartitionCount"))
    partitions = property(lambda s: s.__getPartitions(), lambda s, v: s.__readOnly("partitions"))
    device = property(lambda s: s._device, lambda s, v: s.__readOnly("device"))

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

    def supportsFeature(self, feature):
        """Check that the disk type supports the provided feature."""
        return self.__disk.type.check_feature(feature)

    def addPartition(self, partition=None, constraint=None):
        """Add a new Partition to this Disk with the given Constraint."""
        return self.__disk.add_partition(partition.getPedPartition(),
                                         constraint.getPedConstraint())

    def removePartition(self, partition=None):
        """Removes specified Partition from this Disk.  NOTE:  If the
           Partition is an extended partition, it must not contain any
           logical partitions.  Also note that the partition is not
           actually destroyed unless you use the deletePartition()
           method."""
        return self.__disk.remove_partition(partition.getPedPartition())

    def deletePartition(self, partition):
        """Removes specified Partition from this Disk under the same
           conditions as removePartition(), but also destroy the
           removed Partition."""
        return self.__disk.delete_partition(partition.getPedPartition())

    def deleteAllPartitions(self):
        """Removes and destroys all Partitions in this Disk."""
        return self.__disk.delete_all()

    def setPartitionGeometry(self, partition=None, constraint=None, start=None, end=None):
        """Sets the Geometry of the specified Partition using the given
           Constraint and start and end sectors.  Note that this method
           does not modify the partition contents, just the partition
           table."""
        return self.__disk.set_partition_geom(partition.getPedPartition(),
                                              constraint.getPedConstraint(),
                                              start, end)

    def maximizePartition(self, partition=None, constraint=None):
        """Grow the Partition's Geometry to the maximum possible subject
           to Constraint."""
        return self.__disk.maximize_partition(partition.getPedPartition(),
                                              constraint.getPedConstraint())

    def calculateMaxPartitionGeometry(self, partition=None, constraint=None):
        """Get the maximum Geometry the Partition can be grown to,
           subject to the given Constraint."""
        return self.__disk.get_max_partition_geometry(partition.getPedPartition(), constraint.getPedConstraint())

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

    def getMaxLogicalPartitions(self):
        """Return the maximum number of logical partitions this Disk
           will hold.  Returns 0 if there is no extended partition on
           the disk, returns 11 when all else fails."""
        if not self.supportsFeature(parted.DISK_TYPE_EXTENDED):
            return 0

        # maximum number of logical partitions per device type
        maxLogicalPartitionCount = {
            "hd": 59,
            "sd": 11,
            "ataraid/": 11,
            "rd/": 3,
            "cciss/": 11,
            "i2o/": 11,
            "iseries/vd": 3,
            "ida/": 11,
            "sx8/": 11,
            "xvd": 11,
            "vd": 11,
            "mmcblk": 5
        }

        dev = self.device.path[5:]
        for key in maxLogicalPartitionCount.keys():
            if dev.startswith(key):
                return maxLogicalPartitionCount[key]

        # XXX: if we don't know about it, should we pretend it can't have
        # logicals?  probably safer to just use something reasonable
        return 11

    def getExtendedPartition(self):
        """Return the extended Partition, if any, on this Disk."""
        try:
            return parted.Partition(PedPartition=self.__disk.extended_partition())
        except:
            return None

    def __filterPartitions(self, fn):
        return filter(fn, self.partitions)

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

    def getFreeSpacePartitions(self):
        """Return a list of Partition objects representing the available
           free space regions on this Disk."""
        freespace = []
        part = self.__disk.next_partition()

        while part:
            if part.type & parted.PARTITION_FREESPACE:
                freespace.append(parted.Partition(PedPartition=part))

            part = self.__disk.next_partition(part)

        return freespace

    def getFirstPartition(self):
        """Return the first Partition object on the disk or None if
           there is not one."""
        return parted.Partition(PedPartition=self.__disk.next_partition())

    def getPartitionByPath(self, path):
        """Return a Partition object associated with the partition device
           path, such as /dev/sda1.  Returns None if no partition is found."""
        for partition in self.partitions:
            if "/dev/%s" % partition.getDeviceNodeName() == path:
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
    except:
        break
