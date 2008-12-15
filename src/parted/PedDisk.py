#
# PedDisk.py
# Python bindings for libparted (built on top of the _ped Python module).
#
# Copyright (C) 2007, 2008 Red Hat, Inc.
# All rights reserved.
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
import parted
import warnings

__depstr = "%s is deprecated and will be removed."

class PedDisk(object):
    def __init__(self, type):
        warnings.warn("PedDisk class is deprecated by Disk", DeprecationWarning, stacklevel=2)
        self.__disk = _ped.Disk(type)

    def __setx(self, value):
        raise AttributeError, "attribute is read-only"

    dev = property(lambda self: self.__device.dev, __setx)
    type = property(lambda self: self.__device.type, __setx)
    max_primary_partition_count = property(lambda self: self.__device.get_max_primary_partition_count(), __setx)
    extended_partition = property(lambda self: self.__device.extended_partition(), __setx)

    def next_partition(self, parttype):
        warnings.warn("PedDisk class is deprecated by Disk", DeprecationWarning, stacklevel=2)

        try:
            return self.__disk.next_partition(parttype)
        except TypeError:
            raise
        except Exception, e:
            raise parted.error(e.message)

    def delete_partition(self, parttype):
        warnings.warn("PedDisk class is deprecated by Disk", DeprecationWarning, stacklevel=2)

        try:
            return self.__disk.delete_partition(parttype)
        except TypeError:
            raise
        except Exception, e:
            raise parted.error(e.message)

    def delete_all(self):
        warnings.warn("PedDisk class is deprecated by Disk", DeprecationWarning, stacklevel=2)

        try:
            return self.__disk.delete_all()
        except Exception, e:
            raise parted.error(e.message)

    def add(self, part, constraint):
        warnings.warn("PedDisk class is deprecated by Disk", DeprecationWarning, stacklevel=2)

        try:
            return self.__disk.add(part, constraint)
        except TypeError:
            raise
        except Exception, e:
            raise parted.error(e.message)

    def get_partition(self, num):
        warnings.warn("PedDisk class is deprecated by Disk", DeprecationWarning, stacklevel=2)

        try:
            return self.__disk.get_partition(part, num)
        except Exception, e:
            raise parted.error(e.message)

    def get_partition_by_sector(self, sector):
        warnings.warn("PedDisk class is deprecated by Disk", DeprecationWarning, stacklevel=2)

        try:
            return self.__disk.get_partition_by_sector(part, sector)
        except Exception, e:
            raise parted.error(e.message)

    def partition_new(self, type, fs, start, end):
        warnings.warn("PedDisk class is deprecated by Disk", DeprecationWarning, stacklevel=2)

        try:
            raise _ped.Partition(self.__device, type, fs, start, end)
        except TypeError:
            raise
        except Exception, e:
            raise parted.error(e.message)

    def minimum_extended_partition(self):
        warnings.warn("PedDisk class is deprecated by Disk", DeprecationWarning, stacklevel=2)

        try:
            return self.__disk.minimize_extended_partition()
        except Exception, e:
            raise parted.error(e.message)

    def maximize_partition(self, part, constraint):
        warnings.warn("PedDisk class is deprecated by Disk", DeprecationWarning, stacklevel=2)

        try:
            return self.__disk.maximize_partition(part, constraint)
        except Exception, e:
            raise parted.error(e.message)

    def get_primary_partition_count(self):
        warnings.warn("PedDisk class is deprecated by Disk", DeprecationWarning, stacklevel=2)

        try:
            return self.__disk.get_primary_partition_count()
        except Exception, e:
            raise parted.error(e.message)

    def get_last_partition_num(self):
        warnings.warn("PedDisk class is deprecated by Disk", DeprecationWarning, stacklevel=2)

        try:
            return self.__disk.get_last_partition_num()
        except Exception, e:
            raise parted.error(e.message)

    def commit(self):
        warnings.warn("PedDisk class is deprecated by Disk", DeprecationWarning, stacklevel=2)

        try:
            return self.__disk.commit()
        except Exception, e:
            raise parted.error(e.message)
