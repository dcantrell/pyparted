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
from tests.baseclass import RequiresGPTDisk

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

class PartitionGPTNewTestCase(RequiresGPTDisk):
    """
        Like PartitionNewTestCase but with a GPT-labeled disk image.
    """
    def setUp(self):
        super(PartitionGPTNewTestCase, self).setUp()
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
            self.part.type = "blah"
        with self.assertRaises(exn):
            self.part.disk = self.disk

        # DOS disklabels don't support names
        self.assertEqual(self.part.name, None)
        with self.assertRaises(parted.PartitionException):
            self.part.name = "root"

        # Check that looking for invalid attributes fails properly.
        with self.assertRaises((AttributeError)):
            print(self.part.blah)

class PartitionGetSetGPTTestCase(PartitionGPTNewTestCase):
    """
        Like PartitionGetSetTestCase, but with a GPT-labeled test
        image to work with rather than a DOS-labeled test image.
    """
    def runTest(self):
        # GPT labeled disks can support partition names
        print(self.part.name)
        self.part.name = "root"

class PartitionSetFlagTestCase(PartitionNewTestCase):
    """
        Method setFlag should return True, if flag was set to "on" state.
        Next few flag testcases will inherit from setUp.
        This testcase checks if method setFlag returns:
        1)correct bool when state of BOOT and RAID flags is set to "on"
        on partition.
        2)raises user defined exception on unavailable flag
        See parted/include/parted/disk.in.h for flag numbers.
        Partition flags are dependent on disklabel. In this case msdos label is
        used, see parted library libparted/labels/dos.c for flags availability.
    """
    def setUp(self):
        super(PartitionSetFlagTestCase, self).setUp()
        self.neg_msg = "Method returns unexpected bool value"
        self.flag_set = self.part.setFlag(1)
        self.flag_set1 = self.part.setFlag(5)

    def runTest(self):
        self.assertTrue(self.flag_set, self.neg_msg)
        self.assertTrue(self.flag_set1, self.neg_msg)
        with self.assertRaises((parted.PartitionException,)):
            self.part.setFlag(2)

class PartitionGetFlagTestCase(PartitionSetFlagTestCase):
    """
        Method getFlag should return correct bool value depending on flag
        setting(flag is off=>False; on=>True).
        This testcase checks if method getFlag returns:
        1)correct bools when checks state of BOOT(on), RAID(on) and LVM(off)
        flags.
    """
    def runTest(self):
        self.assertTrue(self.part.getFlag(1), self.neg_msg)
        self.assertTrue(self.part.getFlag(5), self.neg_msg)
        self.assertFalse(self.part.getFlag(6), self.neg_msg)

class PartitionUnsetFlagTestCase(PartitionSetFlagTestCase):
    """
        Method unsetFlag should set flag to "off" state and return True on
        success. PartitionException should be raised on error.
        This testcase checks if method unsetFlag returns:
        1)correct bool value when flag is unset and if flag is in state 'off'
        2)raises user defined exception on unavailable flag
    """
    def runTest(self):
        self.assertTrue(self.part.unsetFlag(1), self.neg_msg)
        self.assertFalse(self.part.getFlag(1), self.neg_msg)

        with self.assertRaises((parted.PartitionException,)):
            self.part.unsetFlag(2)

class PartitionIsFlagAvailableTestCase(PartitionNewTestCase):
    '''
        Method isFlagAvailable should return bool value whenever flag is
        available according to chosen disk label setting and disk proportions
        itself.
        This testcase checks if method isFlagAvailable returns:
        1)bool value without traceback
        2)false on nonexistent flag
        3)raises user defined exception when called on inactive partition
    '''
    def runTest(self):
        for f in ['PARTITION_BOOT', 'PARTITION_ROOT', 'PARTITION_SWAP',
                  'PARTITION_HIDDEN', 'PARTITION_RAID', 'PARTITION_LVM',
                  'PARTITION_LBA', 'PARTITION_HPSERVICE',
                  'PARTITION_PALO', 'PARTITION_PREP',
                  'PARTITION_MSFT_RESERVED',
                  'PARTITION_APPLE_TV_RECOVERY',
                  'PARTITION_BIOS_GRUB', 'PARTITION_DIAG',
                  'PARTITION_MSFT_DATA', 'PARTITION_IRST',
                  'PARTITION_ESP', 'PARTITION_NONFS']:
            if not hasattr(parted, f):
                continue
            attr = getattr(parted, f)
            self.assertIsInstance(self.part.isFlagAvailable(attr), bool)

        self.assertFalse(self.part.isFlagAvailable(1000))

        with self.assertRaises((parted.PartitionException,)):
            self.part = parted.Partition(self.disk, parted.PARTITION_FREESPACE,
            geometry=self.geom)
            self.part.isFlagAvailable(parted.PARTITION_BOOT)

class PartitionGetFlagsAsStringTestCase(PartitionSetFlagTestCase):
    '''
        Method getFlagsAsString should return all flags which are in state "on"
        as comma separated list.
    '''
    def runTest(self):
        self.assertEqual(self.part.getFlagsAsString(), 'boot, raid')

@unittest.skip("Unimplemented test case.")
class PartitionGetMaxGeometryTestCase(unittest.TestCase):
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
