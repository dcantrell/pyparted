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

import _ped

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

def getDisk(path):
    """Given the operating system level path to a device node, return
       a Disk object for that disk.  Raises DiskException if an invalid
       path is given."""
    return _ped.device_get(path)

def getAllDisks():
    """Return a list of Disk objects for all disks in the system."""
    lst = []
    disk = None

    _ped.device_probe_all()

    while True:
        try:
            if not disk:
                disk = _ped.device_get_next()
            else:
                disk = _ped.device_get_next(disk)

            lst.append(disk)
        except IndexError:
            return lst

def version():
    """Return a dict containing the pyparted and libparted versions."""
    ver = {}
    ver['libparted'] = _ped.libparted_version()
    ver['pyparted'] = _ped.pyparted_version()
    return ver
