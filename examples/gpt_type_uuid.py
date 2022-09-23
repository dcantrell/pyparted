#!/usr/bin/env python3
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

"""
print and change the GPT partition type UUID
"""

import parted
import sys
import uuid

# adjust as needed, eg /dev/sdc..
device="sdcard.img"

if len(sys.argv) not in (3, 4):
    print(f"{sys.argv[0]}: DEVICE PART-NUM [NEW-UUID]")
    sys.exit(1)

devpath = sys.argv[1]
partnum = int(sys.argv[2])
newuuid = None
if len(sys.argv) == 4:
    newuuid = uuid.UUID(sys.argv[3])

device = parted.getDevice(devpath)
disk = parted.newDisk(device)

if partnum == 0 or partnum > len(disk.partitions):
    print(f"{sys.argv[0]}: partition {partnum} must be in range 1:{len(disk.partitions)}")
    sys.exit(1)

part = disk.partitions[partnum-1]

if not hasattr(parted, "DISK_TYPE_PARTITION_TYPE_UUID"):
    print(f"{sys.argv[0]}: build against parted version >= 3.5 is required")
    sys.exit(1)

if not disk.supportsFeature(parted.DISK_TYPE_PARTITION_TYPE_UUID):
    print(f"{sys.argv[0]}: disk label {disk.type} does not support partition type UUIDs")
    sys.exit(1)

if newuuid is not None:
    part.type_uuid = newuuid.bytes
    disk.commit()

print(str(uuid.UUID(bytes=part.type_uuid)))
