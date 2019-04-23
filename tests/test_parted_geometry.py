#
# Test cases for the methods in the parted.geometry module itself
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
        self.assertTrue(self.geom.write("".join(["\x00"] * self.device.sectorSize), 0, 1))
        self.assertTrue(self.geom.write("".join(["\x01"] * self.device.sectorSize), 0, 1))
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
