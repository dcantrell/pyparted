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
from parted import Device

# XXX: add docstrings!

# XXX: figure out how to do _ped.DiskType

class Disk(object):
    # XXX: fixme
    def __init__(self, device=None, type=None):
        if device is None or type is None:
            raise _ped.PartedException, "no type or _ped.Device specified"

        self.__device = device
        self.__type = type
        self.__disk = _ped.Disk(self.__device.getPedDevice(), self.__type)
        self.__partitions = {}

        i = 1
        while i <= self.lastPartitionNumber:
            self.__partitions[i] = parted.Partition(self.__disk.get_partition(i))

    def __readOnlyProperty(self, property=''):
        raise parted.ReadOnlyProperty, property

    number = property(lambda s: s.__disk.num, lambda s, v: setattr(s.__disk, "num", v))
    type = property(lambda s: s.__disk.type, lambda s, v: setattr(s.__disk, "type", v))
    primaryPartitionCount = property(lambda s: s.__disk.get_primary_partition_count(), lambda s, v: s.__readOnlyProperty("primaryPartitionCount"))
    lastPartitionNumber = property(lambda s: s.__disk.get_last_partition_num(), lambda s, v: s.__readOnlyProperty("lastPartitionNumber"))
    maxPrimaryPartitionCount = property(lambda s: s.__disk.get_max_primary_partition_count(), lambda s, v: s.__readOnlyProperty("maxPrimaryPartitionCount"))

    def clobber(self, type=None):
        if type is None:
            return s.__disk.clobber()
        else:
            return s.__disk.clobber_exclude(type)

    def duplicate(self):
        return s.__disk.duplicate()

    def destroy(self):
        return s.__disk.destroy()

    def commit(self):
        return s.__disk.commit()

    def commitToDevice(self):
        return s.__disk.commit_to_dev()

    def commitToOS(self):
        return s.__disk.commit_to_os()

    def check(self):
        return s.__disk.check()

    def addPartition(self, partition=None, constraint=None):
        return s.__disk.add_partition(partition, constraint)

    # XXX: is removePartition and deletePartition necessary?
    def removePartition(self, partition=None):
        return s.__disk.remove_partition(partition)

    def deletePartition(self, partition):
        return s.__disk.delete_partition(partition)

    def deleteAllPartitions(self):
        return s.__disk.delete_all()

    def setPartitionGeometry(self, partition=None, constraint=None, start, end):
        return s.__disk.set_partition_geom(partition, constraint, start, end)

    def maximizePartition(self, partition=None, constraint=None):
        return s.__disk.maximize_partition(partition, constraint)

    def calculateMaxPartitionGeometry(self, partition=None, constraint=None):
        return s.__disk.get_max_partition_geometry(partition, constraint)

    def minimizeExtendedPartition(self):
        return s.__disk.minimize_extended_partition()

    def getPartitionBySector(self, sector):
        return s.__disk.get_partition_by_sector(sector)

    def getExtendedPartition(self):
        return s.__disk.extended_partition()
