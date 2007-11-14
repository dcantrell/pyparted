#
# PedDevice.py
# Python bindings for libparted (built on top of the _ped Python module).
#
# Copyright (C) 2007  Red Hat, Inc.
# All rights reserved.
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

__depstr = "%s is deprecated and will be removed."

class PedDevice:
    import _ped
    import warnings

    def __init__(self, path):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)
        self.device = _ped.Device.get(path)
        #self.length = self.device.
        #self.model = 
        #self.path = 
        #self.sector_size = 
        #self.type = 
        #self.heads = 
        #self.sectors = 
        #self.cylinders = 

    def close(self):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)
        return _ped.Device.close(self.device)

    def constraint_any(self):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)
        return _ped.Constraint.any(self.device)

    def destroy(self):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)
        return _ped.Device.destroy(self.device)

    def disk_new_fresh(self, type):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)
        return _ped.Disk.new_fresh(self.device, type)

    def disk_probe(self):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)
        return _ped.Disk.probe(self.device)

    def geometry_new(self, start, length):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)
        return _ped.Geometry.new(self.device, start, length)

    def is_busy(self):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)
        return _ped.Device.is_busy(self.device)

    def open(self):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)
        return _ped.Device.open(self.device)

    def sync(self):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning, stacklevel=2)
        return _ped.Device.sync(self.device)

def get(path):
    import warnings

    warnings.warn("PedDevice.get() is deprecated by Device.get()", DeprecationWarning, stacklevel=2)
    return PedDevice(path)
