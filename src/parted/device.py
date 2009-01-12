#
# device.py
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

# XXX: add python docstrings

class Device(Object):
    def __init__(self, path=None, device=None):
        if path is not None:
            self.__device = _ped.device_get(path)
        elif device is not None:
            self.__device = device
        else:
            raise _ped.PartedException, "no path or _ped.Device specified"

    model = property(lambda s: s.__device.model, lambda s, v: s.__device.model = v)
    path = property(lambda s: s.__device.path, lambda s, v: s.__device.path = v)
    type = property(lambda s: s.__device.type, lambda s, v: s.__device.type = v)
    sectorSize = property(lambda s: s.__device.sector_size, lambda s, v: s.__device.sector_size = v)
    physicalSectorSize = property(lambda s: s.__device.phys_sector_size, lambda s, v: s.__device.phys_sector_size = v)
    length = property(lambda s: s.__device.length, lambda s, v: s.__device.length = v)
    openCount = property(lambda s: s.__device.open_count, lambda s, v: s.__device.open_count = v)
    readOnly = property(lambda s: s.__device.read_only, lambda s, v: s.__device.read_only = v)
    externalMode = property(lambda s: s.__device.external_mode, lambda s, v: s.__device.external_mode = v)
    dirty = property(lambda s: s.__device.dirty, lambda s, v: s.__device.dirty = v)
    bootDirty = property(lambda s: s.__device.boot_dirty, lambda s, v: s.__device.boot_dirty = v)
    host = property(lambda s: s.__device.host, lambda s, v: s.__device.host = v)
    did = property(lambda s: s.__device.did, lambda s, v: s.__device.did = v)

    # XXX: define parted.ReadOnlyProperty exception
    busy = property(lambda s: s.__device.is_busy(), lambda s: raise parted.ReadOnlyProperty, "busy is a read-only property")

    def open(self):
        return self.__device.open()

    def close(self):
        return self.__device.close()

    def destroy(self):
        return self.__device.destroy()

    def removeFromCache(self):
        return self.__device.cache_remove()

    def beginExternalAccess(self):
        return self.__device.begin_external_access()

    def endExternalAccess(self):
        return self.__device.end_external_access()

    def read(self, buffer, start, count):
        return self.__device.read(buffer, start, count)

    def write(self, buffer, start, count):
        return self.__device.write(buffer, start, count)

    def sync(self, fast=False):
        if fast:
            return self.__device.sync_fast()
        else:
            return self.__device.sync()

    def check(self, buffer, start, count):
        return self.__device.check(buffer, start, count)

    def getConstraint(self):
        return self.__device.get_constraint()
