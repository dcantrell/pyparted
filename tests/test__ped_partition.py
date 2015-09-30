#
# Copyright (C) 2009-2014  Red Hat, Inc.
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

import os
import _ped
import tempfile

from tests.baseclass import RequiresDisk, RequiresPartition

# One class per method, multiple tests per class.  For these simple methods,
# that seems like good organization.  More complicated methods may require
# multiple classes and their own test suite.
class PartitionNewTestCase(RequiresDisk):
    def runTest(self):
        # Check that not passing args to _ped.Partition.__init__ is caught.
        self.assertRaises(TypeError, _ped.Partition)

        # Or passing the arguments in the wrong order.
        self.assertRaises(TypeError, _ped.Partition, _ped.file_system_type_get("ext2"),
                                                     _ped.PARTITION_NORMAL, self._disk, 0, 100)

        part = _ped.Partition(self._disk, _ped.PARTITION_NORMAL, 0, 100,
                              _ped.file_system_type_get("ext2"))
        self.assertIsInstance(part, _ped.Partition)

        # You don't need to pass a filesystem type at all, since this partition
        # might be FREESPACE or METADATA.
        part = _ped.Partition(self._disk, _ped.PARTITION_NORMAL, 0, 100)
        self.assertIsInstance(part, _ped.Partition)

class PartitionGetSetTestCase(RequiresPartition):
    def runTest(self):
        # Test that passing the kwargs to __init__ works.
        self.assertEqual(self._part.disk, self._disk)
        self.assertIsInstance(self._part.geom, _ped.Geometry)
        self.assertEqual(self._part.type, _ped.PARTITION_NORMAL)
        self.assertEqual(self._part.fs_type.name, "ext2")

        # Test that setting the RW attributes directly works.
        self._part.type = _ped.PARTITION_EXTENDED
        self.assertEqual(getattr(self._part, "type"), _ped.PARTITION_EXTENDED)

        # Test that setting the RO attributes directly doesn't work.
        exn = (AttributeError, TypeError)
        self.assertRaises(exn, setattr, self._part, "num", 1)
        self.assertRaises(exn, setattr, self._part, "fs_type",
            _ped.file_system_type_get("fat32"))
        self.assertRaises(exn, setattr, self._part, "geom",
                                     _ped.Geometry(self._device, 10, 20))
        self.assertRaises(exn, setattr, self._part, "disk", self._disk)

        # Check that values have the right type.
        self.assertRaises(exn, setattr, self._part, "type", "blah")

        # Check that looking for invalid attributes fails properly.
        self.assertRaises(AttributeError, getattr, self._part, "blah")

class PartitionDestroyTestCase(RequiresPartition):
    def runTest(self):
        self.assertEqual(self._part.destroy(), None)

class PartitionIsActiveTestCase(RequiresPartition):
    def runTest(self):
        # A partition is active as long as it's not METADATA or FREE.
        for ty in [_ped.PARTITION_NORMAL, _ped.PARTITION_LOGICAL,
                   _ped.PARTITION_EXTENDED, _ped.PARTITION_PROTECTED]:
            self._part.type = ty
            self.assertTrue(self._part.is_active())

        for ty in [_ped.PARTITION_FREESPACE, _ped.PARTITION_METADATA]:
            # Can't have a partition of these two types that also has a
            # filesystem type associated with it.  libparted doesn't like
            # that combination.
            self._part = _ped.Partition(self._disk, ty, 0, 100)
            self.assertFalse(self._part.is_active())

class PartitionSetFlagTestCase(RequiresPartition):
    def runTest(self):
        self.assertTrue(self._part.set_flag(_ped.PARTITION_BOOT, 1))
        # try setting unavailable flag
        with self.assertRaises(_ped.PartedException):
            self._part.set_flag(1000, 1)

class PartitionGetFlagTestCase(RequiresPartition):
    def runTest(self):
        self.assertTrue(self._part.set_flag(_ped.PARTITION_BOOT, 1))
        self.assertTrue(self._part.get_flag(_ped.PARTITION_BOOT))

        # try getting unavailable flag - doesn't raise an exception
#        with self.assertRaises(_ped.PartedException):
#            self._part.get_flag(1000)



