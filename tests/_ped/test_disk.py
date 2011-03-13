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
# Red Hat Author(s): Chris Lumens <clumens@redhat.com>
#                    David Cantrell <dcantrell@redhat.com>
#

import _ped
import unittest

from baseclass import *

# One class per method, multiple tests per class.  For these simple methods,
# that seems like good organization.  More complicated methods may require
# multiple classes and their own test suite.
class DiskNewUnlabeledTestCase(RequiresDevice):
    def runTest(self):
        self.assertRaises(_ped.DiskLabelException, _ped.Disk, self._device)

class DiskNewLabeledTestCase(RequiresLabeledDevice):
    def runTest(self):
        result = _ped.Disk(self._device)
        self.assertTrue(isinstance(result, _ped.Disk))
        self.assertEquals(result.type.name, 'msdos')

@unittest.skip("Unimplemented test case.")
class DiskGetSetTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class DiskClobberTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class DiskClobberExcludeTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class DiskDuplicateTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class DiskDestroyTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        self.fail("Unimplemented test case.")

class DiskCommitTestCase(RequiresDisk):
    def runTest(self):
        self.assertTrue(self._disk.commit())

class DiskCommitToDevTestCase(RequiresDisk):
    def runTest(self):
        self.assertTrue(self._disk.commit_to_dev())

class DiskCommitToOsTestCase(RequiresDisk):
    def runTest(self):
        self.assertTrue(self._disk.commit_to_os())

class DiskCheckTestCase(RequiresDisk):
    def runTest(self):
        self.assertTrue(self._disk.check())

@unittest.skip("Unimplemented test case.")
class DiskPrintTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        self.fail("Unimplemented test case.")

class DiskGetPrimaryPartitionCountTestCase(RequiresDisk):
    def runTest(self):
        # XXX: this could probably test more
        self.assertEquals(self._disk.get_primary_partition_count(), 0)

class DiskGetLastPartitionNumTestCase(RequiresDisk):
    def runTest(self):
        # XXX: this could probably test more
        self.assertEquals(self._disk.get_last_partition_num(), -1)

class DiskGetMaxPrimaryPartitionCountTestCase(RequiresDisk):
    def runTest(self):
        self.assertEquals(self._disk.get_max_primary_partition_count(), 4)

class DiskGetMaxSupportedPartitionCountTestCase(RequiresDisk):
    def runTest(self):
        self.assertEquals(self._disk.get_max_supported_partition_count(), 16)

class DiskGetPartitionAlignmentTestCase(RequiresDisk):
    def runTest(self):
        alignment = self._disk.get_partition_alignment()
        self.assertTrue(isinstance(alignment, _ped.Alignment))
        # These 2 tests assume an MSDOS label as given by RequiresDisk
        self.assertEquals(alignment.offset, 0)
        self.assertEquals(alignment.grain_size, 1)

class DiskMaxPartitionLengthTestCase(RequiresDisk):
    def runTest(self):
        # This test assumes an MSDOS label as given by RequiresDisk
        self.assertEquals(self._disk.max_partition_length(), 4294967295L)

class DiskMaxPartitionStartSectorTestCase(RequiresDisk):
    def runTest(self):
        # This test assumes an MSDOS label as given by RequiresDisk
        self.assertEquals(self._disk.max_partition_start_sector(), 4294967295L)

class DiskSetFlagTestCase(RequiresDisk):
    def runTest(self):
        # These 2 tests assume an MSDOS label as given by RequiresDisk
        self._disk.set_flag(_ped.DISK_CYLINDER_ALIGNMENT, 1)
        self.assertEquals(self._disk.get_flag(_ped.DISK_CYLINDER_ALIGNMENT), True)
        self._disk.set_flag(_ped.DISK_CYLINDER_ALIGNMENT, 0)
        self.assertEquals(self._disk.get_flag(_ped.DISK_CYLINDER_ALIGNMENT), False)

