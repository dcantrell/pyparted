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

class Device(object):
    """Device()

       Device represents a phyiscal piece of hardware in the system, e.g. a
       disk.  A Device should be considered a low-level and operating system
       specific interface to the hardware.

       A number of read-only properties about the Device are available.

       For information on the individual methods, see help(Device.METHODNAME)"""

    def __init__(self, path=None, device=None):
        """Create a new Device object based on the specified path or the
           already existing _ped.Device object.  You must provide either a
           path (e.g., "/dev/sda") or an existing _ped.Device object, but
           not both."""

        if path is not None:
            self.__device = _ped.device_get(path)
        elif device is not None:
            self.__device = device
        else:
            raise _ped.PartedException, "no path or _ped.Device specified"

    def __readOnlyProperty(self, property=''):
        raise parted.ReadOnlyProperty, property

    model = property(lambda s: s.__device.model, lambda s, v: setattr(s.__device, "model", v))
    path = property(lambda s: s.__device.path, lambda s, v: setattr(s.__device, "path", v))
    type = property(lambda s: s.__device.type, lambda s, v: setattr(s.__device, "type", v))
    sectorSize = property(lambda s: s.__device.sector_size, lambda s, v: setattr(s.__device, "sector_size", v))
    physicalSectorSize = property(lambda s: s.__device.phys_sector_size, lambda s, v: setattr(s.__device, "phys_sector_size", v))
    length = property(lambda s: s.__device.length, lambda s, v: setattr(s.__device, "length", v))
    openCount = property(lambda s: s.__device.open_count, lambda s, v: setattr(s.__device, "open_count", v))
    readOnly = property(lambda s: s.__device.read_only, lambda s, v: setattr(s.__device, "read_only", v))
    externalMode = property(lambda s: s.__device.external_mode, lambda s, v: setattr(s.__device, "external_mode", v))
    dirty = property(lambda s: s.__device.dirty, lambda s, v: setattr(s.__device, "dirty", v))
    bootDirty = property(lambda s: s.__device.boot_dirty, lambda s, v: setattr(s.__device, "boot_dirty", v))
    host = property(lambda s: s.__device.host, lambda s, v: setattr(s.__device, "host", v))
    did = property(lambda s: s.__device.did, lambda s, v: setattr(s.__device, "did", v))

    busy = property(lambda s: s.__device.is_busy(), lambda s, v: s.__readOnlyProperty("busy"))

    def open(self):
        """Open this Device for read operations."""

        return self.__device.open()

    def close(self):
        """Close this Device.  Used after open() method calls."""

        return self.__device.close()

    def destroy(self):
        """Destroy this Device.  Operating system specific."""

        return self.__device.destroy()

    def removeFromCache(self):
        """Remove this Device from the internal libparted device cache."""

        return self.__device.cache_remove()

    def beginExternalAccess(self):
        """Set up the Device for use by an external program.  Call this method
           before running an external program that uses the Device."""

        return self.__device.begin_external_access()

    def endExternalAccess(self):
        """Turn off external access mode for the Device.  Call this method once
           your external program has finished using the Device."""

        return self.__device.end_external_access()

    def read(self, buffer, start, count):
        """From the sector indentified by start, read count sectors from the
           Device in to buffer."""

        return self.__device.read(buffer, start, count)

    def write(self, buffer, start, count):
        """From the sector identified by start, write count sectors from
           buffer to the Device."""

        return self.__device.write(buffer, start, count)

    def sync(self, fast=False):
        """Perform a operating-system specific sync(2) operation on the
           Device.  If fast is True, try to perform a fast sync(2)."""

        if fast:
            return self.__device.sync_fast()
        else:
            return self.__device.sync()

    def check(self, buffer, start, count):
        """From the sector identified by start, perform an operating
           system specific check on count sectors."""

        return self.__device.check(buffer, start, count)

    def getConstraint(self):
        """Return a Constraint defining the limitations imposed by
           this Device."""

        return self.__device.get_constraint()
