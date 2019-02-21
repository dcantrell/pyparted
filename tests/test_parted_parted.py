#
# Test cases for the methods in the parted module itself
#
# Copyright (C) 2009-2011  Red Hat, Inc.
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

from __future__ import division

import _ped
import parted
import unittest
from tests.baseclass import RequiresDevice, RequiresDeviceNode

# One class per method, multiple tests per class.  For these simple methods,
# that seems like good organization.  More complicated methods may require
# multiple classes and their own test suite.
class FormatBytesTestCase(unittest.TestCase):
    def runTest(self):
        self.assertRaises(SyntaxError, parted.formatBytes, 57, "GIB")
        self.assertEqual(1e-24, parted.formatBytes(1, "YB"))
        self.assertEqual(1/2**80, parted.formatBytes(1, "YiB"))
        self.assertEqual(1, parted.formatBytes(1, 'B'))
        self.assertEqual(1, parted.formatBytes(1e24, 'YB'))
        self.assertEqual(1, parted.formatBytes(2**80, 'YiB'))

class BytesToSectorsTestCase(unittest.TestCase):
    def runTest(self):
        self.assertRaises(SyntaxError, parted.sizeToSectors, 9, "yb", 1)
        self.assertEqual(int(parted.sizeToSectors(7777.0, "B", 512)),
                             parted.sizeToSectors(7777.0, "B", 512))
        self.assertEqual(parted.sizeToSectors(1000, "B", 512), 2)

class GetLabelsTestCase(unittest.TestCase):
    def runTest(self):
        self.assertGreater(len(parted.getLabels()), 0)
        self.assertSetEqual(parted.getLabels('ppcc'), set())
        self.assertSetEqual(parted.getLabels('sparc6'), set())
        self.assertSetEqual(parted.getLabels('i586'), {'gpt', 'msdos'})
        self.assertSetEqual(parted.getLabels('s390'), {'dasd', 'msdos'})
        self.assertSetEqual(parted.getLabels('s390x'), {'dasd', 'msdos'})
        self.assertSetEqual(parted.getLabels('sparc'), {'sun'})
        self.assertSetEqual(parted.getLabels('sparc64'), {'sun'})
        self.assertSetEqual(parted.getLabels('ppc'), {'amiga', 'gpt', 'mac', 'msdos'})
        self.assertSetEqual(parted.getLabels('ppc64'), {'amiga', 'gpt', 'mac', 'msdos'})
        self.assertSetEqual(parted.getLabels('ppc64le'), {'gpt', 'msdos'})
        self.assertSetEqual(parted.getLabels('alpha'), {'bsd', 'msdos'})
        self.assertSetEqual(parted.getLabels('ia64'), {'gpt', 'msdos'})
        self.assertSetEqual(parted.getLabels('aarch64'), {'gpt', 'msdos'})
        self.assertSetEqual(parted.getLabels('armv7l'), {'gpt', 'msdos'})
        self.assertSetEqual(parted.getLabels('riscv32'), {'gpt', 'msdos'})
        self.assertSetEqual(parted.getLabels('riscv64'), {'gpt', 'msdos'})

class GetDeviceTestCase(RequiresDeviceNode):
    def runTest(self):
        # Check that a DiskException is raised for an invalid path
        self.assertRaises(parted.DeviceException, parted.getDevice, None)
        self.assertRaises(parted.IOException, parted.getDevice, "")
        self.assertRaises(parted.IOException, parted.getDevice, "/dev/whatever")

        # Check that we get a parted.Device back
        self.assertIsInstance(parted.getDevice(self.path), parted.Device)

        # Make sure the device node paths match
        self.assertEqual(parted.getDevice(self.path).path, self.path)

class GetAllDevicesTestCase(unittest.TestCase):
    def setUp(self):
        self.devices = parted.getAllDevices()

    def runTest(self):
        # Check self.devices and see that it's a list
        self.assertEqual(type(self.devices).__name__, 'list')

        # And make sure each element of the list is a parted.Device
        for dev in self.devices:
            self.assertIsInstance(dev, parted.Device)

@unittest.skip("Unimplemented test case.")
class ProbeForSpecificFileSystemTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class ProbeFileSystemTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class FreshDiskTestCase(RequiresDevice):
    def runTest(self):
        # Make sure we get SyntaxError when using an invalid disk type
        self.assertRaises(KeyError, parted.freshDisk, self.device, 'cheese')
        self.assertRaises(KeyError, parted.freshDisk, self.device, 'crackers')

        # Create a new disk for each disk type key, verify each one
        # XXX: Skip over dvh for now (SGI disk label), which doesn't seem to have
        # working libparted support.  If anyone with an SGI cares, patches welcome.
        for key in parted.diskType.keys():
            if key in ['dvh', 'aix']:
                continue
            disk = parted.freshDisk(self.device, key)
            self.assertIsInstance(disk, parted.Disk)
            self.assertEqual(disk.type, key)

        # Create a new disk each disk type value, verify each one
        for value in parted.diskType.values():
            if value.name in ['dvh', 'aix']:
                continue
            disk = parted.freshDisk(self.device, value)
            self.assertIsInstance(disk, parted.Disk)
            self.assertEqual(parted.diskType[disk.type], value)

@unittest.skip("Unimplemented test case.")
class IsAlignToCylindersTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class ToggleAlignToCylindersTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class VersionTestCase(unittest.TestCase):
    def runTest(self):
        ver = parted.version()
        self.assertEqual(ver['libparted'], _ped.libparted_version())
        self.assertEqual(ver['pyparted'], _ped.pyparted_version())
