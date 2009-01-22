#!/usr/bin/python
#
# Test cases for the methods in the _ped module itself - just the pyunit
# and pynatmath files.
#
# Copyright (C) 2008  Red Hat, Inc.
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
#
import _ped
import unittest
import os, tempfile

from baseclass import *

# One class per method, multiple tests per class.  For these simple methods,
# that seems like good organization.  More complicated methods may require
# multiple classes and their own test suite.
class FlagGetNameTestCase(unittest.TestCase):
    def runTest(self):
        for f in [_ped.PARTITION_BOOT, _ped.PARTITION_ROOT, _ped.PARTITION_SWAP,
                  _ped.PARTITION_HIDDEN, _ped.PARTITION_RAID, _ped.PARTITION_LVM,
                  _ped.PARTITION_LBA, _ped.PARTITION_HPSERVICE,
                  _ped.PARTITION_PALO, _ped.PARTITION_PREP,
                  _ped.PARTITION_MSFT_RESERVED]:
            self.assert_(_ped.flag_get_name(f) != "", "Could not get name for flag %s" % f)

        self.assertRaises(ValueError, _ped.flag_get_name, -1)
        self.assertRaises(ValueError, _ped.flag_get_name, 1000)

class FlagGetByNameTestCase(unittest.TestCase):
    def runTest(self):
        for f in ["boot", "root", "swap", "hidden", "raid", "lvm", "lba",
                  "hp-service", "palo", "prep", "msftres"]:
            self.assert_(_ped.flag_get_by_name(f) != "", "Could not get flag %s" % f)

        self.assert_(_ped.flag_get_by_name("nosuchflag") == 0)

class FlagNextTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class GreatestCommonDivisorTestCase(unittest.TestCase):
    def runTest(self):
        # Can't test cases where we pass a negative to greatest_common_divisor
        # because libparted will assert() on those and we'll abort.
        self.assertEqual(_ped.greatest_common_divisor(40, 0), 40)
        self.assertEqual(_ped.greatest_common_divisor(0, 40), 40)
        self.assertEqual(_ped.greatest_common_divisor(40, 10), 10)
        self.assertEqual(_ped.greatest_common_divisor(47, 19), 1)

class ConstraintNewFromMinMaxTestCase(RequiresDevice):
    def runTest(self):
        self.assertRaises(TypeError, _ped.constraint_new_from_min_max, None)

        # min is required to be within max, so test various combinations of
        # that not being the case.
        self.assertRaises(_ped.CreateException, _ped.constraint_new_from_min_max,
                          _ped.Geometry(self._device, 0, 10),
                          _ped.Geometry(self._device, 15, 25))
        self.assertRaises(_ped.CreateException, _ped.constraint_new_from_min_max,
                          _ped.Geometry(self._device, 10, 20),
                          _ped.Geometry(self._device, 15, 25))

        # Now test a correct call.
        min = _ped.Geometry(self._device, 10, 20)
        max = _ped.Geometry(self._device, 0, 30)
        constraint = _ped.constraint_new_from_min_max(min, max)

        self.assert_(isinstance(constraint, _ped.Constraint))
        self.assertTrue(constraint.is_solution(_ped.Geometry(self._device, 10, 20)))
        self.assertFalse(constraint.is_solution(_ped.Geometry(self._device, 11, 20)))
        self.assertTrue(constraint.is_solution(_ped.Geometry(self._device, 5, 25)))
        self.assertTrue(constraint.is_solution(_ped.Geometry(self._device, 0, 30)))
        self.assertFalse(constraint.is_solution(_ped.Geometry(self._device, 0, 35)))

class ConstraintNewFromMinTestCase(RequiresDevice):
    def runTest(self):
        self.assertRaises(TypeError, _ped.constraint_new_from_min, None)

        min = _ped.Geometry(self._device, 10, 20)
        constraint = _ped.constraint_new_from_min(min)

        self.assert_(isinstance(constraint, _ped.Constraint))
        self.assertTrue(constraint.is_solution(_ped.Geometry(self._device, 10, 20)))
        self.assertTrue(constraint.is_solution(_ped.Geometry(self._device, 5, 25)))
        self.assertFalse(constraint.is_solution(_ped.Geometry(self._device, 11, 19)))
        self.assertFalse(constraint.is_solution(_ped.Geometry(self._device, 15, 25)))

