#
# Copyright (C) 2008-2011  Red Hat, Inc.
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
import unittest
from tests.baseclass import RequiresDevice, RequiresDeviceAlignment

# One class per method, multiple tests per class.  For these simple methods,
# that seems like good organization.  More complicated methods may require
# multiple classes and their own test suite.
class AlignmentNewTestCase(unittest.TestCase):
    def runTest(self):
        # Check that not passing args to _ped.Alignment.__init__ is caught.
        self.assertRaises(TypeError, _ped.Alignment)

        # And then the correct ways of creating a _ped.Alignment.
        self.assertIsInstance(_ped.Alignment(0, 100), _ped.Alignment)
        self.assertIsInstance(_ped.Alignment(offset=0, grain_size=100), _ped.Alignment)

class AlignmentGetSetTestCase(unittest.TestCase):
    def setUp(self):
        self.a = _ped.Alignment(27, 49)

    def runTest(self):
        # Test that passing the args to __init__ works.
        self.assertIsInstance(self.a, _ped.Alignment)
        self.assertEqual(self.a.offset, 27)
        self.assertEqual(self.a.grain_size, 49)

        # Test that setting directly and getting with getattr works.
        self.a.offset = 10
        self.a.grain_size = 90

        self.assertEqual(getattr(self.a, "offset"), 10)
        self.assertEqual(getattr(self.a, "grain_size"), 90)

        # Check that setting with setattr and getting directly works.
        setattr(self.a, "offset", 20)
        setattr(self.a, "grain_size", 80)

        self.assertEqual(self.a.offset, 20)
        self.assertEqual(self.a.grain_size, 80)

        # Check that values have the right type.
        self.assertRaises(TypeError, setattr, self.a, "offset", "string")

        # Check that looking for invalid attributes fails properly.
        self.assertRaises(AttributeError, getattr, self.a, "blah")

class AlignmentDuplicateTestCase(unittest.TestCase):
    def setUp(self):
        self.a = _ped.Alignment(27, 49)
        self.dup = self.a.duplicate()

    def runTest(self):
        self.assertEqual(self.a.offset, self.dup.offset)
        self.assertEqual(self.a.grain_size, self.dup.grain_size)

class AlignmentIntersectTestCase(unittest.TestCase):
    def setUp(self):
        self.trivialA = _ped.Alignment(47, 0)
        self.trivialB = _ped.Alignment(47, 0)

        self.complexA = _ped.Alignment(512, 3)
        self.complexB = _ped.Alignment(256, 4)

    def orderAlignments(self, a, b):
        if a.grain_size < b.grain_size:
            tmp = a
            a = b
            b = tmp

        return (a, b)

    # from libparted/cs/natmath.c
    def extendedEuclid(self, a, b):
        if b == 0:
            gcd = a
            x = 1
            y = 0
            return (gcd, x, y)

        (tmp_gcd, tmp_x, tmp_y) = self.extendedEuclid(b, a % b)
        gcd = tmp_gcd
        x = tmp_y
        y = tmp_x - (a / b) * tmp_y
        return (gcd, x, y)

    def runTest(self):
        # trivial test first, result should be a duplicate of trivialA
        trivial = self.trivialA.intersect(self.trivialB)
        self.assertEqual(trivial.offset, self.trivialA.offset)
        self.assertEqual(trivial.grain_size, self.trivialA.grain_size)

        # complex test second, see libparted/cs/natmath.c for an explanation
        # of the math behind computing the intersection of two alignments
        (verifyA, verifyB) = self.orderAlignments(self.complexA, self.complexB)
        (gcd, x, _y) = self.extendedEuclid(verifyA.grain_size,
                                           verifyB.grain_size)
        delta_on_gcd = (verifyB.offset - verifyA.offset) / gcd
        new_offset = verifyA.offset + x * delta_on_gcd * verifyA.grain_size
        new_grain_size = verifyA.grain_size * verifyB.grain_size / gcd

        intersection = self.complexA.intersect(self.complexB)
        self.assertEqual(new_offset, intersection.offset)
        self.assertEqual(new_grain_size, intersection.grain_size)

