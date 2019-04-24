#
# Test cases for the methods in the _ped module itself - just the pyunit
# and pynatmath files.
#
# Copyright (C) 2008-2014  Red Hat, Inc.
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

import os
import _ped
import unittest

from tests.baseclass import BuildList, RequiresDevice, RequiresFileSystem

# One class per method, multiple tests per class.  For these simple methods,
# that seems like good organization.  More complicated methods may require
# multiple classes and their own test suite.
class PartitionFlagGetNameTestCase(unittest.TestCase):
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
            if not hasattr(_ped, f):
                continue
            attr = getattr(_ped, f)
            self.assertNotEqual(_ped.partition_flag_get_name(attr), "", "Could not get name for flag _ped.%s" % f)

        self.assertRaises(ValueError, _ped.partition_flag_get_name, -1)
        self.assertRaises(_ped.PartedException, _ped.partition_flag_get_name, 1000)

class PartitionFlagGetByNameTestCase(unittest.TestCase):
    def runTest(self):
        for f in ["boot", "root", "swap", "hidden", "raid", "lvm", "lba",
                  "hp-service", "palo", "prep", "msftres", "bios_grub",
                  "msftdata", "irst", "esp", "nonfs"]:
            self.assertNotEqual(_ped.partition_flag_get_by_name(f), "", "Could not get flag %s" % f)

        self.assertEqual(_ped.partition_flag_get_by_name("nosuchflag"), 0)

class PartitionFlagNextTestCase(unittest.TestCase):
    def runTest(self):
        # We should get TypeError when the parameter is invalid
        self.assertRaises(TypeError, _ped.partition_flag_next)
        self.assertRaises(TypeError, _ped.partition_flag_next, 'blah')

        # First flag is 0, keep getting flags until we loop back around
        # to zero.  Make sure each flag we get is an integer.
        flag = _ped.partition_flag_next(0)
        self.assertEqual(type(flag).__name__, 'int')

        while True:
            flag = _ped.partition_flag_next(flag)
            if not flag:
                break
            self.assertEqual(type(flag).__name__, 'int')

class DiskFlagGetNameTestCase(unittest.TestCase):
    def runTest(self):
        for f in [_ped.DISK_CYLINDER_ALIGNMENT]:
            self.assertNotEqual(_ped.disk_flag_get_name(f), "", "Could not get name for flag %s" % f)

        self.assertRaises(ValueError, _ped.disk_flag_get_name, -1)
        self.assertRaises(_ped.PartedException, _ped.disk_flag_get_name, 1000)

class DiskFlagGetByNameTestCase(unittest.TestCase):
    def runTest(self):
        for f in ["cylinder_alignment"]:
            self.assertNotEqual(_ped.disk_flag_get_by_name(f), 0, "Could not get flag %s" % f)

        self.assertEqual(_ped.disk_flag_get_by_name("nosuchflag"), 0)

class DiskFlagNextTestCase(unittest.TestCase):
    def runTest(self):
        # We should get TypeError when the parameter is invalid
        self.assertRaises(TypeError, _ped.disk_flag_next)
        self.assertRaises(TypeError, _ped.disk_flag_next, 'blah')

        # First flag is 0, keep getting flags until we loop back around
        # to zero.  Make sure each flag we get is an integer.
        flag = _ped.disk_flag_next(0)
        self.assertEqual(type(flag).__name__, 'int')

        while True:
            flag = _ped.disk_flag_next(flag)
            if not flag:
                break
            self.assertEqual(type(flag).__name__, 'int')

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
        minimum = _ped.Geometry(self._device, 10, 20)
        maximum = _ped.Geometry(self._device, 0, 30)
        constraint = _ped.constraint_new_from_min_max(minimum, maximum)

        self.assertIsInstance(constraint, _ped.Constraint)
        self.assertTrue(constraint.is_solution(_ped.Geometry(self._device, 10, 20)))
        self.assertFalse(constraint.is_solution(_ped.Geometry(self._device, 11, 20)))
        self.assertTrue(constraint.is_solution(_ped.Geometry(self._device, 5, 25)))
        self.assertTrue(constraint.is_solution(_ped.Geometry(self._device, 0, 30)))
        self.assertFalse(constraint.is_solution(_ped.Geometry(self._device, 0, 35)))

