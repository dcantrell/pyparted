#
# Test cases for the methods in the parted.device module itself
#
# Copyright The pyparted Project Authors
# SPDX-License-Identifier: GPL-2.0-or-later
#

import unittest
from tests.baseclass import RequiresDevice

# One class per method, multiple tests per class.  For these simple methods,
# that seems like good organization.  More complicated methods may require
# multiple classes and their own test suite.
@unittest.skip("Unimplemented test case.")
class DeviceNewTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DeviceGetSetTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DeviceOpenTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DeviceCloseTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DeviceDestroyTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DeviceRemoveFromCacheTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DeviceBeginExternalAccessTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DeviceEndExternalAccessTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DeviceReadTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DeviceWriteTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DeviceSyncTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DeviceCheckTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DeviceStartSectorToCylinderTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DeviceEndSectorToCylinderTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DeviceStartCylinderToSectorTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DeviceEndCylinderToSectorTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DeviceGetSizeTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


class DeviceGetLengthTestCase(RequiresDevice):
    def runTest(self):
        self.assertEqual(self.device.getLength(), self.device.length)


@unittest.skip("Unimplemented test case.")
class DeviceGetSizeAsSectorsTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DeviceGetConstraintTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DeviceGetPedDeviceTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")


@unittest.skip("Unimplemented test case.")
class DeviceStrTestCase(unittest.TestCase):
    def runTest(self):
        # TODO
        self.fail("Unimplemented test case.")
