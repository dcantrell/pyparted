#
# Copyright The pyparted Project Authors
# SPDX-License-Identifier: GPL-2.0-or-later
#

import _ped
import parted
import os
import tempfile
import unittest

# Base class for any test case that requires a temp device node
class RequiresDeviceNode(unittest.TestCase):
    def setUp(self):
        self.addCleanup(self.removeTempDevice)

        self.temp_prefix = "temp-device-"
        (self.fd, self.path) = tempfile.mkstemp(prefix=self.temp_prefix)
        self.f = os.fdopen(self.fd)
        self.f.seek(140000)
        os.write(self.fd, b"0")

    def removeTempDevice(self):
        os.close(self.fd)

        if self.path and os.path.exists(self.path):
            os.unlink(self.path)


# Base class for any test case that requires a _ped.Device or parted.Device
# object first.
class RequiresDevice(RequiresDeviceNode):
    def setUp(self):
        RequiresDeviceNode.setUp(self)
        self._device = _ped.device_get(self.path)
        self.device = parted.getDevice(self.path)


# Base class for any test case that requires a filesystem on a device.
class RequiresFileSystem(unittest.TestCase):
    def setUp(self):
        self.addCleanup(self.removeTempDevice)

        self._fileSystemType = {}
        ty = _ped.file_system_type_get_next()
        self._fileSystemType[ty.name] = ty

        while True:
            try:
                ty = _ped.file_system_type_get_next(ty)
                self._fileSystemType[ty.name] = ty
            except (IndexError, TypeError, _ped.UnknownTypeException):
                break

        self.temp_prefix = "temp-device-"
        (
            self.fd,
            self.path,
        ) = tempfile.mkstemp(prefix=self.temp_prefix)
        self.f = os.fdopen(self.fd)
        self.f.seek(140000)
        os.write(self.fd, b"0")
        self.f.close()

        os.system("mke2fs -F -q %s" % (self.path,))

        self._device = _ped.device_get(self.path)
        self._geometry = _ped.Geometry(self._device, 0, self._device.length - 1)

    def removeTempDevice(self):
        if self.path and os.path.exists(self.path):
            os.unlink(self.path)


# Base class for certain alignment tests that require a _ped.Device
class RequiresDeviceAlignment(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)

    def roundDownTo(self, sector, grain_size):
        if sector < 0:
            shift = sector % grain_size + grain_size
        else:
            shift = sector % grain_size

        return sector - shift

    def roundUpTo(self, sector, grain_size):
        if sector % grain_size:
            return self.roundDownTo(sector, grain_size) + grain_size
        else:
            return sector

    def closestInsideGeometry(self, alignment, geometry, sector):
        if alignment.grain_size == 0:
            if alignment.is_aligned(geometry, sector) and (
                (geometry is None) or geometry.test_sector_inside(sector)
            ):
                return sector
            else:
                return -1

        if sector < geometry.start:
            sector += self.roundUpTo(geometry.start - sector, alignment.grain_size)

        if sector > geometry.end:
            sector -= self.roundUpTo(sector - geometry.end, alignment.grain_size)

        if not geometry.test_sector_inside(sector):
            return -1

        return sector

    def closest(self, sector, a, b):
        if a == -1:
            return b

        if b == -1:
            return a

        if abs(sector - a) < abs(sector - b):
            return a
        else:
            return b


# Base class for any test case that requires a labeled device
class RequiresLabeledDevice(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        os.system("parted -s %s mklabel msdos" % (self.path,))


# Base class for any test case that requires a _ped.Disk or parted.Disk.
class RequiresDisk(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        self._disk = _ped.disk_new_fresh(self._device, _ped.disk_type_get("msdos"))
        self.disk = parted.Disk(PedDisk=self._disk)

    def reopen(self):
        self._disk = _ped.disk_new(self._device)
        self.disk = parted.Disk(PedDisk=self._disk)


# Base class for any test case that requires a GPT-labeled _ped.Disk or parted.Disk.
class RequiresGPTDisk(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        self._disk = _ped.disk_new_fresh(self._device, _ped.disk_type_get("gpt"))
        self.disk = parted.Disk(PedDisk=self._disk)

    def reopen(self):
        self._disk = _ped.disk_new(self._device)
        self.disk = parted.Disk(PedDisk=self._disk)


# Base class for any test case that requires a filesystem made and mounted.
class RequiresMount(RequiresDevice):
    def setUp(self):
        self.addCleanup(self.removeMountpoint)
        RequiresDevice.setUp(self)
        self.mountpoint = None

    def mkfs(self):
        os.system("mkfs.ext2 -F -q %s" % self.path)

    def doMount(self):
        self.mountpoint = tempfile.mkdtemp()
        os.system("mount -o loop %s %s" % (self.path, self.mountpoint))

    def removeMountpoint(self):
        if self.mountpoint and os.path.exists(self.mountpoint):
            os.system("umount %s" % self.mountpoint)
            os.rmdir(self.mountpoint)


# Base class for any test case that requires a _ped.Partition.
class RequiresPartition(RequiresDisk):
    def setUp(self):
        RequiresDisk.setUp(self)
        self._part = _ped.Partition(
            disk=self._disk,
            type=_ped.PARTITION_NORMAL,
            start=1,
            end=100,
            fs_type=_ped.file_system_type_get("ext2"),
        )

    def reopen(self):
        RequiresDisk.reopen(self)
        self._part = self._disk.next_partition(self._disk.next_partition())


# Base class for any test case that requires a _ped.Partition on GPT disk.
class RequiresGPTPartition(RequiresGPTDisk):
    def setUp(self):
        RequiresGPTDisk.setUp(self)
        self._part = _ped.Partition(
            disk=self._disk,
            type=_ped.PARTITION_NORMAL,
            start=0,
            end=100,
            fs_type=_ped.file_system_type_get("ext2"),
        )

    def reopen(self):
        RequiresDisk.reopen(self)
        self._part = self._disk.next_partition()


# Base class for any test case that requires a hash table of all
# _ped.DiskType objects available
class RequiresDiskTypes(unittest.TestCase):
    def setUp(self):
        self.disktype = {}
        ty = _ped.disk_type_get_next()
        self.disktype[ty.name] = ty

        while True:
            try:
                ty = _ped.disk_type_get_next(ty)
                self.disktype[ty.name] = ty
            except (IndexError, TypeError, _ped.UnknownTypeException):
                break


# Base class for any test case that requires a list being built via successive
# calls of some function.  The function must raise IndexError when there's no
# more output to add to the return list.  This class is most useful for all
# those _get_next methods.
class BuildList:
    def getDeviceList(self, func):
        lst = []
        prev = None

        while True:
            try:
                if not prev:
                    prev = func()
                else:
                    prev = func(prev)

                lst.append(prev)
            except IndexError:
                break

        return lst
