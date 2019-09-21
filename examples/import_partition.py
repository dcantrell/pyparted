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
Import a filesystem dump from file `new_partition.img` into a raw GPT image
`sdcard.img` as a new partition.
"""

# pip3 install pyparted
import parted
import pprint
import subprocess
import os

def pad_block(image_file):
    """
    Pad a block to the nearest 1MiB (2048 sectors)
    """
    # how many sectors (LBAs) does the whole disk use right now?
    sector_count = int(os.path.getsize(image_file) / 512)

    # partitions need to start at 1MiB offsets
    sector_alignment = sector_count % 2048

    if sector_alignment == 0:
        print("PADDING: already OK")
    else:
        padding_needed = 2048 - (sector_count % 2048)

        print(f"Padding {padding_needed} sectors to fill 1MiB block")
        with open(image_file, 'ab') as file:
            file.write(bytearray(padding_needed * 512))

        new_sectors = int(os.path.getsize(image_file) / 512)
        print(f"Padded {sector_count} --> {new_sectors}")


# Example using a raw image file created with `dd` (already contains partitions)
sdcard="sdcard.img"

# 100MiB file created with `dd`, formatted ext4
new_partition_file = "new_partition.img"

# Grab the sizes of the SD card and the new partition before we start work
new_partition_size = os.path.getsize(new_partition_file)
new_partition_size_sectors = int(new_partition_size/512)
sdcard_original_size = os.path.getsize(sdcard)
original_sectors = int(sdcard_original_size/512)


# GPT stores a backup at the end of the drive that uses 33 blocks of 512 byte
# sectors - see https://en.wikipedia.org/wiki/GUID_Partition_Table and this
# second copy is required to both exist and be in the right place for the disk
# to be readable. If we just append our new partition, we can use `sgdisk -e` to
# move the backup back to the end of the disk, however, it will leave a gap that
# breaks both our alignment and starting sector calculation so we will write out
# the current GPT to a backup file, pad the image, append our new filesystem,
# pad the imagage again and restore the backup GPT to give us a layout like:
#
# | existing data            |
# | padding to 1MiB boundary |
# | ------------------------ |
# | new partition            |
# | padding to 1MiB boundary |
# | ------------------------ |
# | restored GPT             |

gpt_backup = "gpt.dat"
gpt_offset = -1
gpt_size = 33 * 512
print("Taking backup of GPT")
with open(sdcard, 'rb') as file:

    # seek to the start of the GPT data 33 LBA from the end of the disk, then
    # read it into a `gpt_data`
    file.seek(- gpt_size, os.SEEK_END)  # Note minus sign, doesn't work in `wb` mode(?)
    gpt_offset = file.tell();
    print(f"GPT offset: {gpt_offset}")
    gpt_data = file.read()

    # sanity check that we read the right amount of data
    gpt_data_len = len(gpt_data)
    if gpt_data_len != gpt_size:
        raise ValueError(f"GPT backup bad: {gpt_data_len} != {gpt_size}")

    # write out our GPT backup
    with open(gpt_backup, 'wb') as g:
        g.write(gpt_data)

    # sanity check we wrote the right amount of data
    gpt_backup_len = os.path.getsize(gpt_backup)
    if gpt_backup_len != gpt_size:
        raise ValueError(f"GPT backup bad: {gpt_backup_len} != {gpt_size}")

# Truncate the last 33 LBA (GPT backup)
with open(sdcard, 'rb+') as file:
    file.truncate(gpt_offset)

new_sectors = int(os.path.getsize(sdcard)/512)

if (original_sectors - new_sectors) != 33:
    raise ValueError(f"bad truncation {original_sectors} - {new_sectors} != 33")

# now that the GPT is gone, pad out the image file ready for the new partition
# and record the resulting image size. This gives us the starting point for our
# new partition aligned to a 1MiB boundary
pad_block(sdcard)
partition_insertion_point =  os.path.getsize(sdcard)
partition_insertion_point_sector = int(partition_insertion_point / 512)
subprocess.run(f"cat {new_partition_file} >> {sdcard}", shell=True, capture_output=True, check=True)

# pad out the block after writing our new partition
pad_block(sdcard)

# Restore the backup GPT
subprocess.run(f"cat {gpt_backup} >> {sdcard}", shell=True, capture_output=True, check=True)

# Fix the backup GPT by "moving" it to end of disk (its already there but
# contains checksums that needs recomputing as well as pointers to its position
# on disk)
subprocess.run(f"sgdisk -e {sdcard}", shell=True, capture_output=True, check=True)


# Now the disk image contains the new partition and the GPT is adjusted for the
# new free space. We just have to add our new partition and its ready to use
device = parted.getDevice(sdcard)
disk = parted.newDisk(device)

geometry = parted.Geometry(
    device,
    start=partition_insertion_point_sector,
    length=new_partition_size_sectors
)

print(f"New partition geometry:\n{geometry}")

filesystem = parted.FileSystem(type='ext2', geometry=geometry)
new_partition = parted.Partition(
    disk=disk,
    type=parted.PARTITION_NORMAL,
    fs = filesystem,
    geometry=geometry
)
# name isn't in the constructor but we can set it separately
new_partition.name = "myimportedpartition"
disk.addPartition(partition=new_partition,
                  constraint=parted.Constraint(exactGeom=geometry))

disk.commit()
print("created partition OK")
