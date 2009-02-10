#!/usr/bin/python
#
# Test cases for the methods in the parted module itself
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
# Red Hat Author(s): David Cantrell <dcantrell@redhat.com>
#

import _ped
import parted
import unittest

# One class per method, multiple tests per class.  For these simple methods,
# that seems like good organization.  More complicated methods may require
# multiple classes and their own test suite.
class GetDeviceTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class GetAllDevicesTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class ProbeForSpecificFileSystemTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class ProbeFileSystemTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class FreshDiskTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class VersionTestCase(unittest.TestCase):
    def runTest(self):
        ver = parted.version()
        self.assertTrue(ver['libparted'] == _ped.libparted_version())
        self.assertTrue(ver['pyparted'] == _ped.pyparted_version())

# And then a suite to hold all the test cases for this module.
def suite():
    suite = unittest.TestSuite()
    suite.addTest(GetDeviceTestCase())
    suite.addTest(GetAllDevicesTestCase())
    suite.addTest(ProbeForSpecificFileSystemTestCase())
    suite.addTest(ProbeFileSystemTestCase())
    suite.addTest(FreshDiskTestCase())
    suite.addTest(VersionTestCase())
    return suite

s = suite()
unittest.TextTestRunner(verbosity=2).run(s)
