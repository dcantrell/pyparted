#!/usr/bin/python
#
# Test cases for the methods in the parted.disk module itself
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

import parted
import unittest

from baseclass import *

# One class per method, multiple tests per class.  For these simple methods,
# that seems like good organization.  More complicated methods may require
# multiple classes and their own test suite.
class DiskNewTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskGetSetTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskClobberTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskDuplicateTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskDestroyTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskCommitTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskCommitToDeviceTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskCommitToOSTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskCheckTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskSupportsFeatureTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskAddPartitionTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskRemovePartitionTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskDeletePartitionTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskDeleteAllPartitionsTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskSetPartitionGeometryTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskMaximizePartitionTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskCalculateMaxPartitionGeometryTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskMinimizeExtendedPartitionTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskGetPartitionBySectorTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskGetMaxLogicalPartitionsTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskGetMaxSupportedPartitionCountTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskMaxPartitionLengthTestCase(RequiresDisk):
    def runTest(self):
        # This test assumes an MSDOS label as given by RequiresDisk
        self.assertEquals(self._disk.maxPartitionLength, 4294967295L)

class DiskMaxPartitionStartSectorTestCase(RequiresDisk):
    def runTest(self):
        # This test assumes an MSDOS label as given by RequiresDisk
        self.assertEquals(self._disk.maxPartitionStartSector, 4294967295L)

class DiskGetFlagTestCase(RequiresDisk):
    def runTest(self):
        flag = self._disk.getFlag(parted.DISK_CYLINDER_ALIGNMENT)
        self.assertTrue(isinstance(flag, bool))

class DiskSetFlagTestCase(RequiresDisk):
    def runTest(self):
        # This test assumes an MSDOS label as given by RequiresDisk
        self._disk.setFlag(parted.DISK_CYLINDER_ALIGNMENT)
        flag = self._disk.getFlag(parted.DISK_CYLINDER_ALIGNMENT)
        self.assertEquals(flag, True)

class DiskUnsetFlagTestCase(RequiresDisk):
    def runTest(self):
        # This test assumes an MSDOS label as given by RequiresDisk
        self._disk.unsetFlag(parted.DISK_CYLINDER_ALIGNMENT)
        flag = self._disk.getFlag(parted.DISK_CYLINDER_ALIGNMENT)
        self.assertEquals(flag, False)

class DiskIsFlagAvailableTestCase(RequiresDisk):
    def runTest(self):
        # This test assumes an MSDOS label as given by RequiresDisk
        available = self._disk.isFlagAvailable(parted.DISK_CYLINDER_ALIGNMENT)
        self.assertEquals(available, True)

class DiskGetExtendedPartitionTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskGetLogicalPartitionsTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskGetPrimaryPartitionsTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskGetRaidPartitionsTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskGetLVMPartitionsTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskGetFreeSpaceRegionsTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskGetFreeSpacePartitionsTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskGetFirstPartitionTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskGetPartitionByPathTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskGetPedDiskTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class DiskStrTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

# And then a suite to hold all the test cases for this module.
def suite():
    suite = unittest.TestSuite()
    suite.addTest(DiskNewTestCase())
    suite.addTest(DiskGetSetTestCase())
    suite.addTest(DiskClobberTestCase())
    suite.addTest(DiskDuplicateTestCase())
    suite.addTest(DiskDestroyTestCase())
    suite.addTest(DiskCommitTestCase())
    suite.addTest(DiskCommitToDeviceTestCase())
    suite.addTest(DiskCommitToOSTestCase())
    suite.addTest(DiskCheckTestCase())
    suite.addTest(DiskSupportsFeatureTestCase())
    suite.addTest(DiskAddPartitionTestCase())
    suite.addTest(DiskRemovePartitionTestCase())
    suite.addTest(DiskDeletePartitionTestCase())
    suite.addTest(DiskDeleteAllPartitionsTestCase())
    suite.addTest(DiskSetPartitionGeometryTestCase())
    suite.addTest(DiskMaximizePartitionTestCase())
    suite.addTest(DiskCalculateMaxPartitionGeometryTestCase())
    suite.addTest(DiskMinimizeExtendedPartitionTestCase())
    suite.addTest(DiskGetPartitionBySectorTestCase())
    suite.addTest(DiskGetMaxLogicalPartitionsTestCase())
    suite.addTest(DiskGetMaxSupportedPartitionCountTestCase())
    suite.addTest(DiskMaxPartitionLengthTestCase())
    suite.addTest(DiskMaxPartitionStartSectorTestCase())
    suite.addTest(DiskGetFlagTestCase())
    suite.addTest(DiskSetFlagTestCase())
    suite.addTest(DiskUnsetFlagTestCase())
    suite.addTest(DiskIsFlagAvailableTestCase())
    suite.addTest(DiskGetExtendedPartitionTestCase())
    suite.addTest(DiskGetLogicalPartitionsTestCase())
    suite.addTest(DiskGetPrimaryPartitionsTestCase())
    suite.addTest(DiskGetRaidPartitionsTestCase())
    suite.addTest(DiskGetLVMPartitionsTestCase())
    suite.addTest(DiskGetFreeSpaceRegionsTestCase())
    suite.addTest(DiskGetFreeSpacePartitionsTestCase())
    suite.addTest(DiskGetFirstPartitionTestCase())
    suite.addTest(DiskGetPartitionByPathTestCase())
    suite.addTest(DiskGetPedDiskTestCase())
    suite.addTest(DiskStrTestCase())
    return suite

s = suite()
unittest.TextTestRunner(verbosity=2).run(s)