class DiskGetFlagTestCase(RequiresDisk):
    def runTest(self):
        flag = self._disk.get_flag(_ped.DISK_CYLINDER_ALIGNMENT)
        self.assertTrue(isinstance(flag, bool))

class DiskIsFlagAvailableTestCase(RequiresDisk):
    def runTest(self):
        # We don't know which flags should be available and which shouldn't,
        # but we can at least check that there aren't any tracebacks from
        # trying all of the valid ones.
        for flag in [_ped.DISK_CYLINDER_ALIGNMENT]:
            self.assertTrue(isinstance(self._disk.is_flag_available(flag), bool))

        # However, an invalid flag should definitely not be available.
        self.assertFalse(self._disk.is_flag_available(1000))

@unittest.skip("Unimplemented test case.")
class DiskAddPartitionTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class DiskRemovePartitionTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class DiskDeletePartitionTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class DiskDeleteAllTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class DiskSetPartitionGeomTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class DiskMaxmimzePartitionTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class DiskGetMaxPartitionGeoemtryTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class DiskMinimizeExtendedPartitionTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class DiskNextPartitionTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class DiskGetPartitionTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class DiskGetPartitionBySectorTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        self.fail("Unimplemented test case.")

class DiskExtendedPartitionTestCase(RequiresDisk):
    def runTest(self):
        self.assertRaises(_ped.PartitionException,
                          self._disk.extended_partition)

class DiskStrTestCase(RequiresDisk):
    def runTest(self):
        expected = "_ped.Disk instance --\n  dev: %s  type: %s" % \
                   (repr(self._disk.dev), repr(self._disk.type),)
        self.assertEquals(expected, str(self._disk))

# And then a suite to hold all the test cases for this module.
def suite():
    suite = unittest.TestSuite()
    suite.addTest(DiskNewUnlabeledTestCase())
    suite.addTest(DiskNewLabeledTestCase())
    suite.addTest(DiskGetSetTestCase())
    suite.addTest(DiskClobberTestCase())
    suite.addTest(DiskClobberExcludeTestCase())
    suite.addTest(DiskDuplicateTestCase())
    suite.addTest(DiskDestroyTestCase())
    suite.addTest(DiskCommitTestCase())
    suite.addTest(DiskCommitToDevTestCase())
    suite.addTest(DiskCommitToOsTestCase())
    suite.addTest(DiskCheckTestCase())
    suite.addTest(DiskPrintTestCase())
    suite.addTest(DiskGetPrimaryPartitionCountTestCase())
    suite.addTest(DiskGetLastPartitionNumTestCase())
    suite.addTest(DiskGetMaxPrimaryPartitionCountTestCase())
    suite.addTest(DiskGetMaxSupportedPartitionCountTestCase())
    suite.addTest(DiskGetPartitionAlignmentTestCase())
    suite.addTest(DiskMaxPartitionLengthTestCase())
    suite.addTest(DiskMaxPartitionStartSectorTestCase())
    suite.addTest(DiskSetFlagTestCase())
    suite.addTest(DiskGetFlagTestCase())
    suite.addTest(DiskIsFlagAvailableTestCase())
    suite.addTest(DiskAddPartitionTestCase())
    suite.addTest(DiskRemovePartitionTestCase())
    suite.addTest(DiskDeletePartitionTestCase())
    suite.addTest(DiskDeleteAllTestCase())
    suite.addTest(DiskSetPartitionGeomTestCase())
    suite.addTest(DiskMaxmimzePartitionTestCase())
    suite.addTest(DiskGetMaxPartitionGeoemtryTestCase())
    suite.addTest(DiskMinimizeExtendedPartitionTestCase())
    suite.addTest(DiskNextPartitionTestCase())
    suite.addTest(DiskGetPartitionTestCase())
    suite.addTest(DiskGetPartitionBySectorTestCase())
    suite.addTest(DiskExtendedPartitionTestCase())
    suite.addTest(DiskStrTestCase())
    return suite

s = suite()
if __name__ == "__main__":
    unittest.main(defaultTest='s', verbosity=2)
