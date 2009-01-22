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

# One class per method, multiple tests per class.  For these simple methods,
# that seems like good organization.  More complicated methods may require
# multiple classes and their own test suite.
class DiskNewTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class DiskGetSetTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class DiskClobberTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class DiskClobberExcludeTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class DiskDuplicateTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class DiskDestroyTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class DiskCommitTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class DiskCommitToDevTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class DiskCommitToOsTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class DiskCheckTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class DiskPrintTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class DiskGetPrimaryPartitionCountTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class DiskGetLastPartitionNumTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class DiskGetMaxPrimaryPartitionCountTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class DiskAddPartitionTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class DiskRemovePartitionTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class DiskDeletePartitionTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class DiskDeleteAllTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class DiskSetPartitionGeomTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class DiskMaxmimzePartitionTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class DiskGetMaxPartitionGeoemtryTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class DiskMinimizeExtendedPartitionTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class DiskNextPartitionTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class DiskGetPartitionTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class DiskGetPartitionBySectorTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class DiskExtendedPartitionTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass


# And then a suite to hold all the test cases for this module.
def suite():
    suite = unittest.TestSuite()
    suite.addTest(DiskNewTestCase())
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
    return suite

s = suite()
unittest.TextTestRunner(verbosity=2).run(s)
