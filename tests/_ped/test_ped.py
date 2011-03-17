#
# Test cases for the methods in the _ped module itself - just the pyunit
# and pynatmath files.
#
# Copyright (C) 2008, 2009  Red Hat, Inc.
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
import os
import tempfile

from baseclass import *

# One class per method, multiple tests per class.  For these simple methods,
# that seems like good organization.  More complicated methods may require
# multiple classes and their own test suite.
class PartitionFlagGetNameTestCase(unittest.TestCase):
    def runTest(self):
        for f in [_ped.PARTITION_BOOT, _ped.PARTITION_ROOT, _ped.PARTITION_SWAP,
                  _ped.PARTITION_HIDDEN, _ped.PARTITION_RAID, _ped.PARTITION_LVM,
                  _ped.PARTITION_LBA, _ped.PARTITION_HPSERVICE,
                  _ped.PARTITION_PALO, _ped.PARTITION_PREP,
                  _ped.PARTITION_MSFT_RESERVED,
                  _ped.PARTITION_APPLE_TV_RECOVERY,
                  _ped.PARTITION_BIOS_GRUB, _ped.PARTITION_DIAG]:
            self.assertNotEquals(_ped.partition_flag_get_name(f), "", "Could not get name for flag %s" % f)

        self.assertRaises(ValueError, _ped.partition_flag_get_name, -1)
        self.assertRaises(ValueError, _ped.partition_flag_get_name, 1000)

class PartitionFlagGetByNameTestCase(unittest.TestCase):
    def runTest(self):
        for f in ["boot", "root", "swap", "hidden", "raid", "lvm", "lba",
                  "hp-service", "palo", "prep", "msftres", "bios_grub"]:
            self.assertNotEquals(_ped.partition_flag_get_by_name(f), "", "Could not get flag %s" % f)

        self.assertEquals(_ped.partition_flag_get_by_name("nosuchflag"), 0)

class PartitionFlagNextTestCase(unittest.TestCase):
    def runTest(self):
        # We should get TypeError when the parameter is invalid
        self.assertRaises(TypeError, _ped.partition_flag_next)
        self.assertRaises(TypeError, _ped.partition_flag_next, 'blah')

        # First flag is 0, keep getting flags until we loop back around
        # to zero.  Make sure each flag we get is an integer.
        flag = _ped.partition_flag_next(0)
        self.assertEquals(type(flag).__name__, 'int')

        while True:
            flag = _ped.partition_flag_next(flag)
            if not flag:
                break
            self.assertEquals(type(flag).__name__, 'int')

class DiskFlagGetNameTestCase(unittest.TestCase):
    def runTest(self):
        for f in [_ped.DISK_CYLINDER_ALIGNMENT]:
            self.assertNotEquals(_ped.disk_flag_get_name(f), "", "Could not get name for flag %s" % f)

        self.assertRaises(ValueError, _ped.disk_flag_get_name, -1)
        self.assertRaises(ValueError, _ped.disk_flag_get_name, 1000)

class DiskFlagGetByNameTestCase(unittest.TestCase):
    def runTest(self):
        for f in ["cylinder_alignment"]:
            self.assertNotEquals(_ped.disk_flag_get_by_name(f), 0, "Could not get flag %s" % f)

        self.assertEquals(_ped.disk_flag_get_by_name("nosuchflag"), 0)

class DiskFlagNextTestCase(unittest.TestCase):
    def runTest(self):
        # We should get TypeError when the parameter is invalid
        self.assertRaises(TypeError, _ped.disk_flag_next)
        self.assertRaises(TypeError, _ped.disk_flag_next, 'blah')

        # First flag is 0, keep getting flags until we loop back around
        # to zero.  Make sure each flag we get is an integer.
        flag = _ped.disk_flag_next(0)
        self.assertEquals(type(flag).__name__, 'int')

        while True:
            flag = _ped.disk_flag_next(flag)
            if not flag:
                break
            self.assertEquals(type(flag).__name__, 'int')

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

        self.assertTrue(isinstance(constraint, _ped.Constraint))
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

        self.assertTrue(isinstance(constraint, _ped.Constraint))
        self.assertTrue(constraint.is_solution(_ped.Geometry(self._device, 10, 20)))
        self.assertTrue(constraint.is_solution(_ped.Geometry(self._device, 5, 25)))
        self.assertFalse(constraint.is_solution(_ped.Geometry(self._device, 11, 19)))
        self.assertFalse(constraint.is_solution(_ped.Geometry(self._device, 15, 25)))