class ConstraintNewFromMaxTestCase(RequiresDevice):
    def runTest(self):
        self.assertRaises(TypeError, _ped.constraint_new_from_max, None)

        max = _ped.Geometry(self._device, 10, 20)
        constraint = _ped.constraint_new_from_max(max)

        self.assert_(isinstance(constraint, _ped.Constraint))
        self.assertTrue(constraint.is_solution(_ped.Geometry(self._device, 10, 20)))
        self.assertFalse(constraint.is_solution(_ped.Geometry(self._device, 5, 25)))
        self.assertTrue(constraint.is_solution(_ped.Geometry(self._device, 11, 19)))
        self.assertFalse(constraint.is_solution(_ped.Geometry(self._device, 15, 25)))

class ConstraintAnyTestCase(RequiresDevice):
    def runTest(self):
        self.assertRaises(TypeError, _ped.constraint_any, None)

        constraint = _ped.constraint_any(self._device)
        self.assert_(isinstance(constraint, _ped.Constraint))

        for testGeom in [_ped.Geometry(self._device, 0, 5),
                         _ped.Geometry(self._device, 10, 25),
                         _ped.Geometry(self._device, 0, 100)]:
            self.assertTrue(constraint.is_solution(testGeom))

class ConstraintExactTestCase(RequiresDevice):
    def runTest(self):
        geom = _ped.Geometry(self._device, 0, 100)

        self.assertRaises(TypeError, _ped.constraint_exact, None)

        constraint = _ped.constraint_exact(geom)
        self.assert_(isinstance(constraint, _ped.Constraint))

        for testGeom in [_ped.Geometry(self._device, 1, 100),
                         _ped.Geometry(self._device, 0, 99),
                         _ped.Geometry(self._device, 10, 20),
                         _ped.Geometry(self._device, 50, 101)]:
            self.assertFalse(constraint.is_solution(testGeom))

        self.assertTrue(constraint.is_solution(_ped.Geometry(self._device, 0, 100)))

class DeviceGetTestCase(RequiresDevice):
    def runTest(self):
        # Try getting the device we just made.
        self.assert_(isinstance(_ped.device_get(self.path), _ped.Device))

        # Try getting a device that doesn't exist.
        self.assertRaises(_ped.DiskException, _ped.device_get, "/blah/whatever")
        self.assertRaises(_ped.DiskException, _ped.device_get, "")
        self.assertRaises(_ped.DiskException, _ped.device_get, None)

class DeviceGetNextTestCase(unittest.TestCase, BuildList):
    def runTest(self):
        # Make sure there are some devices in the system first and then
        # make a list out of them.  That's easier to work with.
        _ped.device_probe_all()
        lst = self.getDeviceList(_ped.device_get_next)

        # Now the test cases.
        self.assert_(len(lst) > 0)
        self.assertRaises(TypeError, _ped.device_get_next, None)

        for ele in lst:
            self.assert_(isinstance(ele, _ped.Device))

        self.assertRaises(IndexError, _ped.device_get_next, lst[-1])

class DeviceProbeAllTestCase(RequiresDevice, BuildList):
    def runTest(self):
        # Since we inherit from RequiresDevice, we can test that the temp
        # device we created is in the results list.  I can't really think of
        # any other way to test this method except by getting a list of devices
        # via some other mechanism and comparing that to the device_probe_all
        # results.
        _ped.device_probe_all()
        lst = self.getDeviceList(_ped.device_get_next)

        self.assert_(len(lst) > 0)
        self.assert_(len(filter(lambda e: e.path.startswith("/tmp/temp-device-"), lst)) > 0)

class DeviceFreeAllTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class DiskTypeGetTestCase(unittest.TestCase):
    def runTest(self):
        for d in ["aix", "amiga", "bsd", "dvh", "gpt", "loop", "mac", "msdos",
                  "pc98","sun"]:
            self.assert_(_ped.disk_type_get(d) != "", "Could not get type %s" % d)

        self.assertRaises(_ped.UnknownTypeException, _ped.disk_type_get, "nosuch")

