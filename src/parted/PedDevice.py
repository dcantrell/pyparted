#
# PedDevice.py
# Python bindings for libparted (built on top of the _ped Python module).
#
# Copyright (C) 2007  Red Hat, Inc.
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
import warnings

__depstr = "%s is deprecated and will be removed."

class PedDevice(object):
    def __init__(self, path):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)
        self.__device = _ped.device_get(path)

    def __setx(self, value):
        raise AttributeError, "attribute is read-only"

    length = property(lambda self: self.__device.length, __setx)
    model = property(lambda self: self.__device.model, __setx)
    path = property(lambda self: self.__device.path, __setx)
    sector_size = property(lambda self: self.__device.sector_size, __setx)
    type = property(lambda self: self.__device.type, __setx)
    heads = property(lambda self: self.__device.hw_geom.heads, __setx)
    sectors = property(lambda self: self.__device.hw_geom.sectors, __setx)
    cylinders = property(lambda self: self.__device.hw_geom.cylinders, __setx)

    def close(self):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)

        try:
            return self.__device.close()
        except Exception, e:
            raise parted.error(e.message)

    def constraint_any(self):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)

        try:
            return _ped.constraint_any(self.__device)
        except Exception, e:
            raise parted.error(e.message)

    def destroy(self):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)
        return self.__device.destroy()

    def disk_new_fresh(self, type):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)

        try:
            return _ped.Disk(self.__device, type)
        except TypeError:
            raise
        except Exception, e:
            raise parted.error(e.message)

    def disk_probe(self):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)

        try:
            return self.__device.disk_probe()
        except Exception, e:
            raise parted.error(e.message)

    def geometry_new(self, start, length):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)

        try:
            return _ped.Geometry(self.__device, start, length)
        except Exception, e:
            raise parted.error(e.message)

    def is_busy(self):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)

        try:
            return self.__device.is_busy()
        except Exception, e:
            raise parted.error(e.message)

    def open(self):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)

        try:
            return self.__device.open()
        except Exception, e:
            raise parted.error(e.message)

    def sync(self):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)

        try:
            return self.__device.sync()
        except Exception, e:
            raise parted.error(e.message)

def get(path):
    import warnings

    warnings.warn("PedDevice.get() is deprecated by Device.get()", DeprecationWarning, stacklevel=2)
    return PedDevice(path)