class ConstraintNewFromMinTestCase(RequiresDevice):
    def runTest(self):
        self.assertRaises(TypeError, _ped.constraint_new_from_min, None)

        minimum = _ped.Geometry(self._device, 10, 20)
        constraint = _ped.constraint_new_from_min(minimum)

        self.assertIsInstance(constraint, _ped.Constraint)
        self.assertTrue(constraint.is_solution(_ped.Geometry(self._device, 10, 20)))
        self.assertTrue(constraint.is_solution(_ped.Geometry(self._device, 5, 25)))
        self.assertFalse(constraint.is_solution(_ped.Geometry(self._device, 11, 19)))
        self.assertFalse(constraint.is_solution(_ped.Geometry(self._device, 15, 25)))

class ConstraintNewFromMaxTestCase(RequiresDevice):
    def runTest(self):
        self.assertRaises(TypeError, _ped.constraint_new_from_max, None)

        maximum = _ped.Geometry(self._device, 10, 20)
        constraint = _ped.constraint_new_from_max(maximum)

        self.assertIsInstance(constraint, _ped.Constraint)
        self.assertTrue(constraint.is_solution(_ped.Geometry(self._device, 10, 20)))
        self.assertFalse(constraint.is_solution(_ped.Geometry(self._device, 5, 25)))
        self.assertTrue(constraint.is_solution(_ped.Geometry(self._device, 11, 19)))
        self.assertFalse(constraint.is_solution(_ped.Geometry(self._device, 15, 25)))

class ConstraintAnyTestCase(RequiresDevice):
    def runTest(self):
        self.assertRaises(TypeError, _ped.constraint_any, None)

        constraint = _ped.constraint_any(self._device)
        self.assertIsInstance(constraint, _ped.Constraint)

        for testGeom in [_ped.Geometry(self._device, 0, 5),
                         _ped.Geometry(self._device, 10, 25),
                         _ped.Geometry(self._device, 0, 100)]:
            self.assertTrue(constraint.is_solution(testGeom))

class ConstraintExactTestCase(RequiresDevice):
    def runTest(self):
        geom = _ped.Geometry(self._device, 0, 100)

        self.assertRaises(TypeError, _ped.constraint_exact, None)

        constraint = _ped.constraint_exact(geom)
        self.assertIsInstance(constraint, _ped.Constraint)

        for testGeom in [_ped.Geometry(self._device, 1, 100),
                         _ped.Geometry(self._device, 0, 99),
                         _ped.Geometry(self._device, 10, 20),
                         _ped.Geometry(self._device, 50, 101)]:
            self.assertFalse(constraint.is_solution(testGeom))

        self.assertTrue(constraint.is_solution(_ped.Geometry(self._device, 0, 100)))

class DeviceGetTestCase(RequiresDevice):
    def runTest(self):
        # Try getting the device we just made.
        self.assertIsInstance(_ped.device_get(self.path), _ped.Device)

        # Try getting a device that doesn't exist.
        self.assertRaises(_ped.IOException, _ped.device_get, "/blah/whatever")
        self.assertRaises(_ped.IOException, _ped.device_get, "")
        self.assertRaises(_ped.DeviceException, _ped.device_get, None)

@unittest.skipUnless(os.geteuid() == 0, 'Requires root')
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
            self.assertIsInstance(ele, _ped.Device)

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
        prefix = self.path[:self.path.index(self.temp_prefix) - 1] + "/" + self.temp_prefix

        self.assertGreater(len(lst), 0)
        self.assertGreater(
            len([e for e in lst if e.path.startswith(prefix)]), 0)

class DeviceFreeAllTestCase(RequiresDevice):
    def runTest(self):
        _ped.device_probe_all()
        self.assertEqual(_ped.device_free_all(), None)

class DiskTypeGetTestCase(unittest.TestCase):
    def runTest(self):
        for d in ["aix", "amiga", "bsd", "dvh", "gpt", "loop", "mac", "msdos",
                  "pc98","sun"]:
            t = _ped.disk_type_get(d)
            self.assertIsInstance(t, _ped.DiskType)
            self.assertEqual(t.name, d)

        self.assertRaises(_ped.UnknownTypeException, _ped.disk_type_get, "nosuch")

