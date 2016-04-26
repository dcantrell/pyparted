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

class PartitionNewTestCase(RequiresDisk):
    """
        PartitionNew tests if parted.Partition:
        1) raises user defined exception if called without arguments
        2) if parted.Partition is instantiable with argument fs
        3) if parted.Partition is instantiable without argument fs
    """
    def setUp(self):
        super(PartitionNewTestCase, self).setUp()
        self.geom = parted.Geometry(self.device, start=100, length=100)
        self.fs = parted.FileSystem(type='ext2', geometry=self.geom)
        self.part = parted.Partition(self.disk, parted.PARTITION_NORMAL,
                                geometry=self.geom, fs=self.fs)

    def runTest(self):
        # Check that not passing args to parted.Partition.__init__ is caught.
        with self.assertRaises((parted.PartitionException,)):
            parted.Partition()

        self.assertIsInstance(self.part, parted.Partition)
        # You don't need to pass a filesystem type at all, since this partition
        # might be FREESPACE or METADATA.
        part_nofs = parted.Partition(self.disk, parted.PARTITION_NORMAL,
                                geometry=self.geom)
        self.assertIsInstance(part_nofs, parted.Partition)

class PartitionGetSetTestCase(PartitionNewTestCase):
    """
        PartitionGetSet tests "part" instance
        of class parted.Partition(created in baseclass)
        for basic get and set operations on its attributes.
    """
    def runTest(self):
        # Test that passing the kwargs to __init__ works.
        self.assertEqual(self.part.disk, self.disk)
        self.assertIsInstance(self.part.geometry, parted.Geometry)
        self.assertEqual(self.part.type, parted.PARTITION_NORMAL)
        self.assertEqual(self.part.fileSystem.type, "ext2")

        # Test that setting the RW attributes directly works.
        self.part.type = parted.PARTITION_EXTENDED
        self.assertEqual(self.part.type, parted.PARTITION_EXTENDED)

        # Test that setting the RO attributes directly doesn't work.
        exn = (AttributeError, TypeError)
        with self.assertRaises(exn):
            self.part.number = 1
        with self.assertRaises(exn):
            self.part.active = True
        with self.assertRaises(exn):
            self.part.name = "blah"
        with self.assertRaises(exn):
            self.part.type = "blah"
        with self.assertRaises(exn):
            self.part.geometry = parted.Geometry(self.device, start=10, length=20)
        with self.assertRaises(exn):
            self.part.fileSystem = parted.FileSystem(type='ext4', geometry=self.geom)
        with self.assertRaises(exn):
            self.part.disk = self.disk

        # Check that looking for invalid attributes fails properly.
        with self.assertRaises((AttributeError)):
            self.part.blah

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