class DiskTypeGetNextTestCase(unittest.TestCase, BuildList):
    def runTest(self):
        lst = self.getDeviceList(_ped.disk_type_get_next)
        self.assert_(len(lst) > 0)
        self.assertRaises(TypeError, _ped.device_get_next, None)

        for ele in lst:
            self.assert_(isinstance(ele, _ped.DiskType))

        self.assertRaises(IndexError, _ped.disk_type_get_next, lst[-1])

class DivRoundToNearestTestCase(unittest.TestCase):
    def runTest(self):
        self.assertEqual(_ped.div_round_to_nearest(0, 100), 0)
        self.assertEqual(_ped.div_round_to_nearest(100, 5), 20)
        self.assertEqual(_ped.div_round_to_nearest(100, 6), 17)
        self.assertRaises(ZeroDivisionError, _ped.div_round_to_nearest, 100, 0)

class DivRoundUpTestCase(unittest.TestCase):
    def runTest(self):
        self.assertEqual(_ped.div_round_up(0, 100), 0)
        self.assertEqual(_ped.div_round_to_nearest(100, 5), 20)
        self.assertEqual(_ped.div_round_to_nearest(100, 6), 17)
        self.assertRaises(ZeroDivisionError, _ped.div_round_up, 100, 0)

class FileSystemProbeTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class FileSystemProbeSpecificTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class FileSystemTypeGetTestCase(unittest.TestCase):
    def runTest(self):
        for f in ["affs0", "amufs", "apfs1", "asfs", "ext2", "ext3", "fat16",
                  "fat32", "hfs", "hfs+", "hfsx", "hp-ufs", "jfs", "linux-swap",
                  "ntfs", "reiserfs", "sun-ufs", "xfs"]:
            self.assert_(isinstance(_ped.file_system_type_get(f), _ped.FileSystemType),
                         "Could not get fs type %s" % f)

        self.assertRaises(_ped.UnknownTypeException, _ped.file_system_type_get, "nosuch")

class FileSystemTypeGetNextTestCase(unittest.TestCase, BuildList):
    def runTest(self):
        lst = self.getDeviceList(_ped.file_system_type_get_next)
        self.assert_(len(lst) > 0)
        self.assertRaises(TypeError, _ped.file_system_type_get_next, None)

        for ele in lst:
            self.assert_(isinstance(ele, _ped.FileSystemType))

        self.assertRaises(IndexError, _ped.file_system_type_get_next, lst[-1])

class RoundDownToTestCase(unittest.TestCase):
    def runTest(self):
        self.assertEqual(_ped.round_down_to(0, 100), 0)
        self.assertEqual(_ped.round_down_to(100, 17), 85)
        self.assertEqual(_ped.round_down_to(100, -17), 85)
        self.assertRaises(ZeroDivisionError, _ped.round_down_to, 100, 0)

class RoundToNearestTestCase(unittest.TestCase):
    def runTest(self):
        self.assertEqual(_ped.round_to_nearest(0, 100), 0)
        self.assertEqual(_ped.round_to_nearest(100, 17), 102)
        self.assertEqual(_ped.round_to_nearest(100, -17), 68)
        self.assertRaises(ZeroDivisionError, _ped.round_to_nearest, 100, 0)

class RoundUpToTestCase(unittest.TestCase):
    def runTest(self):
        self.assertEqual(_ped.round_to_nearest(0, 100), 0)
        self.assertEqual(_ped.round_up_to(100, 17), 102)
        self.assertEqual(_ped.round_up_to(100, -17), 68)
        self.assertRaises(ZeroDivisionError, _ped.round_up_to, 100, 0)

class PartitionTypeGetNameTestCase(unittest.TestCase):
    def runTest(self):
        for t in [_ped.PARTITION_METADATA, _ped.PARTITION_FREESPACE,
                  _ped.PARTITION_EXTENDED, _ped.PARTITION_LOGICAL]:
            self.assert_(_ped.partition_type_get_name(t) != "", "Could not get name for flag %s" % t)

