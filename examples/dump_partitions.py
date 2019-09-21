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
print the partitions in a device/file according to pyparted
"""

import parted

# adjust as needed, eg /dev/sdc..
device="sdcard.img"

# A device is a "thing" with own properties (type, manufacturer etc) - its a
# high level abstraction that lets you just identify you have the right device
device = parted.getDevice(device)

# grab a "disk" instance - the other choice is `freshDisk`, wheras `newDisk`
# seems to just read an existing disk (which is what we want for adding new
# partition)
disk = parted.newDisk(device)

print("***** sanity check *****")
print(f"result: {disk.check()}")

print("===== device ====")
print(f"  model: {device.model}")
print(f"  path: {device.path}")
print(f"  sectorSize: {device.sectorSize}")
print(f"  physicalSectorSize: {device.physicalSectorSize}")
print(f"  length: {device.length}")
# this next line can crash if you have a wonky GPT...
# print(f"  size (MB): {device.getSize()}")

print("===== disk ====")
print(f"  type: {disk.type}")
print(f"  lastPartitionNumber: {disk.lastPartitionNumber}")
print(f"  primaryPartitionCount: {disk.primaryPartitionCount}")
print( "  free space regions:")
free_space_regions = disk.getFreeSpaceRegions()
for i, free_space_region in enumerate(free_space_regions):
    print(f"    {i}:")
    print(f"      start: {free_space_region.start}")
    print(f"      end: {free_space_region.end}")
    print(f"      length: {free_space_region.length}")


print("===== partitions ====")
for partition in disk.partitions:
    # print(str(partition)) - gives a nice human-readable dump
    print(f"Partition {partition.number}:")
    print(f"  length: {partition.getLength()}")
    print(f"  active: {partition.active}")
    print(f"  busy: {partition.busy}")
    print(f"  path: {partition.path}")
    print(f"  type: {partition.type}")
    print(f"  size(MB): {partition.getSize()}")

    # supported only by GPT partition tables
    # will throw exception on MSDOS partition tables (fixed upstream)
    print(f"  name: {partition.name}")
    filesystem = partition.fileSystem
    print(f"  filesystem:")
    if filesystem:
        print(f"    type: {filesystem.type}")
        print(f"    checked: {filesystem.checked}")
    else:
        print(f"    Filesystem info missing! you have an unformatted partition...")

    geometry = partition.geometry
    print(f"  geometry:")
    print(f"    start: {geometry.start}")
    print(f"    end: {geometry.end}")
    print(f"    length: {geometry.length}")