class DiskTypeGetNextTestCase(unittest.TestCase, BuildList):
    def runTest(self):
        lst = self.getDeviceList(_ped.disk_type_get_next)
        self.assertGreater(len(lst), 0)
        self.assertRaises(TypeError, _ped.device_get_next, None)

        for ele in lst:
            self.assertIsInstance(ele, _ped.DiskType)

        self.assertRaises(IndexError, _ped.disk_type_get_next, lst[-1])

class FileSystemProbeTestCase(RequiresFileSystem):
    def runTest(self):
        ty = _ped.file_system_probe(self._geometry)

        for name in self._fileSystemType.keys():
            if name == 'ext2':
                self.assertEqual(ty.name, name)
            else:
                self.assertNotEqual(ty.name, name)

class FileSystemProbeSpecificTestCase(RequiresFileSystem):
    def runTest(self):
        for (name, ty,) in self._fileSystemType.items():
            if name == 'ext2':
                result = _ped.file_system_probe_specific(ty, self._geometry)

                # XXX: this should work
                # we're getting
                #     ValueError: object comparing to must be a _ped.Geometry
                # at runtime.  works fine in pdb.
                #self.assertEqual(result, self._geometry)

                self.assertIsInstance(result, _ped.Geometry)
                self.assertEqual(result.start, self._geometry.start)
                self.assertLessEqual(result.end, self._geometry.end)
                self.assertLessEqual(result.length, self._geometry.length)
                self.assertEqual(result.dev, self._device)
            else:
                result = _ped.file_system_probe_specific(ty, self._geometry)
                self.assertEqual(result, None)

class FileSystemTypeGetTestCase(unittest.TestCase):
    def runTest(self):
        for f in ["affs0", "amufs", "apfs1", "asfs", "btrfs", "ext2", "ext3", "ext4", "fat16",
                  "fat32", "hfs", "hfs+", "hfsx", "hp-ufs", "jfs", "linux-swap",
                  "ntfs", "reiserfs", "sun-ufs", "xfs"]:
            # may be missing some filesystem types depending on the parted
            # build on the test system
            try:
                t = _ped.file_system_type_get(f)
                self.assertIsInstance(t, _ped.FileSystemType, "Could not get fs type %s" % t)
            except _ped.UnknownTypeException:
                pass

        self.assertRaises(_ped.UnknownTypeException, _ped.file_system_type_get, "nosuch")

class FileSystemTypeGetNextTestCase(unittest.TestCase, BuildList):
    def runTest(self):
        lst = self.getDeviceList(_ped.file_system_type_get_next)
        self.assertGreater(len(lst), 0)
        self.assertRaises(TypeError, _ped.file_system_type_get_next, None)

        for ele in lst:
            self.assertIsInstance(ele, _ped.FileSystemType)

        self.assertRaises(IndexError, _ped.file_system_type_get_next, lst[-1])

class PartitionTypeGetNameTestCase(unittest.TestCase):
    def runTest(self):
        for t in [_ped.PARTITION_METADATA, _ped.PARTITION_FREESPACE,
                  _ped.PARTITION_EXTENDED, _ped.PARTITION_LOGICAL]:
            self.assertNotEqual(_ped.partition_type_get_name(t), "", "Could not get name for flag %s" % t)

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
            self.assertEqual(_ped.unit_get_default(), v, "Could not set unit default to %s" % v)

        self.assertRaises(ValueError, _ped.unit_set_default, -1)
        self.assertRaises(ValueError, _ped.unit_set_default, 1000)

class UnitGetDefaultTestCase(unittest.TestCase):
    def runTest(self):
        self.assertGreaterEqual(_ped.unit_get_default(), 0)

