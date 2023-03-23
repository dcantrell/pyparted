#
# Copyright The pyparted Project Authors
# SPDX-License-Identifier: GPL-2.0-or-later
#

import sys
import _ped
from tests.baseclass import RequiresFileSystem

# One class per method, multiple tests per class.  For these simple methods,
# that seems like good organization.  More complicated methods may require
# multiple classes and their own test suite.
class FileSystemNewTestCase(RequiresFileSystem):
    def runTest(self):
        fstype = _ped.file_system_type_get("ext2")

        with self.assertRaises(TypeError):
            _ped.FileSystem(type=None, geom=None)
            _ped.FileSystem(type=fstype, geom=None)

        fs = _ped.FileSystem(type=fstype, geom=self._geometry)
        self.assertIsInstance(fs, _ped.FileSystem)

class FileSystemGetSetTestCase(RequiresFileSystem):
    def runTest(self):
        fstype = _ped.file_system_type_get("ext2")
        fs = _ped.FileSystem(type=fstype, geom=self._geometry)

        self.assertIsInstance(fs, _ped.FileSystem)
        self.assertEqual(fs.type, fstype)
        self.assertEqual(getattr(fs, "type"), fstype)
        # read-only attribute
        if sys.version_info[0] == 3:
            self.assertRaises(AttributeError, setattr, fs, "type", fstype)
        else:
            self.assertRaises(TypeError, setattr, fs, "type", fstype)
        self.assertRaises(AttributeError, getattr, fs, "junk")


class FileSystemStrTestCase(RequiresFileSystem):
    def runTest(self):
        fstype = _ped.file_system_type_get("ext2")
        fs = _ped.FileSystem(type=fstype, geom=self._geometry)
        # don't use assertEqual b/c __str__ includes memory addresses of
        # fstype and geom objects. This is easier.
        self.assertTrue(str(fs).startswith("_ped.FileSystem instance --"))
