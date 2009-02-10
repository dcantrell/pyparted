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

# One class per method, multiple tests per class.  For these simple methods,
# that seems like good organization.  More complicated methods may require
# multiple classes and their own test suite.
class DiskNewTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskGetSetTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskClobberTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskDuplicateTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskDestroyTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskCommitTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskCommitToDeviceTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskCommitToOSTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskCheckTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskSupportsFeatureTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskAddPartitionTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskRemovePartitionTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskDeletePartitionTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskDeleteAllPartitionsTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskSetPartitionGeometryTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskMaximizePartitionTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskCalculateMaxPartitionGeometryTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskMinimizeExtendedPartitionTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskGetPartitionBySectorTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskGetMaxLogicalPartitionsTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskGetExtendedPartitionTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskGetLogicalPartitionsTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskGetPrimaryPartitionsTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskGetRaidPartitionsTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskGetLVMPartitionsTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskGetFreeSpaceRegionsTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskGetFreeSpacePartitionsTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskGetFirstPartitionTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskGetPartitionByPathTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

class DiskGetPedDiskTestCase(unittest.TestCase):
    def runTest(self):
        # TODO

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
    return suite

s = suite()
unittest.TextTestRunner(verbosity=2).run(s)
