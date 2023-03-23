#
# fdisk.py
# Partitioning utility with an fdisk-style interface, but written in
# Python and using the pyparted Python module.
#
# Copyright The pyparted Project Authors
# SPDX-License-Identifier: GPL-2.0-or-later
#

import getopt
import os
import parted
import sys

def usage(cmd):
    sys.stdout.write("Usage: %s [-b SSZ] [-u] DISK       Change partition table\n" % (cmd,))
    sys.stdout.write("       %s [-l] [-b SSZ] [-u] DISK  List partition table(s)\n" % (cmd,))
    sys.stdout.write("       %s -s PARTITION             Give partition size(s) in blocks\n\n" % (cmd,))
    sys.stdout.write("Here DISK is something like /dev/hdb or /dev/sda\n")
    sys.stdout.write("and PARTITION is something like /dev/hda7\n\n")
    sys.stdout.write("Options:\n")
    sys.stdout.write("   -b=SSZ, --sectorsize=SSZ    Sector size (in bytes) to use\n")
    sys.stdout.write("   -l, --list                  List partition table(s)\n")
    sys.stdout.write("   -u, --showsectors           Give Start and End in sector units\n")
    sys.stdout.write("   -s, --showblocks            Give Start and End in block units\n")
    sys.stdout.write("   -V, --version               Show fdisk version\n")
    sys.stdout.write("   -?, --help                  Display fdisk usage screen\n")

def displayVersion(cmd):
    ver = parted.version()

    sys.stdout.write("%s:\n" % (cmd,))
    sys.stdout.write("pyparted version: %s.%s.%s\n" % (ver["pyparted"][0], ver["pyparted"][1], ver["pyparted"][2]))
    sys.stdout.write("libparted version: %s\n" % ver["libparted"])

def listPartitionTable(path, sectorsize, showsectors, showblocks):
    device = parted.getDevice(path)
    (cylinders, heads, sectors) = device.biosGeometry
    sizeInBytes = device.length * device.sectorSize
    disk = parted.Disk(device)

    partlist = []
    for partition in disk.partitions:
        if partition.type == parted.PARTITION_PROTECTED or \
           partition.type == parted.PARTITION_METADATA or \
           partition.type == parted.PARTITION_FREESPACE:
            continue

        partlist.append((partition,
                         partition.path,
                         partition.getFlag(parted.PARTITION_BOOT),
                         partition.geometry.start,
                         partition.geometry.end,
                         partition.geometry.length,
                         partition.type,
                         partition.fileSystem))

    colLength = 0
    for parts in partlist:
        path = parts[1]
        if len(path) > colLength:
            colLength = len(path)

    sys.stdout.write("Disk %s: %%d %%s, %d bytes\n" % (disk.device.path, sizeInBytes))
    sys.stdout.write("%d heads, %d sectors/track, %d cylinders\n" % (heads, sectors, cylinders,))
    sys.stdout.write("Units = FIXME\n")
    sys.stdout.write("Disk identifier: FIXME\n\n")

    sys.stdout.write("%-11s %-4s %-11s %-11s %-12s %-4s %s\n" % ("Device", "Boot", "Start", "End", "Blocks", "Id", "System",))

    for parts in partlist:
        (partition, path, bootable, start, end, length, ty, fs) = parts

        if bootable:
            bootflag = '*'
        else:
            bootflag = ''

        sys.stdout.write("%-11s %-4s %-11d %-11d %-12d %-4s" % (path, bootflag, start, end, length, ty,))

        if fs is None:
            # no filesystem, check flags
            if partition.getFlag(parted.PARTITION_SWAP):
                sys.stdout.write(" Linux swap\n")
            elif partition.getFlag(parted.PARTITION_RAID):
                sys.stdout.write(" RAID\n")
            elif partition.getFlag(parted.PARTITION_LVM):
                sys.stdout.write(" Linux LVM\n")
            elif partition.getFlag(parted.PARTITION_HPSERVICE):
                sys.stdout.write(" HP Service\n")
            elif partition.getFlag(parted.PARTITION_PALO):
                sys.stdout.write(" PALO\n")
            elif partition.getFlag(parted.PARTITION_PREP):
                sys.stdout.write(" PReP\n")
            elif partition.getFlag(parted.MSFT_RESERVED):
                sys.stdout.write(" MSFT Reserved\n")
            else:
                sys.stdout.write(" unknown\n")
        else:
            sys.stdout.write(" %s\n" % (fs.type,))

#Disk /dev/sda: 250.0 GB, 250059350016 bytes
#255 heads, 63 sectors/track, 30401 cylinders
#Units = cylinders of 16065 * 512 = 8225280 bytes
#Disk identifier: 0x00000000
#
#   Device Boot      Start         End      Blocks   Id  System
#/dev/sda1               1          26      204819+  ee  GPT
#/dev/sda2              26        4203    33554432   af  Unknown
#/dev/sda3   *        4203        4229      204800   83  Linux
#/dev/sda4            4229       30402   210234515+  8e  Linux LVM

def main(argv):
    cmd = os.path.basename(sys.argv[0])
    opts, args = [], []
    showhelp, showlist, showsectors, showblocks = False, False, False, False

    # These three are unused for now so I'm marking them with an underscore
    # to make pylint happy.
    sectorsize, _cylinders, _heads, _sectors = None, None, None, None

    if len(sys.argv) == 1:
        showhelp = True

    try:
        opts, args = getopt.getopt(sys.argv[1:], "lb:C:H:S:usV?",
                                   ["list", "sectorsize=", "cylinders=",
                                    "heads=", "sectors=", "showsectors",
                                    "showblocks", "version", "help"])
    except getopt.GetoptError:
        showhelp = True

    for o, a in opts:
        if o in ('-l', '--list'):
            showlist = True
        elif o in ('-b', '--sectorsize'):
            sectorsize = a
        elif o in ('-C', '--cylinders'):
            _cylinders = a
        elif o in ('-H', '--heads'):
            _heads = a
        elif o in ('-S', '--sectors'):
            _sectors = a
        elif o in ('-u', '--showsectors'):
            showsectors = True
        elif o in ('-s', '--showblocks'):
            showblocks = True
        elif o in ('-V', '--version'):
            displayVersion(cmd)
            sys.exit(0)
        elif o in ('-?', '--help'):
            usage(cmd)
            sys.exit(0)
        else:
            sys.stderr.write("Invalid option: %s\n\n" % (o,))
            showhelp = True

    if showhelp:
        usage(cmd)
        sys.exit(1)

    for arg in args:
        if showlist:
            listPartitionTable(arg, sectorsize, showsectors, showblocks)

if __name__ == "__main__":
    main(sys.argv)