class PartitionIsFlagAvailableTestCase(RequiresPartition):
    def runTest(self):
        # We don't know which flags should be available and which shouldn't,
        # but we can at least check that there aren't any tracebacks from
        # trying all of the valid ones.
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
            self.assertIsInstance(self._part.is_flag_available(attr), bool)

        # However, an invalid flag should definitely not be available.
        self.assertFalse(self._part.is_flag_available(1000))

        # Partitions that are inactive should not have any available flags.
        self._part = _ped.Partition(self._disk, _ped.PARTITION_FREESPACE, 0, 100)
        self.assertRaises(_ped.PartitionException, self._part.is_flag_available,
                          _ped.PARTITION_BOOT)

class PartitionSetSystemTestCase(RequiresPartition):
    def runTest(self):
        self.assertTrue(self._part.set_system(_ped.file_system_type_get("fat32")))

        self.assertRaises(TypeError, self._part.set_system, 47)

        # Partitions that are inactive cannot have the system type set.
        self._part = _ped.Partition(self._disk, _ped.PARTITION_FREESPACE, 0, 100)
        self.assertRaises(_ped.PartitionException, self._part.set_system,
                          _ped.file_system_type_get("ext2"))

class PartitionSetNameTestCase(RequiresPartition):
    def runTest(self):
        # The DOS disklabel does not support naming.
        self.assertRaises(_ped.PartitionException, self._part.set_name, "blah")

        # These should work.
        self._disk = _ped.disk_new_fresh(self._device, _ped.disk_type_get("mac"))
        self._part = _ped.Partition(self._disk, _ped.PARTITION_NORMAL, 0, 100,
                                    _ped.file_system_type_get("fat32"))
        self.assertTrue(self._part.set_name("blah"))
        self.assertEqual(self._part.get_name(), "blah")

        # Partitions that are inactive won't work.
        self._part = _ped.Partition(self._disk, _ped.PARTITION_FREESPACE, 0, 100)
        self.assertRaises(_ped.PartitionException, self._part.get_name)

class PartitionGetNameTestCase(RequiresPartition):
    def runTest(self):
        # The DOS disklabel does not support naming.
        self.assertRaises(_ped.PartitionException, self._part.get_name)

        # Partitions that are inactive won't work either.
        self._part = _ped.Partition(self._disk, _ped.PARTITION_FREESPACE, 0, 100)
        self.assertRaises(_ped.PartitionException, self._part.get_name)

        # Mac disk labels do support naming, but there still has to be a name.
        self._disk = _ped.disk_new_fresh(self._device, _ped.disk_type_get("mac"))
        self._part = _ped.Partition(self._disk, _ped.PARTITION_NORMAL, 0, 100,
                                    _ped.file_system_type_get("fat32"))
        self.assertEqual(self._part.get_name(), "untitled")

        # Finally, Mac disk labels with a name will work.
        self._part.set_name("blah")
        self.assertEqual(self._part.get_name(), "blah")

class PartitionIsBusyTestCase(RequiresPartition):
    def setUp(self):
        self.addCleanup(self.removeMountpoint)
        RequiresPartition.setUp(self)
        self.mountpoint = None

    def doMount(self):
        self.mountpoint = tempfile.mkdtemp()
        os.system("mount -o loop %s %s" % (self.path, self.mountpoint))

    def mkfs(self):
        os.system("mkfs.ext2 -F -q %s" % self.path)

    def removeMountpoint(self):
        if self.mountpoint:
            os.system("umount %s" % self.mountpoint)
            os.rmdir(self.mountpoint)

    def runTest(self):
        # partitions aren't busy until they're mounted.
        self.assertFalse(self._part.is_busy())

# TODO:  need to figure out how to make a loopback device look mounted to
# libparted
#        self.mkfs()
#        self.doMount()
#        self.assertTrue(self._part.is_busy())


class PartitionGetPathTestCase(RequiresPartition):
    def runTest(self):
        self.assertNotEqual(self._part.get_path(), "")

class PartitionStrTestCase(RequiresPartition):
    def runTest(self):
        self.assertTrue(str(self._part).startswith("_ped.Partition instance"))
