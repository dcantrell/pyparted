#!/usr/bin/python
# 
# Test cases for the methods in the _ped module itself - just the pyunit
# and pynatmath files.
#
# Copyright (C) 2008  Red Hat, Inc.
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
import unittest

# One class per method, multiple tests per class.  For these simple methods,
# that seems like good organization.  More complicated methods may require
# multiple classes and their own test suite.
class GreatestCommonDivisorTestCase(unittest.TestCase):
    def runTest(self):
        # Can't test cases where we pass a negative to greatest_common_divisor
        # because libparted will assert() on those and we'll abort.
        self.assertEqual(_ped.greatest_common_divisor(40, 0), 40)
        self.assertEqual(_ped.greatest_common_divisor(0, 40), 40)
        self.assertEqual(_ped.greatest_common_divisor(40, 10), 10)
        self.assertEqual(_ped.greatest_common_divisor(47, 19), 1)

class DivRoundToNearestTestCase(unittest.TestCase):
    def runTest(self):
        self.assertEqual(_ped.div_round_to_nearest(0, 100), 0)
        self.assertEqual(_ped.div_round_to_nearest(100, 5), 20)
        self.assertEqual(_ped.div_round_to_nearest(100, 6), 17)
        self.assertRaises(ZeroDivisionError, _ped.div_round_to_nearest, 100, 0)

class DivRoundUpTestCase(unittest.TestCase):
    def runTest(self):
        self.assertEqual(_ped.div_round_up(0, 100), 0)
        self.assertEqual(_ped.div_round_to_nearest(100, 5), 20)
        self.assertEqual(_ped.div_round_to_nearest(100, 6), 17)
        self.assertRaises(ZeroDivisionError, _ped.div_round_up, 100, 0)

class RoundDownToTestCase(unittest.TestCase):
    def runTest(self):
        self.assertEqual(_ped.round_down_to(0, 100), 0)
        self.assertEqual(_ped.round_down_to(100, 17), 85)
        self.assertEqual(_ped.round_down_to(100, -17), 85)
        self.assertRaises(ZeroDivisionError, _ped.round_down_to, 100, 0)

class RoundToNearestTestCase(unittest.TestCase):
    def runTest(self):
        self.assertEqual(_ped.round_to_nearest(0, 100), 0)
        self.assertEqual(_ped.round_to_nearest(100, 17), 102)
        self.assertEqual(_ped.round_to_nearest(100, -17), 68)
        self.assertRaises(ZeroDivisionError, _ped.round_to_nearest, 100, 0)

class RoundUpToTestCase(unittest.TestCase):
    def runTest(self):
        self.assertEqual(_ped.round_to_nearest(0, 100), 0)
        self.assertEqual(_ped.round_up_to(100, 17), 102)
        self.assertEqual(_ped.round_up_to(100, -17), 68)
        self.assertRaises(ZeroDivisionError, _ped.round_up_to, 100, 0)

# And then a suite to hold all the test cases for this module.
def suite():
    suite = unittest.TestSuite()
    suite.addTest(GreatestCommonDivisorTestCase())
    suite.addTest(DivRoundToNearestTestCase())
    suite.addTest(DivRoundUpTestCase())
    suite.addTest(RoundDownToTestCase())
    suite.addTest(RoundToNearestTestCase())
    suite.addTest(RoundUpToTestCase())
    return suite

s = suite()
unittest.TextTestRunner(verbosity=2).run(s)
