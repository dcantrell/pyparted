#
# Test cases for the methods in the parted.partition module itself
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

import parted
import unittest

from tests.baseclass import RequiresDisk

# One class per method, multiple tests per class.  For these simple methods,
# that seems like good organization.  More complicated methods may require
# multiple classes and their own test suite.
@unittest.skip("Unimplemented test case.")
class PartitionNewTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class PartitionGetSetTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class PartitionGetFlagTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class PartitionSetFlagTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class PartitionUnsetFlagTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class PartitionGetMaxGeometryTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class PartitionIsFlagAvailableTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class PartitionNextPartitionTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class PartitionGetSizeTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class PartitionGetLengthTestCase(RequiresDisk):
    def runTest(self):
        length = 100
        geom = parted.Geometry(self.device, start=100, length=length)
        part = parted.Partition(self.disk, parted.PARTITION_NORMAL, geometry=geom)
        constraint = parted.Constraint(exactGeom=geom)
        self.disk.addPartition(part, constraint)
        self.disk.commit()
        part = self.disk.partitions[0]

        self.assertEqual(part.getLength(), part.geometry.length)
        self.assertEqual(part.getLength(), length)

@unittest.skip("Unimplemented test case.")
class PartitionGetFlagsAsStringTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class PartitionGetMaxAvailableSizeTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class PartitionGetDeviceNodeNameTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class PartitionGetPedPartitionTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class PartitionStrTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

# And then a suite to hold all the test cases for this module.
def makeSuite():
    suite = unittest.TestSuite()
    suite.addTest(PartitionNewTestCase())
    suite.addTest(PartitionGetSetTestCase())
    suite.addTest(PartitionGetFlagTestCase())
    suite.addTest(PartitionSetFlagTestCase())
    suite.addTest(PartitionUnsetFlagTestCase())
    suite.addTest(PartitionGetMaxGeometryTestCase())
    suite.addTest(PartitionIsFlagAvailableTestCase())
    suite.addTest(PartitionNextPartitionTestCase())
    suite.addTest(PartitionGetSizeTestCase())
    suite.addTest(PartitionGetLengthTestCase())
    suite.addTest(PartitionGetFlagsAsStringTestCase())
    suite.addTest(PartitionGetMaxAvailableSizeTestCase())
    suite.addTest(PartitionGetDeviceNodeNameTestCase())
    suite.addTest(PartitionGetPedPartitionTestCase())
    suite.addTest(PartitionStrTestCase())
    return suite

s = makeSuite()
if __name__ == "__main__":
    unittest.main(defaultTest='s', verbosity=2)
