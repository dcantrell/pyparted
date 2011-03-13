#
# Copyright (C) 2009  Red Hat, Inc.
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
from baseclass import *

# One class per method, multiple tests per class.  For these simple methods,
# that seems like good organization.  More complicated methods may require
# multiple classes and their own test suite.
class DiskTypeNewTestCase(unittest.TestCase):
    def runTest(self):
        # You're not allowed to create a new DiskType object by hand.
        self.assertRaises(TypeError, _ped.DiskType)

class DiskTypeGetSetTestCase(RequiresDiskTypes):
    def runTest(self):
        # All attributes are read-only.
        for name in self.disktype.keys():
            t = self.disktype[name]

            self.assertRaises(AttributeError, setattr, t, "name", "fakename")
            self.assertRaises(AttributeError, setattr, t, "features", 47)

            self.assertTrue(isinstance(t.name, str))
            self.assertEquals(t.name, name)
            self.assertTrue(isinstance(t.features, long))

class DiskTypeCheckFeatureTestCase(RequiresDiskTypes):
    def runTest(self):
        # The following types have no features [that libparted supports]
        for name in ['aix', 'sun', 'bsd', 'loop']:
            self.assertFalse(self.disktype[name].check_feature(_ped.DISK_TYPE_EXTENDED))
            self.assertFalse(self.disktype[name].check_feature(_ped.DISK_TYPE_PARTITION_NAME))

        # The following types support DISK_TYPE_EXTENDED
        for name in ['msdos']:
            self.assertTrue(self.disktype[name].check_feature(_ped.DISK_TYPE_EXTENDED))
            self.assertFalse(self.disktype[name].check_feature(_ped.DISK_TYPE_PARTITION_NAME))

        # The following types support DISK_TYPE_PARTITION_NAME
        for name in ['amiga', 'gpt', 'mac', 'pc98']:
            self.assertFalse(self.disktype[name].check_feature(_ped.DISK_TYPE_EXTENDED))
            self.assertTrue(self.disktype[name].check_feature(_ped.DISK_TYPE_PARTITION_NAME))

        # The following types support all features
        for name in ['dvh']:
            self.assertTrue(self.disktype[name].check_feature(_ped.DISK_TYPE_EXTENDED))
            self.assertTrue(self.disktype[name].check_feature(_ped.DISK_TYPE_PARTITION_NAME))

class DiskTypeStrTestCase(RequiresDiskTypes):
    def runTest(self):
        self.assertEquals(str(self.disktype['msdos']), '_ped.DiskType instance --\n  name: msdos  features: 1')
        self.assertEquals(str(self.disktype['aix']), '_ped.DiskType instance --\n  name: aix  features: 0')
        self.assertEquals(str(self.disktype['sun']), '_ped.DiskType instance --\n  name: sun  features: 0')
        self.assertEquals(str(self.disktype['amiga']), '_ped.DiskType instance --\n  name: amiga  features: 2')
        self.assertEquals(str(self.disktype['gpt']), '_ped.DiskType instance --\n  name: gpt  features: 2')
        self.assertEquals(str(self.disktype['mac']), '_ped.DiskType instance --\n  name: mac  features: 2')
        self.assertEquals(str(self.disktype['bsd']), '_ped.DiskType instance --\n  name: bsd  features: 0')
        self.assertEquals(str(self.disktype['pc98']), '_ped.DiskType instance --\n  name: pc98  features: 2')
        self.assertEquals(str(self.disktype['loop']), '_ped.DiskType instance --\n  name: loop  features: 0')
        self.assertEquals(str(self.disktype['dvh']), '_ped.DiskType instance --\n  name: dvh  features: 3')

# And then a suite to hold all the test cases for this module.
def suite():
    suite = unittest.TestSuite()
    suite.addTest(DiskTypeNewTestCase())
    suite.addTest(DiskTypeGetSetTestCase())
    suite.addTest(DiskTypeCheckFeatureTestCase())
    suite.addTest(DiskTypeStrTestCase())
    return suite

s = suite()
if __name__ == "__main__":
    unittest.main(defaultTest='s', verbosity=2)
