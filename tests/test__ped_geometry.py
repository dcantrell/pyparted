#
# Copyright (C) 2008-2016  Red Hat, Inc.
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
import six
from tests.baseclass import RequiresDevice

# One class per method, multiple tests per class.  For these simple methods,
# that seems like good organization.  More complicated methods may require
# multiple classes and their own test suite.
class GeometryNewTestCase(RequiresDevice):
    def runTest(self):
        # Check that not passing args to _ped.Geometry.__init__ is caught.
        self.assertRaises(TypeError, _ped.Geometry)

        # Or passing in the parameters in the wrong order.
        self.assertRaises(TypeError, _ped.Geometry, 0, self._device, 100)

        # And then the correct ways of creating a _ped.Geometry.
        self.assertIsInstance(_ped.Geometry(self._device, 0, 100), _ped.Geometry)
        self.assertIsInstance(_ped.Geometry(self._device, 0, 100, 101), _ped.Geometry)

class GeometryGetSetTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        self.g = _ped.Geometry(self._device, start=0, length=100)

    def runTest(self):
        # Test that passing the kwargs to __init__ works.
        self.assertIsInstance(self.g, _ped.Geometry)
        self.assertEqual(self.g.start, 0)
        self.assertEqual(self.g.length, 100)
        self.assertEqual(self.g.end, 99)

        # Test that setting directly and getting with getattr works.
        self.g.start = 10
        self.g.length = 90
        self.g.end = 99

        self.assertEqual(getattr(self.g, "start"), 10)
        self.assertEqual(getattr(self.g, "length"), 90)
        self.assertEqual(getattr(self.g, "end"), 99)

        # Check that setting with setattr and getting directly works.
        setattr(self.g, "start", 20)
        setattr(self.g, "length", 80)
        setattr(self.g, "end", 99)

        self.assertEqual(self.g.start, 20)
        self.assertEqual(self.g.length, 80)
        self.assertEqual(self.g.end, 99)

        # Check that values have the right type.
        self.assertRaises(TypeError, setattr, self.g, "start", "string")

        # Check that looking for invalid attributes fails properly.
        self.assertRaises(AttributeError, getattr, self.g, "blah")

class GeometryDuplicateTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        self.g = _ped.Geometry(self._device, start=0, length=100)
        self.dup = self.g.duplicate()

    def runTest(self):
        self.assertEqual(self.g.start, self.dup.start)
        self.assertEqual(self.g.length, self.dup.length)
        self.assertEqual(self.g.end, self.dup.end)

class GeometryIntersectTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        self.g1 = _ped.Geometry(self._device, start=0, length=100)
        self.g2 = _ped.Geometry(self._device, start=0, length=100)
        self.i = None

    def runTest(self):
        # g1 and g2 are the same, so their intersection is the same
        self.i = self.g1.intersect(self.g2)
        self.assertEqual(self.i.start, self.g1.start)
        self.assertEqual(self.i.end, self.g1.end)
        self.assertEqual(self.i.length, self.g1.length)

        # g2 is the second half of g1, so their intersection is the same as g2.
        self.g2.set_start(50)
        self.i = self.g1.intersect(self.g2)
        self.assertEqual(self.i.start, self.g2.start)
        self.assertEqual(self.i.end, self.g2.end)
        self.assertEqual(self.i.length, self.g2.length)

        # g2 only partially overlaps the end of g1, so they have a more
        # interesting intersection.
        self.g1.set_end(75)
        self.i = self.g1.intersect(self.g2)
        self.assertEqual(self.i.start, self.g2.start)
        self.assertEqual(self.i.end, self.g1.end)
        self.assertEqual(self.i.length, 26)

        # g1 and g2 do not overlap at all, so they have no intersection.
        self.g1.set(0, 25)
        self.g2.set(50, 100)
        self.assertRaises(ArithmeticError, self.g1.intersect, self.g2)

class GeometrySetTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        self.g = _ped.Geometry(self._device, start=0, length=100)

    def runTest(self):
        self.assertEqual(self.g.start, 0)
        self.assertEqual(self.g.length, 100)

        # Setting a negative for either value, or a length past the end of
        # the device should fail.
        self.assertRaises(_ped.CreateException, self.g.set, 100, -1000)

class GeometrySetStartTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        self.g = _ped.Geometry(self._device, start=0, length=100)

    def runTest(self):
        self.g.set_start(10)
        self.assertEqual(self.g.start, 10)
        self.assertEqual(self.g.length, 90)
        self.assertEqual(self.g.end, 99)

class GeometrySetEndTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        self.g = _ped.Geometry(self._device, start=0, length=100)

    def runTest(self):
        self.g.set_end(50)
        self.assertEqual(self.g.start, 0)
        self.assertEqual(self.g.length, 51)
        self.assertEqual(self.g.end, 50)

        # Setting a negative end or or before the start should fail.
        self.assertRaises(_ped.CreateException, self.g.set_end, -1)
        self.g.set_start(10)
        self.assertRaises(_ped.CreateException, self.g.set_end, 5)

class GeometryTestOverlapTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        self.g1 = _ped.Geometry(self._device, start=0, length=100)
        self.g2 = _ped.Geometry(self._device, start=50, length=100)

    def runTest(self):
        # g2 occupies the second half of g1, so they overlap.
        self.assertTrue(self.g1.test_overlap(self.g2))

        # g2 is entirely contained within g1, so they overlap.
        self.g2.set_end(75)
        self.assertTrue(self.g1.test_overlap(self.g2))

        # g1 goes from inside g2 to the end, so they overlap.
        self.g1.set_start(60)
        self.assertTrue(self.g1.test_overlap(self.g2))

        # g2 exists entirely before g1, so they do not overlap.
        self.g2.set(10, 10)
        self.assertFalse(self.g1.test_overlap(self.g2))

class GeometryTestInsideTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        self.g1 = _ped.Geometry(self._device, start=0, length=100)
        self.g2 = _ped.Geometry(self._device, start=0, length=100)

    def runTest(self):
        # g1 and g2 are the same, so they exist inside each other.
        self.assertTrue(self.g1.test_inside(self.g2))
        self.assertTrue(self.g2.test_inside(self.g1))

        # g2 is entirely contained within g1, so it's inside.
        self.g2.set_end(75)
        self.assertTrue(self.g1.test_inside(self.g2))
        self.assertFalse(self.g2.test_inside(self.g1))

        # g1 goes from inside g2 to the end, so it's not inside.
        self.g1.set_start(60)
        self.assertFalse(self.g1.test_inside(self.g2))
        self.assertFalse(self.g2.test_inside(self.g1))

        # g2 exists entirely before g1, so it's not inside.
        self.g2.set(10, 10)
        self.assertFalse(self.g1.test_inside(self.g2))
        self.assertFalse(self.g2.test_inside(self.g1))

class GeometryTestEqualTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        self.g1 = _ped.Geometry(self._device, start=0, length=100)
        self.g2 = _ped.Geometry(self._device, start=0, length=100)

    def runTest(self):
        # g1 and g2 have the same start and end.
        self.assertTrue(self.g1.test_equal(self.g2))

        # g1 and g2 have the same end, but different starts.
        self.g2.set_start(5)
        self.assertFalse(self.g1.test_equal(self.g2))

        # g1 and g2 have the same start, but different ends.
        self.g2.set_start(5)
        self.g2.set_end(50)
        self.assertFalse(self.g1.test_equal(self.g2))

class GeometryTestSectorInsideTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        self.g = _ped.Geometry(self._device, start=10, length=100)

    def runTest(self):
        # First check the boundary conditions.
        self.assertTrue(self.g.test_sector_inside(10))
        self.assertTrue(self.g.test_sector_inside(109))
        self.assertFalse(self.g.test_sector_inside(110))

        # Then some sectors that are obviously out.
        self.assertFalse(self.g.test_sector_inside(0))
        self.assertFalse(self.g.test_sector_inside(1000))
        self.assertFalse(self.g.test_sector_inside(-1))

class GeometryReadTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        self.g = _ped.Geometry(self._device, start=10, length=100)

    def runTest(self):
        # First try to read from a device that isn't open yet.
        self.assertRaises(_ped.IOException, self.g.read, 0, 10)

        # Our initial device is just full of zeros, so this should read a
        # whole lot of nothing.
        self._device.open()
        self.assertEqual(self.g.read(0, 10), "")

        # Test bad parameter passing.
        self.assertRaises(_ped.IOException, self.g.read, -10, 10)
        self.assertRaises(_ped.IOException, self.g.read, 0, -10)
        self.assertRaises(TypeError, self.g.read, None, None)

        # Can't read past the end of the geometry.
        self.assertRaises(_ped.IOException, self.g.read, 200, 1)
        self.assertRaises(_ped.IOException, self.g.read, 0, 200)

        # Now try writing something to the device, then reading to see if
        # we get the same thing back.
        self.g.write("1111111111", 0, 1)
        self.assertEqual(self.g.read(0, 10), "1111111111")

        # Write five bytes from the string to the geometry, so there's only
        # one byte present.  So, only one "2" should be there when we read.
        self.g.write("2", 20, 5)
        self.assertEqual(self.g.read(20, 5), "2")
        self.assertEqual(self.g.read(20, 1), "2")

        self._device.close()

class GeometrySyncTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        self.g = _ped.Geometry(self._device, start=0, length=100)

    def runTest(self):
        self._device.open()

        # XXX: I don't know of a better way to test this method.
        self.g.write("1111111111", 0, 1)
        self.assertEqual(self.g.sync(), 1)

        self._device.close()

class GeometrySyncFastTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        self.g = _ped.Geometry(self._device, start=0, length=100)

    def runTest(self):
        self._device.open()

        # XXX: I don't know of a better way to test this method.
        self.g.write("1111111111", 0, 1)
        self.assertEqual(self.g.sync_fast(), 1)

        self._device.close()

class GeometryWriteTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        self.g = _ped.Geometry(self._device, start=10, length=100)

    def runTest(self):
        # First try to write to a device that isn't open yet.
        self.assertRaises(_ped.IOException, self.g.write, "X", 0, 10)

        # Now try a real write and make sure we (1) don't get an error code
        # and (2) the data actually ends up on the device.
        self._device.open()
        self.assertNotEqual(self.g.write("X", 0, 10), 0)
        self.assertEqual(self.g.read(0, 10), "X")
        self.assertNotEqual(self.g.write("XXXXXXXXXX", 0, 10), 0)
        self.assertEqual(self.g.read(0, 10), "XXXXXXXXXX")

        # Test bad parameter passing.
        self.assertRaises(_ped.IOException, self.g.write, "X", -10, 10)
        self.assertRaises(_ped.IOException, self.g.write, "X", 0, -10)
        self.assertRaises(TypeError, self.g.write, None, None, None)

        # Can't write past the end of the geometry.
        self.assertRaises(_ped.IOException, self.g.write, "X", 200, 1)
        self.assertRaises(_ped.IOException, self.g.write, "X", 0, 200)

        self._device.close()

class GeometryCheckTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        self.g = _ped.Geometry(self._device, start=10, length=100)

    def runTest(self):
        # trivial test case first
        self.assertRaises(_ped.IOException, self.g.check, 0, 0, 0)

        self._device.open()

        self.assertEqual(self.g.check(0, 0, 10), 0)
        self.assertEqual(self.g.check(0, 0, 50), 0)

        self._device.close()

class GeometryMapTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        self.g1 = _ped.Geometry(self._device, start=10, length=100)
        self.g2 = _ped.Geometry(self._device, start=10, length=90)

    def runTest(self):
        # write a word to the device starting at sector 25
        self._device.open()
        self.g1.write("UNITTEST", 25, 8)

        val1 = self.g2.read(self.g2.map(self.g1, 25), 8)
        val2 = self.g1.read(25, 8)
        self.assertEqual(val1, val2)

        self._device.close()

class GeometryStrTestCase(RequiresDevice):
    def setUp(self):
        RequiresDevice.setUp(self)
        self.g = _ped.Geometry(self._device, start=10, length=100)

    def runTest(self):
        lines = str(self.g).split('\n')
        self.assertEqual(lines[0], '_ped.Geometry instance --')
        self.assertEqual(lines[1], '  start: 10  end: 109  length: 100')
        six.assertRegex(self, lines[2], '^  device: <_ped.Device object at .*')