class UnitSetDefaultTestCase(unittest.TestCase):
    def setUp(self):
        self._initialDefault = _ped.unit_get_default()

    def tearDown(self):
        _ped.unit_set_default(self._initialDefault)

    def runTest(self):
        for v in [_ped.UNIT_BYTE, _ped.UNIT_CHS, _ped.UNIT_COMPACT,
                  _ped.UNIT_CYLINDER, _ped.UNIT_GIBIBYTE, _ped.UNIT_GIGABYTE,
                  _ped.UNIT_KIBIBYTE, _ped.UNIT_KILOBYTE, _ped.UNIT_MEBIBYTE,
                  _ped.UNIT_MEGABYTE, _ped.UNIT_PERCENT, _ped.UNIT_SECTOR,
                  _ped.UNIT_TEBIBYTE, _ped.UNIT_TERABYTE]:
            _ped.unit_set_default(v)
            self.assert_(_ped.unit_get_default() == v, "Could not set unit default to %s" % v)

        self.assertRaises(ValueError, _ped.unit_set_default, -1)
        self.assertRaises(ValueError, _ped.unit_set_default, 1000)

class UnitGetDefaultTestCase(unittest.TestCase):
    def runTest(self):
        self.assert_(_ped.unit_get_default() >= 0)

class UnitGetSizeTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class UnitGetNameTestCase(unittest.TestCase):
    def runTest(self):
        self.assert_(_ped.unit_get_name(_ped.UNIT_COMPACT) == "compact")
        self.assert_(_ped.unit_get_name(_ped.UNIT_MEGABYTE) == "MB")
        self.assertRaises(ValueError, _ped.unit_get_name, -1)
        self.assertRaises(ValueError, _ped.unit_get_name, 1000)

class UnitGetByNameTestCase(unittest.TestCase):
    def runTest(self):
        self.assert_(_ped.unit_get_by_name("cyl") == _ped.UNIT_CYLINDER)
        self.assert_(_ped.unit_get_by_name("TB") == _ped.UNIT_TERABYTE)
        self.assertRaises(_ped.UnknownTypeException, _ped.unit_get_by_name, "blargle")

class UnitFormatCustomByteTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class UnitFormatByteTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class UnitFormatCustomTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class UnitFormatTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class UnitParseTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class UnitParseCustomTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

# And then a suite to hold all the test cases for this module.
def suite():
    suite = unittest.TestSuite()
    suite.addTest(ConstraintNewFromMinMaxTestCase())
    suite.addTest(ConstraintNewFromMinTestCase())
    suite.addTest(ConstraintNewFromMaxTestCase())
    suite.addTest(ConstraintAnyTestCase())
    suite.addTest(ConstraintExactTestCase())
    suite.addTest(DeviceGetTestCase())
    suite.addTest(DeviceGetNextTestCase())
    suite.addTest(DeviceProbeAllTestCase())
    suite.addTest(DeviceFreeAllTestCase())
    suite.addTest(DiskTypeGetTestCase())
    suite.addTest(DiskTypeGetNextTestCase())
    suite.addTest(DivRoundToNearestTestCase())
    suite.addTest(DivRoundUpTestCase())
    suite.addTest(FileSystemProbeTestCase())
    suite.addTest(FileSystemProbeSpecificTestCase())
    suite.addTest(FileSystemTypeGetTestCase())
    suite.addTest(FileSystemTypeGetNextTestCase())
    suite.addTest(FlagGetNameTestCase())
    suite.addTest(FlagGetByNameTestCase())
    suite.addTest(FlagNextTestCase())
    suite.addTest(GreatestCommonDivisorTestCase())
    suite.addTest(RoundDownToTestCase())
    suite.addTest(RoundToNearestTestCase())
    suite.addTest(RoundUpToTestCase())
    suite.addTest(PartitionTypeGetNameTestCase())
    suite.addTest(UnitSetDefaultTestCase())
    suite.addTest(UnitGetDefaultTestCase())
    suite.addTest(UnitGetSizeTestCase())
    suite.addTest(UnitGetNameTestCase())
    suite.addTest(UnitGetByNameTestCase())
    suite.addTest(UnitFormatCustomByteTestCase())
    suite.addTest(UnitFormatByteTestCase())
    suite.addTest(UnitFormatCustomTestCase())
    suite.addTest(UnitFormatTestCase())
    suite.addTest(UnitParseTestCase())
    suite.addTest(UnitParseCustomTestCase())
    return suite

s = suite()
unittest.TextTestRunner(verbosity=2).run(s)
