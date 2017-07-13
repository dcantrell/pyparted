# coding=utf-8
#
# Copyright 2017 John Florian <jflorian@doubledog.org>
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

__author__ = """John Florian <jflorian@doubledog.org>"""
__copyright__ = """Copyright 2017 John Florian"""

"""
An example program that queries the capacity of a device that supports
removable media, such as CompactFlash card reader.
"""
import parted

device_name = '/dev/sdd'


def to_mebibytes(value):
    return value / (2 ** 20)

def to_megabytes(value):
    return value / (10 ** 6)

while True:
     print('press Enter after inserting a new card and letting things settle')
     input()
     dev = parted.getDevice(device_name)
     capacity = dev.length * dev.sectorSize
     print(
        '{} has a capacity of {:n} bytes '
        '({:n} MB or {:n} MiB)'.format(
            device_name,
            capacity,
            to_megabytes(capacity),
            to_mebibytes(capacity),
        )
    )

    # Important!  parted caches details so it's necessary to invalidate its
    # cache to ensure accurate results when using removable media.  See
    # https://github.com/dcantrell/pyparted/issues/33 for more details.
    dev.removeFromCache()
