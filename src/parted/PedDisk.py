#
# PedDevice.py
# Python bindings for libparted (built on top of the _ped Python module).
#
# Copyright (C) 2007  Red Hat, Inc.
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
    def __init__(self, path):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)

    def __setx(self, value):
        raise AttributeError, "attribute is read-only"

#    length = property(lambda self: self.__device.length, __setx)
#    model = property(lambda self: self.__device.model, __setx)
#    path = property(lambda self: self.__device.path, __setx)
#    sector_size = property(lambda self: self.__device.sector_size, __setx)
#    type = property(lambda self: self.__device.type, __setx)
#    heads = property(lambda self: self.__device.heads, __setx)
#    sectors = property(lambda self: self.__device.sectors, __setx)
#    cylinders = property(lambda self: self.__device.cylinders, __setx)

    def new(self):
        warnings.warn("PedDisk class is deprecated by Disk", DeprecationWarning, stacklevel=2)

    def check_feature(self):
        warnings.warn("PedDisk class is deprecated by Disk", DeprecationWarning, stacklevel=2)

    def next_partition(self):
        warnings.warn("PedDisk class is deprecated by Disk", DeprecationWarning, stacklevel=2)

    def delete_partition(self):
        warnings.warn("PedDisk class is deprecated by Disk", DeprecationWarning, stacklevel=2)

    def delete_all(self):
        warnings.warn("PedDisk class is deprecated by Disk", DeprecationWarning, stacklevel=2)

    def add(self):
        warnings.warn("PedDisk class is deprecated by Disk", DeprecationWarning, stacklevel=2)

    def get_partition(self):
        warnings.warn("PedDisk class is deprecated by Disk", DeprecationWarning, stacklevel=2)

    def get_partition_by_sector(self):
        warnings.warn("PedDisk class is deprecated by Disk", DeprecationWarning, stacklevel=2)

    def partition_new(self):
        warnings.warn("PedDisk class is deprecated by Disk", DeprecationWarning, stacklevel=2)

    def minimum_extended_partition(self):
        warnings.warn("PedDisk class is deprecated by Disk", DeprecationWarning, stacklevel=2)

    def maximize_partition(self):
        warnings.warn("PedDisk class is deprecated by Disk", DeprecationWarning, stacklevel=2)

    def get_primary_partition_count(self):
        warnings.warn("PedDisk class is deprecated by Disk", DeprecationWarning, stacklevel=2)

    def get_last_partition_num(self):
        warnings.warn("PedDisk class is deprecated by Disk", DeprecationWarning, stacklevel=2)

    def commit(self):
        warnings.warn("PedDisk class is deprecated by Disk", DeprecationWarning, stacklevel=2)
