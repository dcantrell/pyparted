#!/usr/bin/python
#
# Copyright (C) 2008, 2009  Red Hat, Inc.
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
#                    David Cantrell <dcantrell@redhat.com>
#

import _ped
import os
import tempfile
import unittest

# Base class for any test case that requires a _ped.Device object first.
class RequiresDevice(unittest.TestCase):
    def setUp(self):
        (fd, self.path) = tempfile.mkstemp(prefix="temp-device-")
        f = os.fdopen(fd)
        f.seek(33554432)             # 32MB test image
        os.write(fd, "0")

        self._device = _ped.device_get(self.path)

    def tearDown(self):
        os.unlink(self.path)

# Base class for any test case that requires a _ped.Disk.
class RequiresDisk(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        self._disk = _ped.Disk(self._device)

# Base class for any test case that requires a DOS labeled _ped.Disk
# Place 3 partitions on the test disk to facilitate testing
class RequiresLabeledDisk(RequiresDisk):
    def setUp(self):
        RequiresDevice.setUp(self)

        # set up 3 partitions on this device
        # our fake disk has the following properties:
        #     cylinders=65
        #     heads=16
        #     sectors=63
        # these numbers are just made up, but compute to 32MB more or less
        # we have to provide CHS values on the sfdisk command line
        (fd, self.script) = tempfile.mkstemp(prefix="sfdisk-script-")
        f = open(self.script, "w")
        f.write("0,8,L\n")
        f.write(",8,L\n")
        f.write(",8,L\n")
        f.close()
        os.system("sfdisk -f --no-reread -uM -C65 -H16 -S63 %s < %s" %
                  (self.path, self.script,))

    def tearDown(self):
        RequiresDevice.tearDown(self)
        os.unlink(self.script)

# Base class for any test case that requires a filesystem made and mounted.
class RequiresMount(RequiresDevice):
    def mkfs(self):
        os.system("mkfs.ext2 -F -q %s" % self.path)

    def doMount(self):
        self.mountpoint = tempfile.mkdtemp()
        os.system("mount -o loop %s %s" % (self.path, self.mountpoint))

    def tearDown(self):
        os.system("umount %s" % self.mountpoint)
        os.rmdir(self.mountpoint)
        RequiresDevice.tearDown(self)

# Base class for any test case that requires a _ped.Partition.
class RequiresPartition(RequiresDisk):
    def setUp(self):
        RequiresDisk.setUp(self)
        self._part = _ped.Partition(disk=self._disk, type=_ped.PARTITION_NORMAL,
                                    start=0, end=100, fs_type=_ped.file_system_type_get("ext2"))

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
