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

# XXX: add docstrings!

class Disk(object):
    # XXX: fixme
    def __init__(self, device=None, type=None, PedDisk=None):
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
        if type is None:
            return self.__disk.clobber()
        else:
            return self.__disk.clobber_exclude(type)

    def duplicate(self):
        return self.__disk.duplicate()

    def destroy(self):
        return self.__disk.destroy()

    def commit(self):
        return self.__disk.commit()

    def commitToDevice(self):
        return self.__disk.commit_to_dev()

    def commitToOS(self):
        return self.__disk.commit_to_os()

    def check(self):
        return self.__disk.check()

    def addPartition(self, partition=None, constraint=None):
        return self.__disk.add_partition(partition, constraint)

    # XXX: is removePartition and deletePartition necessary?
    def removePartition(self, partition=None):
        return self.__disk.remove_partition(partition)

    def deletePartition(self, partition):
        return self.__disk.delete_partition(partition)

    def deleteAllPartitions(self):
        return self.__disk.delete_all()

    def setPartitionGeometry(self, partition=None, constraint=None, start=None, end=None):
        return self.__disk.set_partition_geom(partition, constraint, start, end)

    def maximizePartition(self, partition=None, constraint=None):
        return self.__disk.maximize_partition(partition, constraint)

    def calculateMaxPartitionGeometry(self, partition=None, constraint=None):
        return self.__disk.get_max_partition_geometry(partition, constraint)

    def minimizeExtendedPartition(self):
        return self.__disk.minimize_extended_partition()

    def getPartitionBySector(self, sector):
        return self.__disk.get_partition_by_sector(sector)

    def getExtendedPartition(self):
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
