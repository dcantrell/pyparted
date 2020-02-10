#
# __init__.py
# Python bindings for libparted (built on top of the _ped Python module).
#
# Copyright (C) 2007-2015 Red Hat, Inc.
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
# Author(s): David Cantrell <dcantrell@redhat.com>
#            Chris Lumens <clumens@redhat.com>
#            Alex Skinner <alex@lx.lc>
#

from __future__ import division

import math
import platform
import re
import sys
import warnings
import _ped

__all__ = ['Alignment', 'Constraint', 'Device', 'Disk',
           'FileSystem', 'Geometry', 'Partition']

from _ped import AlignmentException
from _ped import CreateException
from _ped import ConstraintException
from _ped import DeviceException
from _ped import DiskException
from _ped import DiskLabelException
from _ped import FileSystemException
from _ped import GeometryException
from _ped import IOException
from _ped import NotNeededException
from _ped import PartedException
from _ped import PartitionException
from _ped import TimerException
from _ped import UnknownDeviceException
from _ped import UnknownTypeException

from _ped import register_exn_handler
from _ped import clear_exn_handler

from parted.alignment import Alignment
from parted.constraint import Constraint
from parted.device import Device
from parted.disk import Disk
from parted.disk import diskType
from parted.disk import diskFlag
from parted.filesystem import FileSystem
from parted.filesystem import fileSystemType
from parted.geometry import Geometry
from parted.partition import Partition
from parted.partition import partitionFlag

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
from _ped import PARTITION_DIAG
from _ped import PARTITION_LEGACY_BOOT
if hasattr(_ped, 'PARTITION_MSFT_DATA'):
    # pylint: disable=E0611
    from _ped import PARTITION_MSFT_DATA
if hasattr(_ped, 'PARTITION_IRST'):
    # pylint: disable=E0611
    from _ped import PARTITION_IRST
if hasattr(_ped, 'PARTITION_ESP'):
    # pylint: disable=E0611
    from _ped import PARTITION_ESP
if hasattr(_ped, 'PARTITION_NONFS'):
    # pylint: disable=E0611
    from _ped import PARTITION_NONFS

from _ped import DISK_CYLINDER_ALIGNMENT
from _ped import DISK_GPT_PMBR_BOOT

from _ped import DISK_TYPE_EXTENDED
from _ped import DISK_TYPE_PARTITION_NAME

from _ped import EXCEPTION_TYPE_INFORMATION
from _ped import EXCEPTION_TYPE_WARNING
from _ped import EXCEPTION_TYPE_ERROR
from _ped import EXCEPTION_TYPE_FATAL
from _ped import EXCEPTION_TYPE_BUG
from _ped import EXCEPTION_TYPE_NO_FEATURE

from _ped import EXCEPTION_RESOLVE_UNHANDLED
from _ped import EXCEPTION_RESOLVE_FIX
from _ped import EXCEPTION_RESOLVE_YES
from _ped import EXCEPTION_RESOLVE_NO
from _ped import EXCEPTION_RESOLVE_OK
from _ped import EXCEPTION_RESOLVE_RETRY
from _ped import EXCEPTION_RESOLVE_IGNORE
from _ped import EXCEPTION_RESOLVE_CANCEL

from _ped import EXCEPTION_OPT_OK_CANCEL
from _ped import EXCEPTION_OPT_YES_NO
from _ped import EXCEPTION_OPT_YES_NO_CANCEL
from _ped import EXCEPTION_OPT_IGNORE_CANCEL
from _ped import EXCEPTION_OPT_RETRY_CANCEL
from _ped import EXCEPTION_OPT_RETRY_IGNORE_CANCEL

from parted.decorators import localeC

if sys.version_info >= (3,):
    string_types = str
else:
    string_types = basestring # pylint: disable=undefined-variable

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

# Refercences:
#
# 1. NIST Special Publication 330, 2008 Edition, Barry N. Taylor and Ambler
#    Thompson, Editors
#    The International System of Units (SI)
#    Available from: http://physics.nist.gov/cuu/pdf/sp811.pdf
#
# 2. International standard IEC 60027-2, third edition,
#    Letter symbols to be used in electrical technology --
#    Part 2: Telecommunications and electronics.
#
# See the links below for quick online summaries:
#
# SI units:  http://physics.nist.gov/cuu/Units/prefixes.html
# IEC units: http://physics.nist.gov/cuu/Units/binary.html
__exponents = {
    "B":    1,       # byte
    "kB":   1000**1, # kilobyte
    "MB":   1000**2, # megabyte
    "GB":   1000**3, # gigabyte
    "TB":   1000**4, # terabyte
    "PB":   1000**5, # petabyte
    "EB":   1000**6, # exabyte
    "ZB":   1000**7, # zettabyte
    "YB":   1000**8, # yottabyte

    "KiB":  1024**1, # kibibyte
    "MiB":  1024**2, # mebibyte
    "GiB":  1024**3, # gibibyte
    "TiB":  1024**4, # tebibyte
    "PiB":  1024**5, # pebibyte
    "EiB":  1024**6, # exbibyte
    "ZiB":  1024**7, # zebibyte
    "YiB":  1024**8  # yobibyte
}

