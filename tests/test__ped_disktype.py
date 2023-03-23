#
# Copyright The pyparted Project Authors
# SPDX-License-Identifier: GPL-2.0-or-later
#

import _ped
import sys
import unittest
from tests.baseclass import RequiresDiskTypes

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

            if sys.version_info >= (3,):
                bigint = int
                uni = str
            else:
                bigint = long       # pylint: disable=undefined-variable
                uni = unicode       # pylint: disable=undefined-variable
            self.assertIsInstance(t.name, uni)
            self.assertEqual(t.name, name)
            self.assertIsInstance(t.features, bigint)

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

            if hasattr(_ped, "DISK_TYPE_PARTITION_TYPE_ID"):
                self.assertTrue(self.disktype[name].check_feature(_ped.DISK_TYPE_PARTITION_TYPE_ID))

            if hasattr(_ped, "DISK_TYPE_PARTITION_TYPE_UUID"):
                self.assertFalse(self.disktype[name].check_feature(_ped.DISK_TYPE_PARTITION_TYPE_UUID))

            if hasattr(_ped, "DISK_TYPE_DISK_UUID"):
                self.assertFalse(self.disktype[name].check_feature(_ped.DISK_TYPE_DISK_UUID))

            if hasattr(_ped, "DISK_TYPE_PARTITION_UUID"):
                self.assertFalse(self.disktype[name].check_feature(_ped.DISK_TYPE_PARTITION_UUID))

        # The following types support DISK_TYPE_PARTITION_NAME
        for name in ['amiga', 'gpt', 'mac', 'pc98']:
            self.assertFalse(self.disktype[name].check_feature(_ped.DISK_TYPE_EXTENDED))
            self.assertTrue(self.disktype[name].check_feature(_ped.DISK_TYPE_PARTITION_NAME))

        # The following types support DISK_TYPE_PARTITION_TYPE_UUID
        for name in ['gpt']:
            if hasattr(_ped, "DISK_TYPE_PARTITION_TYPE_UUID"):
                self.assertTrue(self.disktype[name].check_feature(_ped.DISK_TYPE_PARTITION_TYPE_UUID))

            if hasattr(_ped, "DISK_TYPE_DISK_UUID"):
                self.assertTrue(self.disktype[name].check_feature(_ped.DISK_TYPE_DISK_UUID))

            if hasattr(_ped, "DISK_TYPE_PARTITION_UUID"):
                self.assertTrue(self.disktype[name].check_feature(_ped.DISK_TYPE_PARTITION_UUID))

        # The following types support all features
        for name in ['dvh']:
            self.assertTrue(self.disktype[name].check_feature(_ped.DISK_TYPE_EXTENDED))
            self.assertTrue(self.disktype[name].check_feature(_ped.DISK_TYPE_PARTITION_NAME))

class DiskTypeStrTestCase(RequiresDiskTypes):
    def runTest(self):
        types = {
            "msdos": '_ped.DiskType instance --\n  name: msdos  features:',
            'aix':   '_ped.DiskType instance --\n  name: aix  features: ',
            'sun':   '_ped.DiskType instance --\n  name: sun  features: ',
            'amiga': '_ped.DiskType instance --\n  name: amiga  features: ',
            'gpt':   '_ped.DiskType instance --\n  name: gpt  features: ',
            'mac':   '_ped.DiskType instance --\n  name: mac  features: ',
            'bsd':   '_ped.DiskType instance --\n  name: bsd  features: ',
            'pc98':  '_ped.DiskType instance --\n  name: pc98  features: ',
            'loop':  '_ped.DiskType instance --\n  name: loop  features: ',
            'dvh':   '_ped.DiskType instance --\n  name: dvh  features: ',
        }

        for t, expected in types.items():
            self.assertIn(expected, str(self.disktype[t]))
