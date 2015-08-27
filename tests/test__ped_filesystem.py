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
# Red Hat Author(s): Chris Lumens <clumens@redhat.com>
#                    David Cantrell <dcantrell@redhat.com>
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