def formatBytes(bytes_, unit):
    """Convert bytes_ using an SI or IEC prefix. Note that unit is a
       case sensitive string that must exactly match one of the IEC or SI
       prefixes followed by 'B' (e.g. 'GB')."""

    if unit not in __exponents.keys():
        raise SyntaxError("{:} is not a valid SI or IEC byte unit".format(unit))
    else:
        return (bytes_ / __exponents[unit])

def sizeToSectors(bytes_, unit, sector_size):
    """Convert bytes_ of unit to a number of sectors. Note that unit is a
       case sensitive string that must exactly match one of the IEC or SI
       prefixes followed by 'B' (e.g. 'GB')."""

    if unit not in __exponents.keys():
        raise SyntaxError("{:} is not a valid SI or IEC byte unit".format(unit))
    else:
        return math.ceil(bytes_ * __exponents[unit] / sector_size)

# Valid disk labels per architecture type.  The list of label
# names map to keys in the parted.diskType hash table.
archLabels = {'i386': ['msdos', 'gpt'],
              's390': ['dasd', 'msdos'],
              'alpha': ['bsd', 'msdos'],
              'sparc': ['sun'],
              'ia64': ['msdos', 'gpt'],
              'ppc': ['msdos', 'mac', 'amiga', 'gpt'],
              'ppc64': ['msdos', 'mac', 'amiga', 'gpt'],
              'ppc64le': ['msdos', 'gpt'],
              'x86_64': ['msdos', 'gpt'],
              'aarch64': ['msdos', 'gpt'],
              'mips64': ['msdos','gpt'],
              'armv7l': ['msdos', 'gpt'],
              'riscv32': ['msdos', 'gpt'],
              'riscv64': ['msdos', 'gpt']}

# Adapted from:
# http://stackoverflow.com/questions/922550/how-to-mark-a-global-as-deprecated-in-python
#
# Remember that DeprecationWarnings are ignored by default as they are not really
# useful to users.  Developers can turn on DeprecationWarning notices by passing
# the -Wd option to python or by setting PYTHONWARNINGS=d in the environment.
def Deprecated(mod, deprecated=None):
    """ Return a wrapped object that warns about deprecated accesses. """

    if not deprecated:
        deprecated = {}

    class Wrapper(object):
        warnmsg = "%s is deprecated and will be removed in a future release."

        def __getattr__(self, attr):
            if attr in deprecated.keys():
                msg = self.warnmsg + " " + deprecated[attr]
                warnings.warn(msg % attr, DeprecationWarning)

            return getattr(mod, attr)

        def __setattr__(self, attr, value):
            if attr in deprecated.keys():
                msg = self.warnmsg + " " + deprecated[attr]
                warnings.warn(msg % attr, DeprecationWarning)
            setattr(mod, attr, value)

    return Wrapper()

# Valid disk labels and their applicable architectures.  The label names map
# to keys in the parted.diskType hash table.
__archLabels = (('amiga', 'ppc(64)?$'),
                ('bsd', 'alpha$'),
                ('dasd', 's390x?$'),
                ('gpt', 'i[3-6]86$|x86_64$|ia64$|ppc(64|64le)?$|aarch64$|armv7l$|riscv(32|64)$'),
                ('mac', 'ppc(64)?$'),
                ('msdos', 'i[3-6]86$|x86_64$|s390x?$|alpha$|ia64$|ppc(64|64le)?$|aarch64$|mips64$|armv7l$|riscv(32|64)$'),
                ('sun', 'sparc(64)?$'))

def getLabels(arch=None):
    """Return a set containing the disk labels compatible with the
       architecture of the computer calling this function. If an architecture
       is passed, return the labels compatible with that architecture."""
    labels = set()
    if arch is None:
        arch = platform.machine()

    for label, regex in __archLabels:
        if re.match(regex, arch):
            labels.add(label)

    return labels

class ReadOnlyProperty(Exception):
    """Exception raised when a write operation occurs on a read-only property."""

    # pylint: disable=W0231
    def __init__(self, prop=''):
        self.message = "%s is a read-only property" % (prop,)

class WriteOnlyProperty(Exception):
    """Exception raised when a read operation occurs on a write-only property."""

    # pylint: disable=W0231
    def __init__(self, prop=''):
        self.message = "%s is a write-only property" % (prop,)

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

    if isinstance(ty, string_types):
        ty = diskType[ty]
    elif not isinstance(ty, DiskType):
        raise TypeError("type must be a key or value in parted.diskType", ty)

    peddisk = disk_new_fresh(device.getPedDevice(), ty)
    return Disk(PedDisk=peddisk)

@localeC
def newDisk(device):
    """Return a Disk object for this Device. Read the partition table off
       a device (if one is found)."""
    from _ped import disk_new

    peddisk = disk_new(device.getPedDevice())
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

# Mark deprecated items
_deprecated = {"partitionTypesDict": "DOS disk label types are not provided "
                                     "by libparted, so the codes are not "
                                     "useful.",
               "_exponent":          "Use __exponents instead.",
               "archLabels":         "Use getLabels() instead.",
              }
sys.modules[__name__] = Deprecated(sys.modules[__name__], _deprecated)
