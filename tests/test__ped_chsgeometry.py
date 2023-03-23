#
# Copyright The pyparted Project Authors
# SPDX-License-Identifier: GPL-2.0-or-later
#

import _ped
import unittest

from tests.baseclass import RequiresDevice

# One class per method, multiple tests per class.  For these simple methods,
# that seems like good organization.  More complicated methods may require
# multiple classes and their own test suite.
class CHSGeometryNewTestCase(unittest.TestCase):
    def runTest(self):
        # You're not allowed to create a new CHSGeometry object by hand.
        self.assertRaises(TypeError, _ped.CHSGeometry)

class CHSGeometryGetSetTestCase(RequiresDevice):
    def runTest(self):
        # A device has a CHSGeometry, so we can use that to attempt accessing
        # parameters.
        chs = self._device.hw_geom
        self.assertIsInstance(chs, _ped.CHSGeometry)

        # All attributes are read-only.
        self.assertRaises(AttributeError, setattr, chs, "cylinders", 47)
        self.assertRaises(AttributeError, setattr, chs, "heads", 47)
        self.assertRaises(AttributeError, setattr, chs, "sectors", 47)

        self.assertIsInstance(chs.cylinders, int)
        self.assertIsInstance(chs.heads, int)
        self.assertIsInstance(chs.sectors, int)

class CHSGeometryStrTestCase(RequiresDevice):
    def runTest(self):
        expected = "_ped.CHSGeometry instance --\n  cylinders: %d  heads: %d  sectors: %d" % (self._device.hw_geom.cylinders, self._device.hw_geom.heads, self._device.hw_geom.sectors,)
        result = str(self._device.hw_geom)
        self.assertEqual(result, expected)
