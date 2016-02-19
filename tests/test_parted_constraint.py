#
# Test cases for the methods in the parted.constraint module itself
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
# Red Hat Author(s): David Cantrell <dcantrell@redhat.com>
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
        self.assertRaises(parted.ConstraintException, parted.Constraint,
                          startAlign=align1, endAlign=align2)

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

        c = parted.Constraint(startAlign=align1, endAlign=align2,
                              startRange=geom1, endRange=geom2,
                              minSize=10, maxSize=100)
        self.assertIsInstance(c, parted.Constraint)

        # Use a _ped.Constraint as the initializer
        pc = _ped.Constraint(align1.getPedAlignment(),
                             align2.getPedAlignment(),
                             geom1.getPedGeometry(),
                             geom2.getPedGeometry(),
                             10, 100)
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

        self.c = parted.Constraint(startAlign=align1, endAlign=align2,
                                   startRange=geom1, endRange=geom2,
                                   minSize=10, maxSize=100)

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
