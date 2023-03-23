#
# Test cases for the methods in the parted.geometry module itself
#
# Copyright The pyparted Project Authors
# SPDX-License-Identifier: GPL-2.0-or-later
#

import parted
import unittest

from tests.baseclass import RequiresDevice

# One class per method, multiple tests per class.  For these simple methods,
# that seems like good organization.  More complicated methods may require
# multiple classes and their own test suite.
@unittest.skip("Unimplemented test case.")
class GeometryNewTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class GeometryGetSetTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class GeometryCheckTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class GeometryContainsTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class GeometryContainsSectorTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class GeometryEqualTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class GeometryGetSizeTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


class GeometryGetLengthTestCase(RequiresDevice):
    def runTest(self):
        length = 137
        geom = parted.Geometry(self.device, start=100, length=length)

        self.assertEqual(geom.getLength(), geom.length)
        self.assertEqual(geom.getLength(), length)


@unittest.skip("Unimplemented test case.")
class GeometryIntersectTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class GeometryMapTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class GeometryOverlapsWithTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class GeometryReadTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class GeometrySyncTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


class GeometryWriteTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        self.geom = parted.Geometry(self.device, start=10, length=100)

    def runTest(self):
        self._device.open()
        self.assertTrue(
            self.geom.write("".join(["\x00"] * self.device.sectorSize), 0, 1)
        )
        self.assertTrue(
            self.geom.write("".join(["\x01"] * self.device.sectorSize), 0, 1)
        )
        self._device.close()


@unittest.skip("Unimplemented test case.")
class GeometryGetPedGeometryTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class GeometryStrTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")
