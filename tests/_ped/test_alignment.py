#!/usr/bin/python
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
from baseclass import *

# One class per method, multiple tests per class.  For these simple methods,
# that seems like good organization.  More complicated methods may require
# multiple classes and their own test suite.
class AlignmentNewTestCase(unittest.TestCase):
    def runTest(self):
        # Check that not passing args to _ped.Alignment.__init__ is caught.
        self.assertRaises(TypeError, _ped.Alignment)

        # And then the correct ways of creating a _ped.Alignment.
        self.assert_(isinstance(_ped.Alignment(0, 100), _ped.Alignment))
        self.assert_(isinstance(_ped.Alignment(offset=0, grain_size=100), _ped.Alignment))

class AlignmentGetSetTestCase(unittest.TestCase):
    def setUp(self):
        self.a = _ped.Alignment(27, 49)

    def runTest(self):
        # Test that passing the args to __init__ works.
        self.assert_(isinstance(self.a, _ped.Alignment))
        self.assert_(self.a.offset == 27)
        self.assert_(self.a.grain_size == 49)

        # Test that setting directly and getting with getattr works.
        self.a.offset = 10
        self.a.grain_size = 90

        self.assert_(getattr(self.a, "offset") == 10)
        self.assert_(getattr(self.a, "grain_size") == 90)

        # Check that setting with setattr and getting directly works.
        setattr(self.a, "offset", 20)
        setattr(self.a, "grain_size", 80)

        self.assert_(self.a.offset == 20)
        self.assert_(self.a.grain_size == 80)

        # Check that values have the right type.
        self.assertRaises(TypeError, setattr, self.a, "offset", "string")

        # Check that looking for invalid attributes fails properly.
        self.assertRaises(AttributeError, getattr, self.a, "blah")

class AlignmentDuplicateTestCase(unittest.TestCase):
    def setUp(self):
        self.a = _ped.Alignment(27, 49)

    def runTest(self):
        self.dup = self.a.duplicate()
        self.assert_(self.a.offset == self.dup.offset)
        self.assert_(self.a.grain_size == self.dup.grain_size)

class AlignmentIntersectTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        pass

class AlignmentDestroyTestCase(unittest.TestCase):
    def setUp(self):
        self.a = _ped.Alignment(27, 49)

    def runTest(self):
        self.a.destroy()
        self.assert_(hasattr(self.a, "offset") == False)

class AlignmentAlignUpTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        pass

class AlignmentAlignDownTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        pass

class AlignmentAlignNearestTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        pass

class AlignmentIsAlignedTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        self.g = _ped.Geometry(self._device, start=0, length=100)
        self.a = _ped.Alignment(10, 0)

    def runTest(self):
        # Test a couple ways of passing bad arguments.
        self.assertRaises(TypeError, self.a.is_aligned, None, 12)
        self.assertRaises(TypeError, self.a.is_aligned, self.g, None)

        # Sector must be inside the geometry.
        self.assert_(self.a.is_aligned(self.g, 400) == False)

        # If grain_size is 0, sector must be the same as offset.
        self.assert_(self.a.is_aligned(self.g, 10) == True)
        self.assert_(self.a.is_aligned(self.g, 0) == False)
        self.assert_(self.a.is_aligned(self.g, 47) == False)

        # If grain_size is anything else, there's real math involved.
        self.a.grain_size = 5
        self.assert_(self.a.is_aligned(self.g, 20) == True)
        self.assert_(self.a.is_aligned(self.g, 23) == False)

# And then a suite to hold all the test cases for this module.
def suite():
    suite = unittest.TestSuite()
    suite.addTest(AlignmentNewTestCase())
    suite.addTest(AlignmentGetSetTestCase())
    suite.addTest(AlignmentDuplicateTestCase())
    suite.addTest(AlignmentIntersectTestCase())
    suite.addTest(AlignmentDestroyTestCase())
    suite.addTest(AlignmentAlignUpTestCase())
    suite.addTest(AlignmentAlignDownTestCase())
    suite.addTest(AlignmentAlignNearestTestCase())
    suite.addTest(AlignmentIsAlignedTestCase())
    return suite

s = suite()
unittest.TextTestRunner(verbosity=2).run(s)