class ConstraintNewFromMaxTestCase(RequiresDevice):
    def runTest(self):
        self.assertRaises(TypeError, _ped.constraint_new_from_max, None)

        max = _ped.Geometry(self._device, 10, 20)
        constraint = _ped.constraint_new_from_max(max)

        self.assertTrue(isinstance(constraint, _ped.Constraint))
        self.assertTrue(constraint.is_solution(_ped.Geometry(self._device, 10, 20)))
        self.assertFalse(constraint.is_solution(_ped.Geometry(self._device, 5, 25)))
        self.assertTrue(constraint.is_solution(_ped.Geometry(self._device, 11, 19)))
        self.assertFalse(constraint.is_solution(_ped.Geometry(self._device, 15, 25)))

class ConstraintAnyTestCase(RequiresDevice):
    def runTest(self):
        self.assertRaises(TypeError, _ped.constraint_any, None)

        constraint = _ped.constraint_any(self._device)
        self.assertTrue(isinstance(constraint, _ped.Constraint))

        for testGeom in [_ped.Geometry(self._device, 0, 5),
                         _ped.Geometry(self._device, 10, 25),
                         _ped.Geometry(self._device, 0, 100)]:
            self.assertTrue(constraint.is_solution(testGeom))

class ConstraintExactTestCase(RequiresDevice):
    def runTest(self):
        geom = _ped.Geometry(self._device, 0, 100)

        self.assertRaises(TypeError, _ped.constraint_exact, None)

        constraint = _ped.constraint_exact(geom)
        self.assertTrue(isinstance(constraint, _ped.Constraint))

        for testGeom in [_ped.Geometry(self._device, 1, 100),
                         _ped.Geometry(self._device, 0, 99),
                         _ped.Geometry(self._device, 10, 20),
                         _ped.Geometry(self._device, 50, 101)]:
            self.assertFalse(constraint.is_solution(testGeom))

        self.assertTrue(constraint.is_solution(_ped.Geometry(self._device, 0, 100)))

class DeviceGetTestCase(RequiresDevice):
    def runTest(self):
        # Try getting the device we just made.
        self.assertTrue(isinstance(_ped.device_get(self.path), _ped.Device))

        # Try getting a device that doesn't exist.
        self.assertRaises(_ped.IOException, _ped.device_get, "/blah/whatever")
        self.assertRaises(_ped.IOException, _ped.device_get, "")
        self.assertRaises(_ped.DeviceException, _ped.device_get, None)

class DeviceGetNextTestCase(unittest.TestCase, BuildList):
    def runTest(self):
        # Make sure there are some devices in the system first and then
        # make a list out of them.  That's easier to work with.
        _ped.device_probe_all()
        lst = self.getDeviceList(_ped.device_get_next)

        # Now the test cases.
        self.assertGreater(len(lst), 0)
        self.assertRaises(TypeError, _ped.device_get_next, None)

        for ele in lst:
            self.assertTrue(isinstance(ele, _ped.Device))

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

        self.assertGreater(len(lst), 0)
        self.assertGreater(len(filter(lambda e: e.path.startswith("/tmp/temp-device-"), lst)), 0)

class DeviceFreeAllTestCase(RequiresDevice):
    def runTest(self):
        _ped.device_probe_all()
        self.assertEquals(_ped.device_free_all(), None)

class DiskTypeGetTestCase(unittest.TestCase):
    def runTest(self):
        for d in ["aix", "amiga", "bsd", "dvh", "gpt", "loop", "mac", "msdos",
                  "pc98","sun"]:
            t = _ped.disk_type_get(d)
            self.assertTrue(isinstance(t, _ped.DiskType))
            self.assertEquals(t.name, d)

        self.assertRaises(_ped.UnknownTypeException, _ped.disk_type_get, "nosuch")

class DiskTypeGetNextTestCase(unittest.TestCase, BuildList):
    def runTest(self):
        lst = self.getDeviceList(_ped.disk_type_get_next)
        self.assertGreater(len(lst), 0)
        self.assertRaises(TypeError, _ped.device_get_next, None)

        for ele in lst:
            self.assertTrue(isinstance(ele, _ped.DiskType))

        self.assertRaises(IndexError, _ped.disk_type_get_next, lst[-1])

class FileSystemProbeTestCase(RequiresFileSystem):
    def runTest(self):
        type = _ped.file_system_probe(self._geometry)

        for name in self._fileSystemType.keys():
            if name == 'ext2':
                self.assertEquals(type.name, name)
            else:
                self.assertNotEquals(type.name, name)

