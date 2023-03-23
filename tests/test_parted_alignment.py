#
# Test cases for the methods in the parted.alignment module itself
#
# Copyright The pyparted Project Authors
# SPDX-License-Identifier: GPL-2.0-or-later
#

import _ped
import parted
import unittest
from tests.baseclass import RequiresDevice

# One class per method, multiple tests per class.  For these simple methods,
# that seems like good organization.  More complicated methods may require
# multiple classes and their own test suite.
class AlignmentNewTestCase(unittest.TestCase):
    def setUp(self):
        self.pa = _ped.Alignment(0, 100)

    def runTest(self):
        # Check that not passing args to parted.Alignment.__init__ is caught.
        self.assertRaises(parted.AlignmentException, parted.Alignment)

        # And then the correct ways of creating a parted.Alignment
        a = parted.Alignment(offset=0, grainSize=100)
        self.assertIsInstance(a, parted.Alignment)

        b = parted.Alignment(PedAlignment=self.pa)
        self.assertIsInstance(b, parted.Alignment)

        # Test for _ped.Alignment equality
        self.assertEqual(b.getPedAlignment(), self.pa)

class AlignmentGetSetTestCase(unittest.TestCase):
    def setUp(self):
        self.a = parted.Alignment(offset=27, grainSize=49)

    def runTest(self):
        # Test that passing the args to __init__ works.
        self.assertIsInstance(self.a, parted.Alignment)
        self.assertEqual(self.a.offset, 27)
        self.assertEqual(self.a.grainSize, 49)

        # Test that setting directly and getting with getattr works.
        self.a.offset = 10
        self.a.grainSize = 90

        self.assertEqual(getattr(self.a, "offset"), 10)
        self.assertEqual(getattr(self.a, "grainSize"), 90)

        # Check that setting with setattr and getting directly works.
        setattr(self.a, "offset", 20)
        setattr(self.a, "grainSize", 80)

        self.assertEqual(self.a.offset, 20)
        self.assertEqual(self.a.grainSize, 80)

        # Check that values have the right type.
        self.assertRaises(TypeError, setattr, self.a, "offset", "string")

        # Check that looking for invalid attributes fails properly.
        self.assertRaises(AttributeError, getattr, self.a, "blah")

@unittest.skip("Unimplemented test case.")
class AlignmentIntersectTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class AlignmentAlignUpTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class AlignmentAlignDownTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

@unittest.skip("Unimplemented test case.")
class AlignmentAlignNearestTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")

class AlignmentIsAlignedTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        self.g = parted.Geometry(device=self.device, start=0, length=100)
        self.a = parted.Alignment(offset=10, grainSize=0)

    def runTest(self):
        # Test a couple ways of passing bad arguments.
        self.assertRaises(TypeError, self.a.isAligned, None, 12)
        self.assertRaises(TypeError, self.a.isAligned, self.g, None)

        # Sector must be inside the geometry.
        self.assertFalse(self.a.isAligned(self.g, 400))

        # If grain_size is 0, sector must be the same as offset.
        self.assertTrue(self.a.isAligned(self.g, 10))
        self.assertFalse(self.a.isAligned(self.g, 0))
        self.assertFalse(self.a.isAligned(self.g, 47))

        # If grain_size is anything else, there's real math involved.
        self.a.grainSize = 5
        self.assertTrue(self.a.isAligned(self.g, 20))
        self.assertFalse(self.a.isAligned(self.g, 23))

class AlignmentGetPedAlignmentTestCase(unittest.TestCase):
    def setUp(self):
        self.pa = _ped.Alignment(0, 100)
        self.alignment = parted.Alignment(PedAlignment=self.pa)

    def runTest(self):
        # Test to make sure we get a _ped.Alignment
        self.assertIsInstance(self.alignment.getPedAlignment(), _ped.Alignment)

        # Test for _ped.Alignment equality
        self.assertEqual(self.alignment.getPedAlignment(), self.pa)

@unittest.skip("Unimplemented test case.")
class AlignmentStrTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")
