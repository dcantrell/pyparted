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
# Red Hat Author(s): David Cantrell <dcantrell@redhat.com>
#
import _ped
import warnings

__depstr = "%s is deprecated and will be removed."

class PedDevice(object):
    def __init__(self, path):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)
        self.__device = _ped.Device().get(path)

    def __setx(self, value):
        raise AttributeError, "attribute is read-only"

    length = property(lambda self: self.__device.length, __setx)
    model = property(lambda self: self.__device.model, __setx)
    path = property(lambda self: self.__device.path, __setx)
    sector_size = property(lambda self: self.__device.sector_size, __setx)
    type = property(lambda self: self.__device.type, __setx)
    heads = property(lambda self: self.__device.heads, __setx)
    sectors = property(lambda self: self.__device.sectors, __setx)
    cylinders = property(lambda self: self.__device.cylinders, __setx)

    def close(self):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)
        return _ped.Device().close(self.__device)

    def constraint_any(self):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)
        return _ped.Constraint().any(self.__device)

    def destroy(self):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)
        return _ped.Device().destroy(self.__device)

    def disk_new_fresh(self, type):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)
        return _ped.Disk().new_fresh(self.__device, type)

    def disk_probe(self):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)
        return _ped.Disk().probe(self.__device)

    def geometry_new(self, start, length):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)
        return _ped.Geometry().new(self.__device, start, length)

    def is_busy(self):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)
        return _ped.Device().is_busy(self.__device)

    def open(self):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)
        return _ped.Device().open(self.__device)

    def sync(self):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)
        return _ped.Device().sync(self.__device)

def get(path):
    import warnings

    warnings.warn("PedDevice.get() is deprecated by Device.get()", DeprecationWarning, stacklevel=2)
    return PedDevice(path)