class FileSystemProbeSpecificTestCase(RequiresFileSystem):
    def runTest(self):
        for (name, type,) in self._fileSystemType.items():
            if name == 'ext2':
                result = _ped.file_system_probe_specific(type, self._geometry)

                # XXX: this should work
                # we're getting
                #     ValueError: object comparing to must be a _ped.Geometry
                # at runtime.  works fine in pdb.
                #self.assertEquals(result, self._geometry)

                self.assertTrue(isinstance(result, _ped.Geometry))
                self.assertEquals(result.start, self._geometry.start)
                self.assertEquals(result.end, self._geometry.end)
                self.assertEquals(result.length, self._geometry.length)
                self.assertEquals(result.dev, self._device)
            else:
                result = _ped.file_system_probe_specific(type, self._geometry)
                self.assertEquals(result, None)

class FileSystemTypeGetTestCase(unittest.TestCase):
    def runTest(self):
        for f in ["affs0", "amufs", "apfs1", "asfs", "ext2", "ext3", "fat16",
                  "fat32", "hfs", "hfs+", "hfsx", "hp-ufs", "jfs", "linux-swap",
                  "ntfs", "reiserfs", "sun-ufs", "xfs"]:
            self.assertTrue(isinstance(_ped.file_system_type_get(f), _ped.FileSystemType),
                         "Could not get fs type %s" % f)

        self.assertRaises(_ped.UnknownTypeException, _ped.file_system_type_get, "nosuch")

class FileSystemTypeGetNextTestCase(unittest.TestCase, BuildList):
    def runTest(self):
        lst = self.getDeviceList(_ped.file_system_type_get_next)
        self.assertGreater(len(lst), 0)
        self.assertRaises(TypeError, _ped.file_system_type_get_next, None)

        for ele in lst:
            self.assertTrue(isinstance(ele, _ped.FileSystemType))

        self.assertRaises(IndexError, _ped.file_system_type_get_next, lst[-1])

class PartitionTypeGetNameTestCase(unittest.TestCase):
    def runTest(self):
        for t in [_ped.PARTITION_METADATA, _ped.PARTITION_FREESPACE,
                  _ped.PARTITION_EXTENDED, _ped.PARTITION_LOGICAL]:
            self.assertNotEquals(_ped.partition_type_get_name(t), "", "Could not get name for flag %s" % t)

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
            self.assertEquals(_ped.unit_get_default(), v, "Could not set unit default to %s" % v)

        self.assertRaises(ValueError, _ped.unit_set_default, -1)
        self.assertRaises(ValueError, _ped.unit_set_default, 1000)

class UnitGetDefaultTestCase(unittest.TestCase):
    def runTest(self):
        self.assertGreaterEqual(_ped.unit_get_default(), 0)

class UnitGetSizeTestCase(RequiresDevice):
    def runTest(self):
        self.assertEquals(self._device.unit_get_size(_ped.UNIT_SECTOR), 512)
        self.assertEquals(self._device.unit_get_size(_ped.UNIT_BYTE), 1)
        self.assertEquals(self._device.unit_get_size(_ped.UNIT_KILOBYTE), 1000)
        self.assertEquals(self._device.unit_get_size(_ped.UNIT_MEGABYTE), 1000000)
        self.assertEquals(self._device.unit_get_size(_ped.UNIT_GIGABYTE), 1000000000)
        self.assertEquals(self._device.unit_get_size(_ped.UNIT_TERABYTE), 1000000000000)
        self.assertEquals(self._device.unit_get_size(_ped.UNIT_KIBIBYTE), 1024)
        self.assertEquals(self._device.unit_get_size(_ped.UNIT_MEBIBYTE), 1048576)
        self.assertEquals(self._device.unit_get_size(_ped.UNIT_GIBIBYTE), 1073741824)
        self.assertEquals(self._device.unit_get_size(_ped.UNIT_TEBIBYTE), 1099511627776)
        self.assertEquals(self._device.unit_get_size(_ped.UNIT_CYLINDER), 65536)
        self.assertEquals(self._device.unit_get_size(_ped.UNIT_CHS), 512)
        self.assertEquals(self._device.unit_get_size(_ped.UNIT_PERCENT), self._device.length * self._device.sector_size / 100)
        self.assertRaises(ValueError, self._device.unit_get_size, _ped.UNIT_COMPACT)

