#!/usr/bin/python
#
# Test cases for the methods in the parted module itself
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
import unittest
from baseclass import *

# One class per method, multiple tests per class.  For these simple methods,
# that seems like good organization.  More complicated methods may require
# multiple classes and their own test suite.
class GetDeviceTestCase(RequiresDeviceNode):
    def runTest(self):
        # Check that a DiskException is raised for an invalid path
        self.assertRaises(parted.DeviceException, parted.getDevice, None)
        self.assertRaises(parted.DeviceException, parted.getDevice, "")
        self.assertRaises(parted.DeviceException, parted.getDevice, "/dev/whatever")

        # Check that we get a parted.Device back
        self.assert_(isinstance(parted.getDevice(self.path), parted.Device))

        # Make sure the device node paths match
        self.assertTrue(parted.getDevice(self.path).path == self.path)

class GetAllDevicesTestCase(unittest.TestCase):
    def setUp(self):
        self.devices = parted.getAllDevices()

    def runTest(self):
        # Check self.devices and see that it's a list
        self.assertTrue(type(self.devices).__name__ == 'list')

        # And make sure each element of the list is a parted.Device
        map(lambda s: self.assert_(isinstance(s, parted.Device)), self.devices)

class ProbeForSpecificFileSystemTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class ProbeFileSystemTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class FreshDiskTestCase(RequiresDevice):
    def runTest(self):
        # Make sure we get SyntaxError when using an invalid disk type
        self.assertRaises(SyntaxError, parted.freshDisk, self._device, 'cheese')
        self.assertRaises(SyntaxError, parted.freshDisk, self._device, 'crackers')

        # Create a new disk for each disk type key, verify each one
        for key in parted.diskType.keys():
            disk = parted.freshDisk(self._device, key)
            self.assert_(isinstance(disk, parted.Disk))
            self.assertTrue(disk.type == key)

        # Create a new disk each disk type value, verify each one
        for value in parted.diskType.values():
            disk = parted.freshDisk(self._device, value)
            self.assert_(isinstance(disk, parted.Disk))
            self.assertTrue(parted.diskType[disk.type] == value)

class IsAlignToCylindersTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class ToggleAlignToCylindersTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class VersionTestCase(unittest.TestCase):
    def runTest(self):
        ver = parted.version()
        self.assertTrue(ver['libparted'] == _ped.libparted_version())
        self.assertTrue(ver['pyparted'] == _ped.pyparted_version())

# And then a suite to hold all the test cases for this module.
def suite():
    suite = unittest.TestSuite()
    suite.addTest(GetDeviceTestCase())
    suite.addTest(GetAllDevicesTestCase())
    suite.addTest(ProbeForSpecificFileSystemTestCase())
    suite.addTest(ProbeFileSystemTestCase())
    suite.addTest(FreshDiskTestCase())
    suite.addTest(IsAlignToCylindersTestCase())
    suite.addTest(ToggleAlignToCylindersTestCase())
    suite.addTest(VersionTestCase())
    return suite

s = suite()
unittest.TextTestRunner(verbosity=2).run(s)
