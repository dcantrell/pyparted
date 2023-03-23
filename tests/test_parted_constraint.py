#
# Test cases for the methods in the parted.constraint module itself
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
class ConstraintNewTestCase(RequiresDevice):
    def runTest(self):
        align1 = parted.Alignment(offset=10, grainSize=5)
        align2 = parted.Alignment(offset=10, grainSize=5)
        geom1 = parted.Geometry(device=self.device, start=0, length=50)
        geom2 = parted.Geometry(device=self.device, start=0, length=100)

        # Check that not passing enough args to parted.Constraint.__init__
        # is caught.
        self.assertRaises(parted.ConstraintException, parted.Constraint)
        self.assertRaises(
            parted.ConstraintException,
            parted.Constraint,
            startAlign=align1,
            endAlign=align2,
        )

        # And then the correct ways of creating a _ped.Constraint.
        c = parted.Constraint(minGeom=geom1, maxGeom=geom2)
        self.assertIsInstance(c, parted.Constraint)

        c = parted.Constraint(minGeom=geom1)
        self.assertIsInstance(c, parted.Constraint)

        c = parted.Constraint(maxGeom=geom2)
        self.assertIsInstance(c, parted.Constraint)

        c = parted.Constraint(exactGeom=geom1)
        self.assertIsInstance(c, parted.Constraint)

        c = parted.Constraint(device=self.device)
        self.assertIsInstance(c, parted.Constraint)

        c = parted.Constraint(
            startAlign=align1,
            endAlign=align2,
            startRange=geom1,
            endRange=geom2,
            minSize=10,
            maxSize=100,
        )
        self.assertIsInstance(c, parted.Constraint)

        # Use a _ped.Constraint as the initializer
        pc = _ped.Constraint(
            align1.getPedAlignment(),
            align2.getPedAlignment(),
            geom1.getPedGeometry(),
            geom2.getPedGeometry(),
            10,
            100,
        )
        c = parted.Constraint(PedConstraint=pc)
        self.assertIsInstance(c, parted.Constraint)
        self.assertEqual(c.getPedConstraint(), pc)


class ConstraintGetSetTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        align1 = parted.Alignment(offset=10, grainSize=5)
        align2 = parted.Alignment(offset=10, grainSize=5)
        geom1 = parted.Geometry(device=self.device, start=0, length=50)
        geom2 = parted.Geometry(device=self.device, start=25, length=50)

        self.c = parted.Constraint(
            startAlign=align1,
            endAlign=align2,
            startRange=geom1,
            endRange=geom2,
            minSize=10,
            maxSize=100,
        )

    def runTest(self):
        # Test that properties work
        self.assertEqual(self.c.minSize, 10)
        self.assertEqual(self.c.maxSize, 100)
        self.assertIsInstance(self.c.startAlign, parted.Alignment)
        self.assertIsInstance(self.c.endAlign, parted.Alignment)
        self.assertIsInstance(self.c.startRange, parted.Geometry)
        self.assertIsInstance(self.c.endRange, parted.Geometry)

        # Test that setting directly and getting with getattr works.
        self.c.minSize = 15
        self.c.maxSize = 75

        self.assertEqual(getattr(self.c, "minSize"), 15)
        self.assertEqual(getattr(self.c, "maxSize"), 75)
        self.assertIsInstance(getattr(self.c, "startAlign"), parted.Alignment)
        self.assertIsInstance(getattr(self.c, "endAlign"), parted.Alignment)
        self.assertIsInstance(getattr(self.c, "startRange"), parted.Geometry)
        self.assertIsInstance(getattr(self.c, "endRange"), parted.Geometry)

        # Test that setting with setattr and getting directly works.
        setattr(self.c, "minSize", 10)
        setattr(self.c, "maxSize", 90)

        self.assertEqual(self.c.minSize, 10)
        self.assertEqual(self.c.maxSize, 90)

        # Test that values have the right type.
        self.assertRaises(TypeError, setattr, self.c, "minSize", "string")

        # Test that looking for invalid attributes fails properly.
        self.assertRaises(AttributeError, getattr, self.c, "blah")

        self.assertRaises(AttributeError, setattr, self.c, "startRange", 47)
        self.assertRaises(AttributeError, setattr, self.c, "endRange", 47)


@unittest.skip("Unimplemented test case.")
class ConstraintIntersectTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class ConstraintSolveMaxTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class ConstraintSolveNearestTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class ConstraintIsSolutionTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class ConstraintGetPedConstraintTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class ConstraintStrTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")
