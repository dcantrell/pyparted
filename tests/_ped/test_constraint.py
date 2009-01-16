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
class ConstraintNewTestCase(RequiresDevice):
    def runTest(self):
        align1 = _ped.Alignment(10, 5)
        align2 = _ped.Alignment(10, 5)
        geom1 = _ped.Geometry(self._device, 0, 50)
        geom2 = _ped.Geometry(self._device, 25, 50)

        # Check that not passing enough args to _ped.Constraint.__init__ is caught.
        self.assertRaises(TypeError, _ped.Constraint)
        self.assertRaises(TypeError, _ped.Constraint, align1, align2)

#        # Or the parameters in the wrong order.
#        self.assertRaises(TypeError, _ped.Constraint, align1, align2, 10, 100,
#                          geom1, geom2)

        # And then the correct way of creating a _ped.Constraint.
        c = _ped.Constraint(align1, align2, geom1, geom2, 10, 100)
        self.assert_(isinstance(c, _ped.Constraint))

class ConstraintGetSetTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        align1 = _ped.Alignment(10, 5)
        align2 = _ped.Alignment(10, 5)
        geom1 = _ped.Geometry(self._device, 0, 50)
        geom2 = _ped.Geometry(self._device, 25, 50)

        self.c = _ped.Constraint(align1, align2, geom1, geom2, min_size=10,
                                 max_size=100)

    def runTest(self):
        # Test that passing the kwargs to __init__ works.
        self.assert_(self.c.min_size == 10)
        self.assert_(self.c.max_size == 100)
        self.assert_(isinstance(self.c.start_align, _ped.Alignment))
        self.assert_(isinstance(self.c.end_align, _ped.Alignment))
        self.assert_(isinstance(self.c.start_range, _ped.Geometry))
        self.assert_(isinstance(self.c.end_range, _ped.Geometry))

        # Test that setting directly and getting with getattr works.
        self.c.min_size = 15
        self.c.max_size = 75

        self.assert_(getattr(self.c, "min_size") == 15)
        self.assert_(getattr(self.c, "max_size") == 75)
        self.assert_(isinstance(getattr(self.c, "start_align"), _ped.Alignment))
        self.assert_(isinstance(getattr(self.c, "end_align"), _ped.Alignment))
        self.assert_(isinstance(getattr(self.c, "start_range"), _ped.Geometry))
        self.assert_(isinstance(getattr(self.c, "end_range"), _ped.Geometry))

        # Test that setting with setattr and getting directly works.
        setattr(self.c, "min_size", 10)
        setattr(self.c, "max_size", 90)

        self.assert_(self.c.min_size == 10)
        self.assert_(self.c.max_size == 90)

        # Test that values have the right type.
        self.assertRaises(TypeError, setattr, self.c, "min_size", "string")

        # Test that looking for invalid attributes fails properly.
        self.assertRaises(AttributeError, getattr, self.c, "blah")

        # We really shouldn't be allowed to overwrite objects stored in a
        # _ped.Constraint, but for now there's no way to prevent it.
        self.c.end_range = 47
        self.assert_(self.c.end_range == 47)

class ConstraintDuplicateTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        align1 = _ped.Alignment(10, 5)
        align2 = _ped.Alignment(10, 5)
        geom1 = _ped.Geometry(self._device, 0, 50)
        geom2 = _ped.Geometry(self._device, 25, 50)

        self.c = _ped.Constraint(align1, align2, geom1, geom2, min_size=10,
                                 max_size=100)

    def runTest(self):
        self.dup = self.c.duplicate()
        self.assert_(self.c.min_size == self.dup.min_size)
        self.assert_(self.c.max_size == self.dup.max_size)

        # duplicate methods should do a deepcopy, so self.dup should have
        # different references, but the same contents.
        self.assert_(hash(self.c) != hash(self.dup))
        self.assert_(hash(self.c.start_align) != hash(self.dup.start_align))
        self.assert_(hash(self.c.end_align) != hash(self.dup.end_align))
        self.assert_(hash(self.c.start_range) != hash(self.dup.start_range))
        self.assert_(hash(self.c.end_range) != hash(self.dup.end_range))
        self.assert_(self.c.start_align.grain_size == self.dup.start_align.grain_size)
        self.assert_(self.c.start_align.offset == self.dup.start_align.offset)
        self.assert_(self.c.end_align.grain_size == self.dup.end_align.grain_size)
        self.assert_(self.c.end_align.offset == self.dup.end_align.offset)
        self.assert_(self.c.start_range.start == self.dup.start_range.start)
        self.assert_(self.c.start_range.length == self.dup.start_range.length)
        self.assert_(self.c.start_range.end == self.dup.start_range.end)
        self.assert_(self.c.end_range.start == self.dup.end_range.start)
        self.assert_(self.c.end_range.length == self.dup.end_range.length)
        self.assert_(self.c.end_range.end == self.dup.end_range.end)

class ConstraintIntersectTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class ConstraintSolveMaxTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class ConstraintSolveNearestTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass

class ConstraintIsSolutionTestCase(unittest.TestCase):
    # TODO
    def runTest(self):
        pass


# And then a suite to hold all the test cases for this module.
def suite():
    suite = unittest.TestSuite()
    suite.addTest(ConstraintNewTestCase())
    suite.addTest(ConstraintGetSetTestCase())
    suite.addTest(ConstraintDuplicateTestCase())
    suite.addTest(ConstraintIntersectTestCase())
    suite.addTest(ConstraintSolveMaxTestCase())
    suite.addTest(ConstraintSolveNearestTestCase())
    suite.addTest(ConstraintIsSolutionTestCase())
    return suite

s = suite()
unittest.TextTestRunner(verbosity=2).run(s)
