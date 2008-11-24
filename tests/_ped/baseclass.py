#!/usr/bin/python
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
import os
import tempfile
import unittest

# Base class for any test case that requires a _ped.Device object first.
class RequiresDevice(unittest.TestCase):
    def setUp(self):
        (fd, self.path) = tempfile.mkstemp(prefix="temp-device-")
        f = os.fdopen(fd)
        f.seek(128000)
        os.write(fd, "0")

        self._device = _ped.device_get(self.path)

    def tearDown(self):
        os.unlink(self.path)
