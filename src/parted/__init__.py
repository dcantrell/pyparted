#
# __init__.py
# Python bindings for libparted (built on top of the _ped Python module).
#
# Copyright (C) 2007, 2008, 2009 Red Hat, Inc.
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
#                    Chris Lumens <clumens@redhat.com>
#

__all__ = ['alignment', 'constraint', 'device', 'disk',
           'filesystem', 'geometry', 'partition']

import _ped
from alignment import Alignment
from constraint import Constraint
from device import Device
from disk import Disk
from disk import diskType
from filesystem import FileSystem
from filesystem import fileSystemType
from geometry import Geometry
from partition import Partition
from partition import partitionFlag

# the enumerated types in _ped need to be available from here too
UNIT_SECTOR              = _ped.UNIT_SECTOR
UNIT_BYTE                = _ped.UNIT_BYTE
UNIT_KILOBYTE            = _ped.UNIT_KILOBYTE
UNIT_MEGABYTE            = _ped.UNIT_MEGABYTE
UNIT_GIGABYTE            = _ped.UNIT_GIGABYTE
UNIT_TERABYTE            = _ped.UNIT_TERABYTE
UNIT_COMPACT             = _ped.UNIT_COMPACT
UNIT_CYLINDER            = _ped.UNIT_CYLINDER
UNIT_CHS                 = _ped.UNIT_CHS
UNIT_PERCENT             = _ped.UNIT_PERCENT
UNIT_KIBIBYTE            = _ped.UNIT_KIBIBYTE
UNIT_MEBIBYTE            = _ped.UNIT_MEBIBYTE
UNIT_GIBIBYTE            = _ped.UNIT_GIBIBYTE
UNIT_TEBIBYTE            = _ped.UNIT_TEBIBYTE

DEVICE_UNKNOWN           = _ped.DEVICE_UNKNOWN
DEVICE_SCSI              = _ped.DEVICE_SCSI
DEVICE_IDE               = _ped.DEVICE_IDE
DEVICE_DAC960            = _ped.DEVICE_DAC960
DEVICE_CPQARRAY          = _ped.DEVICE_CPQARRAY
DEVICE_FILE              = _ped.DEVICE_FILE
DEVICE_ATARAID           = _ped.DEVICE_ATARAID
DEVICE_I2O               = _ped.DEVICE_I2O
DEVICE_UBD               = _ped.DEVICE_UBD
DEVICE_DASD              = _ped.DEVICE_DASD
DEVICE_VIODASD           = _ped.DEVICE_VIODASD
DEVICE_SX8               = _ped.DEVICE_SX8
DEVICE_DM                = _ped.DEVICE_DM
DEVICE_XVD               = _ped.DEVICE_XVD

PARTITION_NORMAL         = _ped.PARTITION_NORMAL
PARTITION_LOGICAL        = _ped.PARTITION_LOGICAL
PARTITION_EXTENDED       = _ped.PARTITION_EXTENDED
PARTITION_FREESPACE      = _ped.PARTITION_FREESPACE
PARTITION_METADATA       = _ped.PARTITION_METADATA
PARTITION_PROTECTED      = _ped.PARTITION_PROTECTED

PARTITION_BOOT           = _ped.PARTITION_BOOT
PARTITION_ROOT           = _ped.PARTITION_ROOT
PARTITION_SWAP           = _ped.PARTITION_SWAP
PARTITION_HIDDEN         = _ped.PARTITION_HIDDEN
PARTITION_RAID           = _ped.PARTITION_RAID
PARTITION_LVM            = _ped.PARTITION_LVM
PARTITION_LBA            = _ped.PARTITION_LBA
PARTITION_HPSERVICE      = _ped.PARTITION_HPSERVICE
PARTITION_PALO           = _ped.PARTITION_PALO
PARTITION_PREP           = _ped.PARTITION_PREP
PARTITION_MSFT_RESERVED  = _ped.PARTITION_MSFT_RESERVED

DISK_TYPE_EXTENDED       = _ped.DISK_TYPE_EXTENDED
DISK_TYPE_PARTITION_NAME = _ped.DISK_TYPE_PARTITION_NAME

partitionTypesDict = {
    0x00: "Empty",
    0x01: "DOS 12-bit FAT",
    0x02: "XENIX root",
    0x03: "XENIX usr",
    0x04: "DOS 16-bit <32M",
    0x05: "Extended",
    0x06: "DOS 16-bit >=32M",
    0x07: "NTFS/HPFS",
    0x08: "AIX",
    0x09: "AIX bootable",
    0x0a: "OS/2 Boot Manager",
    0x0b: "Win95 FAT32",
    0x0c: "Win95 FAT32",
    0x0e: "Win95 FAT16",
    0x0f: "Win95 Ext'd",
    0x10: "OPUS",
    0x11: "Hidden FAT12",
    0x12: "Compaq Setup",
    0x14: "Hidden FAT16 <32M",
    0x16: "Hidden FAT16",
    0x17: "Hidden HPFS/NTFS",
    0x18: "AST SmartSleep",
    0x1b: "Hidden Win95 FAT32",
    0x1c: "Hidden Win95 FAT32 (LBA)",
    0x1e: "Hidden Win95 FAT16 (LBA)",
    0x24: "NEC_DOS",
    0x39: "Plan 9",
    0x40: "Venix 80286",
    0x41: "PPC_PReP Boot",
    0x42: "SFS",
    0x4d: "QNX4.x",
    0x4e: "QNX4.x 2nd part",
    0x4f: "QNX4.x 2nd part",
    0x51: "Novell?",
    0x52: "Microport",
    0x63: "GNU HURD",
    0x64: "Novell Netware 286",
    0x65: "Novell Netware 386",
    0x75: "PC/IX",
    0x80: "Old MINIX",
    0x81: "Linux/MINIX",
    0x82: "Linux swap",
    0x83: "Linux native",
    0x84: "OS/2 hidden C:",
    0x85: "Linux Extended",
    0x86: "NTFS volume set",
    0x87: "NTFS volume set",
    0x8e: "Linux LVM",
    0x93: "Amoeba",
    0x94: "Amoeba BBT",
    0x9f: "BSD/OS",
    0xa0: "IBM Thinkpad hibernation",
    0xa5: "BSD/386",
    0xa6: "OpenBSD",
    0xb7: "BSDI fs",
    0xb8: "BSDI swap",
    0xbf: "Solaris",
    0xc7: "Syrinx",
    0xdb: "CP/M",
    0xde: "Dell Utility",
    0xe1: "DOS access",
    0xe3: "DOS R/O",
    0xeb: "BEOS",
    0xee: "EFI GPT",
    0xef: "EFI (FAT-12/16/32)",
    0xf2: "DOS secondary",
    0xfd: "Linux RAID",
    0xff: "BBT"
}

