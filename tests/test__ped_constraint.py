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

import _ped

from tests.baseclass import RequiresDevice

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

        # Or the parameters in the wrong order.
        self.assertRaises(TypeError, _ped.Constraint, align1, align2, 10, 100,
                          geom1, geom2)

        # And then the correct way of creating a _ped.Constraint.
        c = _ped.Constraint(align1, align2, geom1, geom2, 10, 100)
        self.assertIsInstance(c, _ped.Constraint)

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
        self.assertEqual(self.c.min_size, 10)
        self.assertEqual(self.c.max_size, 100)
        self.assertIsInstance(self.c.start_align, _ped.Alignment)
        self.assertIsInstance(self.c.end_align, _ped.Alignment)
        self.assertIsInstance(self.c.start_range, _ped.Geometry)
        self.assertIsInstance(self.c.end_range, _ped.Geometry)

        # Test that setting directly and getting with getattr works.
        self.c.min_size = 15
        self.c.max_size = 75

        self.assertEqual(getattr(self.c, "min_size"), 15)
        self.assertEqual(getattr(self.c, "max_size"), 75)
        self.assertIsInstance(getattr(self.c, "start_align"), _ped.Alignment)
        self.assertIsInstance(getattr(self.c, "end_align"), _ped.Alignment)
        self.assertIsInstance(getattr(self.c, "start_range"), _ped.Geometry)
        self.assertIsInstance(getattr(self.c, "end_range"), _ped.Geometry)

        # Test that setting with setattr and getting directly works.
        setattr(self.c, "min_size", 10)
        setattr(self.c, "max_size", 90)

        self.assertEqual(self.c.min_size, 10)
        self.assertEqual(self.c.max_size, 90)

        # Test that values have the right type.
        self.assertRaises(TypeError, setattr, self.c, "min_size", "string")

        # Test that looking for invalid attributes fails properly.
        self.assertRaises(AttributeError, getattr, self.c, "blah")

        # We really shouldn't be allowed to overwrite objects stored in a
        # _ped.Constraint, but for now there's no way to prevent it.
        self.c.end_range = 47
        self.assertEqual(self.c.end_range, 47)

class ConstraintDuplicateTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        align1 = _ped.Alignment(10, 0)
        align2 = _ped.Alignment(10, 0)
        geom1 = _ped.Geometry(self._device, 0, 50)
        geom2 = _ped.Geometry(self._device, 25, 50)

        self.c = _ped.Constraint(align1, align2, geom1, geom2, min_size=10,
                                 max_size=100)
        self.dup = self.c.duplicate()

    def runTest(self):
        self.assertEqual(self.c.min_size, self.dup.min_size)
        self.assertEqual(self.c.max_size, self.dup.max_size)

        # duplicate methods should do a deepcopy, so self.dup should have
        # different references, but the same contents.
        self.assertNotEqual(repr(self.c), repr(self.dup))

        self.assertNotEqual(repr(self.c.start_align), repr(self.dup.start_align))
        self.assertEqual(self.c.start_align.offset, self.dup.start_align.offset)
        self.assertEqual(self.c.start_align.grain_size, self.dup.start_align.grain_size)

        self.assertNotEqual(repr(self.c.end_align), repr(self.dup.end_align))
        self.assertEqual(self.c.end_align.offset, self.dup.end_align.offset)
        self.assertEqual(self.c.end_align.grain_size, self.dup.end_align.grain_size)

        self.assertNotEqual(repr(self.c.start_range), repr(self.dup.start_range))
        self.assertNotEqual(repr(self.c.start_range.dev), repr(self.dup.start_range.dev))
        self.assertEqual(self.c.start_range.dev.path, self.dup.start_range.dev.path)
        self.assertEqual(self.c.start_range.start, self.dup.start_range.start)
        self.assertEqual(self.c.start_range.length, self.dup.start_range.length)
        self.assertEqual(self.c.start_range.end, self.dup.start_range.end)

        self.assertNotEqual(repr(self.c.end_range), repr(self.dup.end_range))
        self.assertNotEqual(repr(self.c.end_range.dev), repr(self.dup.end_range.dev))
        self.assertEqual(self.c.end_range.dev.path, self.dup.end_range.dev.path)
        self.assertEqual(self.c.end_range.start, self.dup.end_range.start)
        self.assertEqual(self.c.end_range.length, self.dup.end_range.length)
        self.assertEqual(self.c.end_range.end, self.dup.end_range.end)

class ConstraintIntersectTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        align1 = _ped.Alignment(10, 0)
        align2 = _ped.Alignment(10, 0)
        geom1 = _ped.Geometry(self._device, 0, 50)
        geom2 = _ped.Geometry(self._device, 25, 50)

        self.c1 = _ped.Constraint(align1, align2, geom1, geom2, min_size=10,
                                  max_size=100)

        geom3 = _ped.Geometry(self._device, 10, 50)
        geom4 = _ped.Geometry(self._device, 30, 40)
        self.c2 = _ped.Constraint(align1, align2, geom3, geom4, min_size=10,
                                  max_size=100)

    def runTest(self):
        startAlign = self.c1.start_align.intersect(self.c2.start_align)
        endAlign = self.c1.end_align.intersect(self.c2.end_align)
        startRange = self.c1.start_range.intersect(self.c2.start_range)
        endRange = self.c1.end_range.intersect(self.c2.end_range)
        minSize = max(self.c1.min_size, self.c2.min_size)
        maxSize = min(self.c1.max_size, self.c2.max_size)

        if not startAlign or not endAlign or not startRange or not endRange:
            expected = None
        else:
            expected = _ped.Constraint(startAlign, endAlign,
                                       startRange, endRange,
                                       min_size=minSize, max_size=maxSize)

        result = self.c1.intersect(self.c2)
        self.assertEqual(result, expected)

class ConstraintSolveMaxTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        self.c1 = self._device.get_constraint()

    def runTest(self):
        result = self.c1.solve_max()
        self.assertEqual(result.dev, self._device)
        self.assertGreaterEqual(result.length, self._device.length - 1)

class ConstraintSolveNearestTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        self.c1 = self._device.get_constraint()
        self.g1 = _ped.Geometry(self._device, 1, 8)

    def runTest(self):
        result = self.c1.solve_nearest(self.g1)
        self.assertEqual(result, self.g1)

class ConstraintIsSolutionTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        self.c1 = self._device.get_constraint()
        self.g1 = _ped.Geometry(self._device, 1, 8)

    def runTest(self):
        self.assertTrue(self.c1.is_solution(self.g1))

class ConstraintStrTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        align1 = _ped.Alignment(10, 0)
        align2 = _ped.Alignment(10, 0)
        geom1 = _ped.Geometry(self._device, 0, 50)
        geom2 = _ped.Geometry(self._device, 25, 50)

        self.c1 = _ped.Constraint(align1, align2, geom1, geom2, min_size=10,
                                  max_size=100)
    def runTest(self):
        result = str(self.c1).split('\n')

        self.assertEqual(result[0], '_ped.Constraint instance --')
        self.assertTrue(result[1].startswith('  start_align: <_ped.Alignment object at '))
        self.assertNotEqual(result[1].find('  end_align: <_ped.Alignment object at '), -1)
        self.assertTrue(result[2].startswith('  start_range: <_ped.Geometry object at '))
        self.assertNotEqual(result[2].find('  end_range: <_ped.Geometry object at '), -1)
        self.assertEqual(result[3], '  min_size: 10  max_size: 100')
