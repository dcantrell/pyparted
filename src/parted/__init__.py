#
# __init__.py
# Python bindings for libparted (built on top of the _ped Python module).
#
# Copyright (C) 2007, 2008, 2009  Red Hat, Inc.
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

from _ped import AlignmentException
from _ped import CreateException
from _ped import ConstraintException
from _ped import DeviceException
from _ped import DiskException
from _ped import FileSystemException
from _ped import GeometryException
from _ped import IOException
from _ped import NotNeededException
from _ped import PartedException
from _ped import PartitionException
from _ped import TimerException
from _ped import UnknownDeviceException
from _ped import UnknownTypeException

from alignment import Alignment
from constraint import Constraint
from device import Device
from disk import Disk
from disk import diskType
from disk import diskFlag
from filesystem import FileSystem
from filesystem import fileSystemType
from geometry import Geometry
from partition import Partition
from partition import partitionFlag

# the enumerated types in _ped need to be available from here too
from _ped import UNIT_SECTOR
from _ped import UNIT_BYTE
from _ped import UNIT_KILOBYTE
from _ped import UNIT_MEGABYTE
from _ped import UNIT_GIGABYTE
from _ped import UNIT_TERABYTE
from _ped import UNIT_COMPACT
from _ped import UNIT_CYLINDER
from _ped import UNIT_CHS
from _ped import UNIT_PERCENT
from _ped import UNIT_KIBIBYTE
from _ped import UNIT_MEBIBYTE
from _ped import UNIT_GIBIBYTE
from _ped import UNIT_TEBIBYTE

from _ped import DEVICE_UNKNOWN
from _ped import DEVICE_SCSI
from _ped import DEVICE_IDE
from _ped import DEVICE_DAC960
from _ped import DEVICE_CPQARRAY
from _ped import DEVICE_FILE
from _ped import DEVICE_ATARAID
from _ped import DEVICE_I2O
from _ped import DEVICE_UBD
from _ped import DEVICE_DASD
from _ped import DEVICE_VIODASD
from _ped import DEVICE_SX8
from _ped import DEVICE_DM
from _ped import DEVICE_XVD
from _ped import DEVICE_SDMMC
from _ped import DEVICE_VIRTBLK

from _ped import PARTITION_NORMAL
from _ped import PARTITION_LOGICAL
from _ped import PARTITION_EXTENDED
from _ped import PARTITION_FREESPACE
from _ped import PARTITION_METADATA
from _ped import PARTITION_PROTECTED

from _ped import PARTITION_BOOT
from _ped import PARTITION_ROOT
from _ped import PARTITION_SWAP
from _ped import PARTITION_HIDDEN
from _ped import PARTITION_RAID
from _ped import PARTITION_LVM
from _ped import PARTITION_LBA
from _ped import PARTITION_HPSERVICE
from _ped import PARTITION_PALO
from _ped import PARTITION_PREP
from _ped import PARTITION_MSFT_RESERVED
from _ped import PARTITION_APPLE_TV_RECOVERY
from _ped import PARTITION_BIOS_GRUB

from _ped import DISK_CYLINDER_ALIGNMENT

from _ped import DISK_TYPE_EXTENDED
from _ped import DISK_TYPE_PARTITION_NAME

from decorators import localeC

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
#   pb    petabytes   1024^5 = 1125899906842624
#   eb    exabytes    1024^6 = 1152921504606846976
#   zb    zettabytes  1024^7 = 1180591620717411303424
#   yb    yottabytes  1024^8 = 1208925819614629174706176
# The resulting value for 1024 raised to the power is used as
# the divisor for conversion functions.
_exponent = {'b': 0, 'kb': 1, 'mb': 2, 'gb': 3, 'tb': 4,
             'pb': 5, 'eb': 6, 'zb': 7, 'yb': 8}

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

@localeC
def getDevice(path):
    """Given the operating system level path to a device node, return
       a Device object for that disk.  Raises DeviceException if an invalid
       path is given."""
    return Device(path=path)

@localeC
def getAllDevices():
    """Return a list of Device objects for all devices in the system."""
    from _ped import device_probe_all
    from _ped import device_get_next

    lst = []
    device = None

    device_probe_all()

    while True:
        try:
            if not device:
                device = device_get_next()
            else:
                device = device_get_next(device)

            lst.append(Device(PedDevice=device))
        except IndexError:
            return lst

@localeC
def freeAllDevices():
    """Free all Device objects.  There is no reason to call this function."""
    from _ped import device_free_all
    return device_free_all()

@localeC
def probeForSpecificFileSystem(fstype, geometry):
    """Call the _ped.file_system_probe_specific() function given the
       filesystem type and geometry.  fstype must be a string
       representing a valid _ped.FileSystemType, geometry is a
       parted.Geometry."""
    from _ped import file_system_probe_specific
    geom = file_system_probe_specific(fileSystemType[fstype], geometry.getPedGeometry())
    return geometry.Geometry(PedGeometry=geom)

@localeC
def probeFileSystem(geometry):
    """Return the name of the filesystem detected on the given
       Geometry.  Returns None is no filesystem found."""
    from _ped import file_system_probe
    fstype = file_system_probe(geometry.getPedGeometry())
    return fstype.name

@localeC
def freshDisk(device, ty):
    """Return a Disk object for this Device and using this DiskType.
       The type should be a member of the parted.diskType hash,
       either a key or a value.

       The new label is not written to disk until commitToDevice()
       is called on the Disk."""
    from _ped import disk_new_fresh, DiskType

    if type(ty) == str:
        ty = diskType[ty]
    elif not isinstance(ty, DiskType):
        raise SyntaxError, "type must be a key or value in parted.diskType"

    peddisk = disk_new_fresh(device.getPedDevice(), ty)
    return Disk(PedDisk=peddisk)

@localeC
def version():
    """Return a dict containing the pyparted and libparted versions."""
    from _ped import libparted_version
    from _ped import pyparted_version

    ver = {}
    ver['libparted'] = libparted_version()
    ver['pyparted'] = pyparted_version()
    return ver