class AlignmentAlignUpTestCase(RequiresDeviceAlignment):
    def setUp(self):
        RequiresDeviceAlignment.setUp(self)
        self.trivialA = _ped.Alignment(10, 0)
        self.complexA = _ped.Alignment(512, 34)
        self.geometry = _ped.Geometry(self._device, start=0, length=100)
        self.sector = 47

    def runTest(self):
        # trivial test case first, grain_size is zero
        expected = self.closestInsideGeometry(self.trivialA, self.geometry,
                                              self.trivialA.offset)
        result = self.trivialA.align_up(self.geometry, self.sector)
        self.assertEqual(result, expected)

        # complex test case second, grain_size is not zero
        tmp = self.roundUpTo(self.sector - self.complexA.offset,
                             self.complexA.grain_size) + self.complexA.offset
        expected = self.closestInsideGeometry(self.complexA, self.geometry, tmp)
        result = self.complexA.align_up(self.geometry, self.sector)
        self.assertEqual(result, expected)

class AlignmentAlignDownTestCase(RequiresDeviceAlignment):
    def setUp(self):
        RequiresDeviceAlignment.setUp(self)
        self.trivialA = _ped.Alignment(10, 0)
        self.complexA = _ped.Alignment(512, 34)
        self.geometry = _ped.Geometry(self._device, start=0, length=100)
        self.sector = 47

    def runTest(self):
        # trivial test case first, grain_size is zero
        expected = self.closestInsideGeometry(self.trivialA, self.geometry,
                                              self.trivialA.offset)
        result = self.trivialA.align_down(self.geometry, self.sector)
        self.assertEqual(result, expected)

        # complex test case second, grain_size is not zero
        tmp = self.roundDownTo(self.sector - self.complexA.offset,
                               self.complexA.grain_size) + self.complexA.offset
        expected = self.closestInsideGeometry(self.complexA, self.geometry, tmp)
        result = self.complexA.align_down(self.geometry, self.sector)
        self.assertEqual(result, expected)

class AlignmentAlignNearestTestCase(RequiresDeviceAlignment):
    def setUp(self):
        RequiresDeviceAlignment.setUp(self)
        self.trivialA = _ped.Alignment(10, 0)
        self.complexA = _ped.Alignment(512, 34)
        self.geometry = _ped.Geometry(self._device, start=0, length=100)
        self.sector = 47

    def runTest(self):
        # trivial test case first, grain_size is zero
        tmp = self.closestInsideGeometry(self.trivialA, self.geometry,
                                         self.trivialA.offset)
        expected = self.closest(self.sector, tmp, tmp)
        result = self.trivialA.align_nearest(self.geometry, self.sector)
        self.assertEqual(result, expected)

        # complex test case second, grain_size is not zero
        tmpA = self.roundUpTo(self.sector - self.complexA.offset,
                              self.complexA.grain_size) + self.complexA.offset
        tmpA = self.closestInsideGeometry(self.complexA, self.geometry, tmpA)

        tmpB = self.roundDownTo(self.sector - self.complexA.offset,
                                self.complexA.grain_size) + self.complexA.offset
        tmpB = self.closestInsideGeometry(self.complexA, self.geometry, tmpB)

        expected = self.closest(self.sector, tmpA, tmpB)
        result = self.complexA.align_nearest(self.geometry, self.sector)
        self.assertEqual(result, expected)

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
        self.assertFalse(self.a.is_aligned(self.g, 400))

        # If grain_size is 0, sector must be the same as offset.
        self.assertTrue(self.a.is_aligned(self.g, 10))
        self.assertFalse(self.a.is_aligned(self.g, 0))
        self.assertFalse(self.a.is_aligned(self.g, 47))

        # If grain_size is anything else, there's real math involved.
        self.a.grain_size = 5
        self.assertTrue(self.a.is_aligned(self.g, 20))
        self.assertFalse(self.a.is_aligned(self.g, 23))

class AlignmentStrTestCase(unittest.TestCase):
    def setUp(self):
        self.alignment = _ped.Alignment(10, 0)

    def runTest(self):
        expected = "_ped.Alignment instance --\n  offset: 10  grain_size: 0"
        self.assertEqual(str(self.alignment), expected)