class UnitGetSizeTestCase(RequiresDevice):
    def runTest(self):
        self.assertEqual(self._device.unit_get_size(_ped.UNIT_SECTOR), 512)
        self.assertEqual(self._device.unit_get_size(_ped.UNIT_BYTE), 1)
        self.assertEqual(self._device.unit_get_size(_ped.UNIT_KILOBYTE), 1000)
        self.assertEqual(self._device.unit_get_size(_ped.UNIT_MEGABYTE), 1000000)
        self.assertEqual(self._device.unit_get_size(_ped.UNIT_GIGABYTE), 1000000000)
        self.assertEqual(self._device.unit_get_size(_ped.UNIT_TERABYTE), 1000000000000)
        self.assertEqual(self._device.unit_get_size(_ped.UNIT_KIBIBYTE), 1024)
        self.assertEqual(self._device.unit_get_size(_ped.UNIT_MEBIBYTE), 1048576)
        self.assertEqual(self._device.unit_get_size(_ped.UNIT_GIBIBYTE), 1073741824)
        self.assertEqual(self._device.unit_get_size(_ped.UNIT_TEBIBYTE), 1099511627776)
        self.assertEqual(self._device.unit_get_size(_ped.UNIT_CYLINDER), 65536)
        self.assertEqual(self._device.unit_get_size(_ped.UNIT_CHS), 512)
        self.assertEqual(self._device.unit_get_size(_ped.UNIT_PERCENT), self._device.length * self._device.sector_size // 100)
        self.assertRaises(ValueError, self._device.unit_get_size, _ped.UNIT_COMPACT)

class UnitGetNameTestCase(unittest.TestCase):
    def runTest(self):
        self.assertEqual(_ped.unit_get_name(_ped.UNIT_BYTE), 'B')
        self.assertEqual(_ped.unit_get_name(_ped.UNIT_CHS), 'chs')
        self.assertEqual(_ped.unit_get_name(_ped.UNIT_COMPACT), 'compact')
        self.assertEqual(_ped.unit_get_name(_ped.UNIT_CYLINDER), 'cyl')
        self.assertEqual(_ped.unit_get_name(_ped.UNIT_GIBIBYTE), 'GiB')
        self.assertEqual(_ped.unit_get_name(_ped.UNIT_GIGABYTE), 'GB')
        self.assertEqual(_ped.unit_get_name(_ped.UNIT_KIBIBYTE), 'kiB')
        self.assertEqual(_ped.unit_get_name(_ped.UNIT_KILOBYTE), 'kB')
        self.assertEqual(_ped.unit_get_name(_ped.UNIT_MEBIBYTE), 'MiB')
        self.assertEqual(_ped.unit_get_name(_ped.UNIT_MEGABYTE), 'MB')
        self.assertEqual(_ped.unit_get_name(_ped.UNIT_PERCENT), '%')
        self.assertEqual(_ped.unit_get_name(_ped.UNIT_SECTOR), 's')
        self.assertEqual(_ped.unit_get_name(_ped.UNIT_TEBIBYTE), 'TiB')
        self.assertEqual(_ped.unit_get_name(_ped.UNIT_TERABYTE), 'TB')

        self.assertRaises(ValueError, _ped.unit_get_name, -1)
        self.assertRaises(ValueError, _ped.unit_get_name, 1000)

class UnitGetByNameTestCase(unittest.TestCase):
    def runTest(self):
        self.assertEqual(_ped.unit_get_by_name('B'), _ped.UNIT_BYTE)
        self.assertEqual(_ped.unit_get_by_name('chs'), _ped.UNIT_CHS)
        self.assertEqual(_ped.unit_get_by_name('compact'), _ped.UNIT_COMPACT)
        self.assertEqual(_ped.unit_get_by_name('cyl'), _ped.UNIT_CYLINDER)
        self.assertEqual(_ped.unit_get_by_name('GiB'), _ped.UNIT_GIBIBYTE)
        self.assertEqual(_ped.unit_get_by_name('GB'), _ped.UNIT_GIGABYTE)
        self.assertEqual(_ped.unit_get_by_name('kiB'), _ped.UNIT_KIBIBYTE)
        self.assertEqual(_ped.unit_get_by_name('kB'), _ped.UNIT_KILOBYTE)
        self.assertEqual(_ped.unit_get_by_name('MiB'), _ped.UNIT_MEBIBYTE)
        self.assertEqual(_ped.unit_get_by_name('MB'), _ped.UNIT_MEGABYTE)
        self.assertEqual(_ped.unit_get_by_name('%'), _ped.UNIT_PERCENT)
        self.assertEqual(_ped.unit_get_by_name('s'), _ped.UNIT_SECTOR)
        self.assertEqual(_ped.unit_get_by_name('TiB'), _ped.UNIT_TEBIBYTE)
        self.assertEqual(_ped.unit_get_by_name('TB'), _ped.UNIT_TERABYTE)

        self.assertRaises(_ped.UnknownTypeException, _ped.unit_get_by_name, "blargle")
