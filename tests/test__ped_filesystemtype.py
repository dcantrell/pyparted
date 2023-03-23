#
# Copyright The pyparted Project Authors
# SPDX-License-Identifier: GPL-2.0-or-later
#

import _ped
import unittest

# One class per method, multiple tests per class.  For these simple methods,
# that seems like good organization.  More complicated methods may require
# multiple classes and their own test suite.
class FileSystemTypeNewTestCase(unittest.TestCase):
    def runTest(self):
        # You can't create a FileSystemType by hand.
        self.assertRaises(TypeError, _ped.FileSystemType)

class FileSystemTypeGetSetTestCase(unittest.TestCase):
    def runTest(self):
        fstype = _ped.file_system_type_get("ext3")

        self.assertIsInstance(fstype, _ped.FileSystemType)
        self.assertEqual(fstype.name, "ext3")
        self.assertEqual(getattr(fstype, "name"), "ext3")
        self.assertRaises(AttributeError, setattr, fstype, "name", "vfat")
        self.assertRaises(AttributeError, getattr, fstype, "junk")

class FileSystemTypeStrTestCase(unittest.TestCase):
    def runTest(self):
        fstype = _ped.file_system_type_get("ext3")

        self.assertEqual(str(fstype), "_ped.FileSystemType instance --\n  name: ext3")
