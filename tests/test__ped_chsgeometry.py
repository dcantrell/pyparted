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
