#
# __init__.py
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
# MERCHANTABILITY or FITNESS FOR A * PARTICULAR PURPOSE.  See the GNU General
# Public License for more details.  You should have received a copy of the
# GNU General Public License along with this program; if not, write to the
# Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
# 02110-1301, USA.  Any Red Hat trademarks that are incorporated in the
# source code or documentation are not subject to the GNU General Public
# License and may only be used or replicated with the express permission of
# Red Hat, Inc.
#
# Red Hat Author(s): David Cantrell
#

import _ped
import warnings

# the enumerated types in _ped need to be available from here too
UNIT_SECTOR = _ped.UNIT_SECTOR
UNIT_BYTE = _ped.UNIT_BYTE
UNIT_KILOBYTE = _ped.UNIT_KILOBYTE
UNIT_MEGABYTE = _ped.UNIT_MEGABYTE
UNIT_GIGABYTE = _ped.UNIT_GIGABYTE
UNIT_TERABYTE = _ped.UNIT_TERABYTE
UNIT_COMPACT = _ped.UNIT_COMPACT
UNIT_CYLINDER = _ped.UNIT_CYLINDER
UNIT_CHS = _ped.UNIT_CHS
UNIT_PERCENT = _ped.UNIT_PERCENT
UNIT_KIBIBYTE = _ped.UNIT_KIBIBYTE
UNIT_MEBIBYTE = _ped.UNIT_MEBIBYTE
UNIT_GIBIBYTE = _ped.UNIT_GIBIBYTE
UNIT_TEBIBYTE = _ped.UNIT_TEBIBYTE

DEVICE_UNKNOWN = _ped.DEVICE_UNKNOWN
DEVICE_SCSI = _ped.DEVICE_SCSI
DEVICE_IDE = _ped.DEVICE_IDE
DEVICE_DAC960 = _ped.DEVICE_DAC960
DEVICE_CPQARRAY = _ped.DEVICE_CPQARRAY
DEVICE_FILE = _ped.DEVICE_FILE
DEVICE_ATARAID = _ped.DEVICE_ATARAID
DEVICE_I2O = _ped.DEVICE_I2O
DEVICE_UBD = _ped.DEVICE_UBD
DEVICE_DASD = _ped.DEVICE_DASD
DEVICE_VIODASD = _ped.DEVICE_VIODASD
DEVICE_SX8 = _ped.DEVICE_SX8
DEVICE_DM = _ped.DEVICE_DM
DEVICE_XVD = _ped.DEVICE_XVD

PARTITION_NORMAL = _ped.PARTITION_NORMAL
PARTITION_LOGICAL = _ped.PARTITION_LOGICAL
PARTITION_EXTENDED = _ped.PARTITION_EXTENDED
PARTITION_FREESPACE = _ped.PARTITION_FREESPACE
PARTITION_METADATA = _ped.PARTITION_METADATA
PARTITION_PROTECTED = _ped.PARTITION_PROTECTED

PARTITION_BOOT = _ped.PARTITION_BOOT
PARTITION_ROOT = _ped.PARTITION_ROOT
PARTITION_SWAP = _ped.PARTITION_SWAP
PARTITION_HIDDEN = _ped.PARTITION_HIDDEN
PARTITION_RAID = _ped.PARTITION_RAID
PARTITION_LVM = _ped.PARTITION_LVM
PARTITION_LBA = _ped.PARTITION_LBA
PARTITION_HPSERVICE = _ped.PARTITION_HPSERVICE
PARTITION_PALO = _ped.PARTITION_PALO
PARTITION_PREP = _ped.PARTITION_PREP
PARTITION_MSFT_RESERVED = _ped.PARTITION_MSFT_RESERVED

DISK_TYPE_EXTENDED = _ped.DISK_TYPE_EXTENDED
DISK_TYPE_PARTITION_NAME = _ped.DISK_TYPE_PARTITION_NAME

class PedDevice:
    def __init__(self):
        warnings.warn("PedDevice class is deprecated by Device", DeprecationWarning)
        # FIXME

class PedDisk:
    def __init__(self):
        warnings.warn("PedDisk class is deprecated by Disk", DeprecationWarning)
        # FIXME

def device_get:
    warnings.warn("device_get() is deprecated and will be removed", DeprecationWarning)
    # FIXME

def disk_type_get:
    warnings.warn("disk_type_get() is deprecated and will be removed", DeprecationWarning)
    # FIXME

def disk_type_get_next:
    warnings.warn("disk_type_get_next() is deprecated and will be removed", DeprecationWarning)
    # FIXME

def error:
    warnings.warn("error() is deprecated and will be removed", DeprecationWarning)
    # FIXME

def exception_set_handler:
    warnings.warn("exception_set_handler() is deprecated and will be removed", DeprecationWarning)
    # FIXME

def file_system_type_get:
    warnings.warn("file_system_type_get() is deprecated and will be removed", DeprecationWarning)
    # FIXME

def file_system_type_get_next:
    warnings.warn("file_system_type_get_next() is deprecated and will be removed", DeprecationWarning)
    # FIXME

def partition_flag_get_by_name:
    warnings.warn("partition_flag_get_by_name() is deprecated and will be removed", DeprecationWarning)
    # FIXME

def partition_flag_get_name:
    warnings.warn("partition_flag_get_name() is deprecated and will be removed", DeprecationWarning)
    # FIXME

def partition_flag_next:
    warnings.warn("partition_flag_next() is deprecated and will be removed", DeprecationWarning)
    # FIXME
