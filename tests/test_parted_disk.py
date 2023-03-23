#
# Test cases for the methods in the parted.disk module itself
#
# Copyright The pyparted Project Authors
# SPDX-License-Identifier: GPL-2.0-or-later
#

import parted
import unittest

from tests.baseclass import RequiresDisk

# One class per method, multiple tests per class.  For these simple methods,
# that seems like good organization.  More complicated methods may require
# multiple classes and their own test suite.
@unittest.skip("Unimplemented test case.")
class DiskNewTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DiskGetSetTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DiskClobberTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DiskDuplicateTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DiskDestroyTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DiskCommitTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DiskCommitToDeviceTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DiskCommitToOSTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DiskCheckTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DiskSupportsFeatureTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


class DiskAddPartitionTestCase(RequiresDisk):
    """
    addPartition should return True if partition is added successfully(even
    without committing)
    """

    def runTest(self):
        self.disk.setFlag(parted.DISK_CYLINDER_ALIGNMENT)

        length = 100
        geom = parted.Geometry(self.device, start=100, length=length)
        part = parted.Partition(self.disk, parted.PARTITION_NORMAL, geometry=geom)
        constraint = parted.Constraint(exactGeom=geom)
        self.assertTrue(self.disk.addPartition(part, constraint))


@unittest.skip("Unimplemented test case.")
class DiskRemovePartitionTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DiskDeletePartitionTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DiskDeleteAllPartitionsTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DiskSetPartitionGeometryTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DiskMaximizePartitionTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DiskCalculateMaxPartitionGeometryTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DiskMinimizeExtendedPartitionTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DiskGetPartitionBySectorTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


class DiskGetMaxSupportedPartitionCountTestCase(RequiresDisk):
    """
    maxSupportedPartitionCount should return value 64, based on default
    value MAX_NUM_PARTS(parted/libparted/arch/linux.c) applied if it cannot
    find value in /sys/block/DEV/ext_range (RequiresDisk implies there is
    no ext_range value). Also see testcase
    DiskGetMaxSupportedPartitionCountTestCase in tests/test__ped_disk ,
    which tests value returned by source C function defined in module _ped
    """

    def runTest(self):
        self.assertEqual(self.disk.maxSupportedPartitionCount, 64)


class DiskMaxPartitionLengthTestCase(RequiresDisk):
    def runTest(self):
        # This test assumes an MSDOS label as given by RequiresDisk
        self.assertEqual(self.disk.maxPartitionLength, 4294967295)


class DiskMaxPartitionStartSectorTestCase(RequiresDisk):
    def runTest(self):
        # This test assumes an MSDOS label as given by RequiresDisk
        self.assertEqual(self.disk.maxPartitionStartSector, 4294967295)


class DiskGetFlagTestCase(RequiresDisk):
    def runTest(self):
        flag = self.disk.getFlag(parted.DISK_CYLINDER_ALIGNMENT)
        self.assertIsInstance(flag, bool)


class DiskSetFlagTestCase(RequiresDisk):
    def runTest(self):
        # This test assumes an MSDOS label as given by RequiresDisk
        self.disk.setFlag(parted.DISK_CYLINDER_ALIGNMENT)
        flag = self.disk.getFlag(parted.DISK_CYLINDER_ALIGNMENT)
        self.assertEqual(flag, True)


class DiskUnsetFlagTestCase(RequiresDisk):
    def runTest(self):
        # This test assumes an MSDOS label as given by RequiresDisk
        self.disk.unsetFlag(parted.DISK_CYLINDER_ALIGNMENT)
        flag = self.disk.getFlag(parted.DISK_CYLINDER_ALIGNMENT)
        self.assertEqual(flag, False)


class DiskIsFlagAvailableTestCase(RequiresDisk):
    def runTest(self):
        # This test assumes an MSDOS label as given by RequiresDisk
        available = self.disk.isFlagAvailable(parted.DISK_CYLINDER_ALIGNMENT)
        self.assertEqual(available, True)


@unittest.skip("Unimplemented test case.")
class DiskGetExtendedPartitionTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DiskGetLogicalPartitionsTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DiskGetPrimaryPartitionsTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DiskGetRaidPartitionsTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DiskGetLVMPartitionsTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DiskGetFreeSpaceRegionsTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DiskGetFreeSpacePartitionsTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DiskGetFirstPartitionTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DiskGetPartitionByPathTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DiskGetPedDiskTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DiskStrTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")