class UnitGetNameTestCase(unittest.TestCase):
    def runTest(self):
        self.assertEquals(_ped.unit_get_name(_ped.UNIT_BYTE), 'B')
        self.assertEquals(_ped.unit_get_name(_ped.UNIT_CHS), 'chs')
        self.assertEquals(_ped.unit_get_name(_ped.UNIT_COMPACT), 'compact')
        self.assertEquals(_ped.unit_get_name(_ped.UNIT_CYLINDER), 'cyl')
        self.assertEquals(_ped.unit_get_name(_ped.UNIT_GIBIBYTE), 'GiB')
        self.assertEquals(_ped.unit_get_name(_ped.UNIT_GIGABYTE), 'GB')
        self.assertEquals(_ped.unit_get_name(_ped.UNIT_KIBIBYTE), 'kiB')
        self.assertEquals(_ped.unit_get_name(_ped.UNIT_KILOBYTE), 'kB')
        self.assertEquals(_ped.unit_get_name(_ped.UNIT_MEBIBYTE), 'MiB')
        self.assertEquals(_ped.unit_get_name(_ped.UNIT_MEGABYTE), 'MB')
        self.assertEquals(_ped.unit_get_name(_ped.UNIT_PERCENT), '%')
        self.assertEquals(_ped.unit_get_name(_ped.UNIT_SECTOR), 's')
        self.assertEquals(_ped.unit_get_name(_ped.UNIT_TEBIBYTE), 'TiB')
        self.assertEquals(_ped.unit_get_name(_ped.UNIT_TERABYTE), 'TB')

        self.assertRaises(ValueError, _ped.unit_get_name, -1)
        self.assertRaises(ValueError, _ped.unit_get_name, 1000)

class UnitGetByNameTestCase(unittest.TestCase):
    def runTest(self):
        self.assertEquals(_ped.unit_get_by_name('B'), _ped.UNIT_BYTE)
        self.assertEquals(_ped.unit_get_by_name('chs'), _ped.UNIT_CHS)
        self.assertEquals(_ped.unit_get_by_name('compact'), _ped.UNIT_COMPACT)
        self.assertEquals(_ped.unit_get_by_name('cyl'), _ped.UNIT_CYLINDER)
        self.assertEquals(_ped.unit_get_by_name('GiB'), _ped.UNIT_GIBIBYTE)
        self.assertEquals(_ped.unit_get_by_name('GB'), _ped.UNIT_GIGABYTE)
        self.assertEquals(_ped.unit_get_by_name('kiB'), _ped.UNIT_KIBIBYTE)
        self.assertEquals(_ped.unit_get_by_name('kB'), _ped.UNIT_KILOBYTE)
        self.assertEquals(_ped.unit_get_by_name('MiB'), _ped.UNIT_MEBIBYTE)
        self.assertEquals(_ped.unit_get_by_name('MB'), _ped.UNIT_MEGABYTE)
        self.assertEquals(_ped.unit_get_by_name('%'), _ped.UNIT_PERCENT)
        self.assertEquals(_ped.unit_get_by_name('s'), _ped.UNIT_SECTOR)
        self.assertEquals(_ped.unit_get_by_name('TiB'), _ped.UNIT_TEBIBYTE)
        self.assertEquals(_ped.unit_get_by_name('TB'), _ped.UNIT_TERABYTE)

        self.assertRaises(_ped.UnknownTypeException, _ped.unit_get_by_name, "blargle")

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
    suite.addTest(DiskFlagGetNameTestCase())
    suite.addTest(DiskFlagGetByNameTestCase())
    suite.addTest(DiskFlagNextTestCase())
    suite.addTest(DiskTypeGetTestCase())
    suite.addTest(DiskTypeGetNextTestCase())
    suite.addTest(FileSystemProbeTestCase())
    suite.addTest(FileSystemProbeSpecificTestCase())
    suite.addTest(FileSystemTypeGetTestCase())
    suite.addTest(FileSystemTypeGetNextTestCase())
    suite.addTest(PartitionFlagGetNameTestCase())
    suite.addTest(PartitionFlagGetByNameTestCase())
    suite.addTest(PartitionFlagNextTestCase())
    suite.addTest(PartitionTypeGetNameTestCase())
    suite.addTest(UnitSetDefaultTestCase())
    suite.addTest(UnitGetDefaultTestCase())
    suite.addTest(UnitGetSizeTestCase())
    suite.addTest(UnitGetNameTestCase())
    suite.addTest(UnitGetByNameTestCase())
    return suite

s = suite()
if __name__ == "__main__":
    unittest.main(defaultTest='s', verbosity=2)
