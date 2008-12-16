#
# PedPartition.py
# Python bindings for libparted (built on top of the _ped Python module).
#
# Copyright (C) 2008  Red Hat, Inc.
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

class PedPartition(object):
    # The old parted.PedPartition was not a class users could access directly.
    # It could only be instantiated through the internals.  The constructor took
    # a libparted PedPartition and parted.PedDisk as arguments.  Since we should
    # only be allowing access through parted internals too, so we can force
    # whatever arguments to __init__ make sense.
    def __init__(self, part):
        warnings.warn("PedPartition class is deprecated by Partition", DeprecationWarning, stacklevel=2)
        self._part = part

    def __setx(self, value):
        raise AttributeError, "attribute is read-only"

    num = property(lambda self: self.__part.num, __setx)
    type = property(lambda self: self.__part.type, __setx)
    disk = property(lambda self: self.__part.disk, __setx)
    native_type = property(lambda self: self.__part.native_type, __setx)
    type_name = property(lambda self: self.__part.type_name, __setx)
    geom = property(lambda self: self.__part.geom, __setx)
    fstype = property(lambda self: self.__part.geom, __setx)

    def is_flag_available(self, flag):
        warnings.warn("PedPartition class is deprecated by Partition", DeprecationWarning, stacklevel=2)

        try:
            return self.__part.is_flag_available(flag)
        except Exception, e:
            raise parted.error(e.message)

    def get_flag(self, flag):
        warnings.warn("PedPartition class is deprecated by Partition", DeprecationWarning, stacklevel=2)

        try:
            return self.__part.get_flag(flag)
        except Exception, e:
            raise parted.error(e.message)

    def set_flag(self, flag, value):
        warnings.warn("PedPartition class is deprecated by Partition", DeprecationWarning, stacklevel=2)

        try:
            self.__part.set_flag(flag, value)
            return None
        except Exception, e:
            raise parted.error(e.message)

    def is_active(self):
        warnings.warn("PedPartition class is deprecated by Partition", DeprecationWarning, stacklevel=2)

        try:
            if self.__part.is_active():
                return 1
            else:
                return 0
        except Exception, e:
            raise parted.error(e.message)

    def set_system(self, fstype):
        warnings.warn("PedPartition class is deprecated by Partition", DeprecationWarning, stacklevel=2)

        try:
            self.__part.set_system(fstype)
            return None
        except Exception, e:
            raise parted.error(e.message)

    def set_name(self, name):
        warnings.warn("PedPartition class is deprecated by Partition", DeprecationWarning, stacklevel=2)

        try:
            self.__part.set_name(name)
            return None
        except Exception, e:
            raise parted.error(e.message)

    def get_name(self):
        warnings.warn("PedPartition class is deprecated by Partition", DeprecationWarning, stacklevel=2)

        try:
            return self.__part.get_name()
        except Exception, e:
            raise parted.error(e.message)

    def is_busy(self):
        warnings.warn("PedPartition class is deprecated by Partition", DeprecationWarning, stacklevel=2)

        try:
            if self.__part.is_busy():
                return 1
            else:
                return 0
        except Exception, e:
            raise parted.error(e.message)