# Exponents for 1024 used when converting sizes to byte-sized
# units for display.  The keys are:
#   b     bytes       1024^0 = 1
#   kb    kilobytes   1024^1 = 1024
#   mb    megabytes   1024^2 = 1048576
#   gb    gigabytes   1024^3 = 1073741824
#   tb    terabytes   1024^4 = 1099511627776
# The resulting value for 1024 raised to the power is used as
# the divisor for conversion functions.
_exponent = {'b': 0, 'kb': 1, 'mb': 2, 'gb': 3, 'tb': 4}

# Valid disk labels per architecture type.  The list of label
# names map to keys in the parted.diskType hash table.
archLabels = {'i386': ['msdos', 'gpt'],
              's390': ['dasd', 'msdos'],
              'alpha': ['bsd', 'msdos'],
              'sparc': ['sun'],
              'ia64': ['msdos', 'gpt'],
              'ppc': ['msdos', 'mac', 'amiga', 'gpt'],
              'x86_64': ['msdos', 'gpt']}

class ReadOnlyProperty(Exception):
    """Exception raised when a write operation occurs on a read-only property."""

    def __init__(self, property=''):
        self.message = "%s is a read-only property" % (property,)

class WriteOnlyProperty(Exception):
    """Exception raised when a read operation occurs on a write-only property."""

    def __init__(self, property=''):
        self.message = "%s is a write-only property" % (property,)

def divRoundUp(numerator, divisor):
    """Rounds up the result of numerator/divisor."""
    return _ped.div_round_up(numerator, divisor)

def divRoundToNearest(numerator, divisor):
    """Returns the closest result of numerator/divisor."""
    return _ped.div_round_to_nearest(numerator, divisor)

def getDevice(path):
    """Given the operating system level path to a device node, return
       a Device object for that disk.  Raises DiskException if an invalid
       path is given."""
    return Device(path=path)

def getAllDevices():
    """Return a list of Device objects for all devices in the system."""
    lst = []
    device = None

    _ped.device_probe_all()

    while True:
        try:
            if not device:
                device = _ped.device_get_next()
            else:
                device = _ped.device_get_next(device)

            lst.append(Device(PedDevice=device))
        except IndexError:
            return lst

def greatestCommonDivisor(a, b):
    """Return the largest divisor of both a and b."""
    return _ped.greatest_common_divisor(a, b)

def probeForSpecificFileSystem(fstype, geometry):
    """Call the _ped.file_system_probe_specific() function given the
       filesystem type and geometry.  fstype must be a string
       representing a valid _ped.FileSystemType, geometry is a
       parted.Geometry."""
    geom = _ped.file_system_probe_specific(filesystem.fileSystemType[fstype], geometry.getPedGeometry())
    return geometry.Geometry(PedGeometry=geom)

def probeFileSystem(geometry):
    fstype = _ped.file_system_probe(geometry.getPedGeometry())
    return fstype.name

def roundUpTo(sector, grainSize):
    """Rounds sector up to the closest multiple of grainSize."""
    return _ped.round_up_to(sector, grainSize)

def roundDownTo(sector, grainSize):
    """Rounds sector down to the closest multiple of grainSize."""
    return _ped.round_down_to(sector, grainSize)

def roundToNearest(sector, grainSize):
    """Rounds sector to the closest multiple of grainSize."""
    return _ped.round_to_nearest(sector, grainSize)

def getPartitionByName(name):
    """Return a Partition object associated with the partition name.
       name is a device node specification, such as /dev/sda1
       Returns None if no partition is found."""
    for device in getAllDevices():
        disk = Disk(device=device)
        for partition in disk.partitions:
            if partition.getDeviceNodeName() == name:
                return partition

    return None

def freshDisk(device, type):
    """Return a Disk object for this Device and using this DiskType.
       The type should be a member of the parted.diskType hash,
       either a key or a value.

       The new label is not written to disk until commitToDevice()
       is called on the Disk."""
    if (type not in diskType.keys()) and (type not in diskType.values()):
        raise SyntaxError, "type must be a key or value in parted.diskType"

    if type in diskType.keys():
        type = diskType[type]

    peddisk = _ped.disk_new_fresh(device.getPedDevice(), type)
    return disk.Disk(PedDisk=peddisk)

def version():
    """Return a dict containing the pyparted and libparted versions."""
    ver = {}
    ver['libparted'] = _ped.libparted_version()
    ver['pyparted'] = _ped.pyparted_version()
    return ver
