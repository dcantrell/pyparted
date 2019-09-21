#!/usr/bin/env python3
#
# Copyright 2019 Geoff Williams <geoff@declarativesystems.com>
#
# This example script is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option) any
# later version so long as this copyright notice remains intact.
#
# This example script is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
# details.
#
# You should have received a copy of the GNU General Public License along with
# this example script.  If not, see <http://www.gnu.org/licenses/>.

__author__ = """Geoff Williams <geoff@declarativesystems.com>"""
__copyright__ = """Copyright 2019 Geoff Williams"""

"""
Allocate free space to a partition. This demo is setup for a raw image file
`sdcard.img` which is assumed to already contain partitions but with some
unallocated space at the end of the drive. If you have an image file that you
have _grown_ for this purpose (eg with `dd` or `truncate`) and it is GPT, you
need to run:

  sgdisk -e FILE

before running this script since GPT demands 33 backup blocks at the end of
the disk. If you forget to do this you will get errors like this:

  File "/home/geoff/.local/lib/python3.7/site-packages/parted/decorators.py", line 42, in new
    ret = fn(*args, **kwds)
  File "/home/geoff/.local/lib/python3.7/site-packages/parted/disk.py", line 245, in addPartition
    constraint.getPedConstraint())
_ped.PartitionException: Unable to satisfy all constraints on the partition.

Lets grow our SD card image:
  $ dd if=/dev/zero bs=1M count=100 >> sdcard.img
  $ sgdisk -e sdcard.img
"""

# pip3 install pyparted
import parted

# adjust as needed, eg /dev/sdc..
device="sdcard.img"
device = parted.getDevice(device)
disk = parted.newDisk(device)
free_space_regions = disk.getFreeSpaceRegions()

# grab the free space region at the end of the disk
geometry = free_space_regions[-1]

# filesystem will be shown as blank by parted until it has been _formatted_
filesystem = parted.FileSystem(type='ext4', geometry=geometry)
new_partition = parted.Partition(
    disk=disk,
    type=parted.PARTITION_NORMAL,
    fs = filesystem,
    geometry=geometry
)

# name isn't in the constructor but we can set it separately
new_partition.name = "bob"

disk.addPartition(partition=new_partition,
                  constraint=device.optimalAlignedConstraint)

disk.commit()
print("created partition OK")

# Now you can format the partition in the image file after setting it up as a
# loopback device, eg:
#   sudo losetup -f -P  sdcard.img
#   sudo mkfs.ext4 /dev/loop6p3
# Partitions are `1` indexed so `p3` is the third partition, not the fourth
# After formatting, the filesystem will be shown as `ext4` in `parted` and the
# new partition is ready to use.
#
# Cleanup your loopback device:
#   sudo losetup -d /dev/loop6
