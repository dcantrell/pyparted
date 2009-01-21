#!/usr/bin/python
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
#
import _ped
import unittest

from baseclass import *

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
        self.assert_(isinstance(part, _ped.Partition))

        # You don't need to pass a filesystem type at all, since this partition
        # might be FREESPACE or METADATA.
        part = _ped.Partition(self._disk, _ped.PARTITION_NORMAL, 0, 100)
        self.assert_(isinstance(part, _ped.Partition))

class PartitionGetSetTestCase(RequiresPartition):
    def runTest(self):
        # Test that passing the kwargs to __init__ works.
        self.assert_(self._part.disk == self._disk)
        self.assert_(isinstance(self._part.geom, _ped.Geometry))
        self.assert_(self._part.type == _ped.PARTITION_NORMAL)
        self.assert_(self._part.fs_type.name == "ext2")

        # Test that setting the RW attributes directly works.
        self._part.type = _ped.PARTITION_EXTENDED
        self._part.num = 1
        self.assert_(getattr(self._part, "type") == _ped.PARTITION_EXTENDED)
        self.assert_(getattr(self._part, "num") == 1)

        # Test that setting the RO attributes directly doesn't work.
        self.assertRaises(AttributeError, setattr, self._part, "fs_type",
                                          _ped.file_system_type_get("fat32"))
        self.assertRaises(AttributeError, setattr, self._part, "geom",
                                          _ped.Geometry(self._device, 10, 20))
        self.assertRaises(AttributeError, setattr, self._part, "disk", self._disk)

        # Check that values have the right type.
        self.assertRaises(TypeError, setattr, self._part, "type", "blah")

        # Check that looking for invalid attributes fails properly.
        self.assertRaises(AttributeError, getattr, self._part, "blah")

class PartitionDestroyTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        pass

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
            self._part.fs_type = None
            self._part.type = ty
            self.assertFalse(self._part.is_active())

class PartitionSetFlagTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        pass

class PartitionGetFlagTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        pass

class PartitionIsFlagAvailableTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        pass

class PartitionSetSystemTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        pass

class PartitionSetNameTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        pass

class PartitionGetNameTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        pass

class PartitionIsBusyTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        pass

class PartitionGetPathTestCase(RequiresPartition):
    def runTest(self):
        self.assert_(self._part.get_path() != "")


# And then a suite to hold all the test cases for this module.
def suite():
    suite = unittest.TestSuite()
    suite.addTest(PartitionNewTestCase())
    suite.addTest(PartitionGetSetTestCase())
    suite.addTest(PartitionDestroyTestCase())
    suite.addTest(PartitionIsActiveTestCase())
    suite.addTest(PartitionSetFlagTestCase())
    suite.addTest(PartitionGetFlagTestCase())
    suite.addTest(PartitionIsFlagAvailableTestCase())
    suite.addTest(PartitionSetSystemTestCase())
    suite.addTest(PartitionSetNameTestCase())
    suite.addTest(PartitionGetNameTestCase())
    suite.addTest(PartitionIsBusyTestCase())
    suite.addTest(PartitionGetPathTestCase())
    return suite

s = suite()
unittest.TextTestRunner(verbosity=2).run(s)
