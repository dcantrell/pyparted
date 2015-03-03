# coding=utf-8
#
# Copyright 2015 John Florian <jflorian@doubledog.org>
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
An example program that creates a single, bootable primary partition on a disk.
"""
from glob import glob
from logging import getLogger
import logging

import parted


__author__ = """John Florian <john.florian@dart.biz>"""
__copyright__ = """Copyright 2015 John Florian"""


class _ConsoleHandler(logging.StreamHandler):
    def __init__(self):
        super().__init__()
        self.setFormatter(
            logging.Formatter('{levelname} - {message}', style='{')
        )


class ExampleDevice(object):
    """
    A simple model of a block storage device that wraps up some examples of
    pyparted capabilities.
    """

    def __init__(self, path):
        """
        Initialize the ExampleDevice object.
        """
        self.path = path
        self.logger = getLogger(__name__)

    @property
    def partition_names(self):
        """
        @return:    A list of partition device names on the block device.
        @rtype:     str
        """
        names = glob('{}[0-9]*'.format(self.path))
        self.logger.debug('has partitions %s', names)
        return names

    def partition(self):
        """
        Create a partition table on the block device.

        The newly created partition will have the following characteristics:
            - a primary partition using 100% of the device capacity
            - optimal alignment given the disk topology information
            - a MS-DOS disk label for simple BIOS booting on PC-type hardware
            - marked as bootable
        """
        self.logger.info('creating primary partition')
        device = parted.getDevice(self.path)
        self.logger.debug('created %s', device)
        disk = parted.freshDisk(device, 'msdos')
        self.logger.debug('created %s', disk)
        geometry = parted.Geometry(device=device, start=1,
                                   length=device.getLength() - 1)
        self.logger.debug('created %s', geometry)
        filesystem = parted.FileSystem(type='ext3', geometry=geometry)
        self.logger.debug('created %s', filesystem)
        partition = parted.Partition(disk=disk, type=parted.PARTITION_NORMAL,
                                     fs=filesystem, geometry=geometry)
        self.logger.debug('created %s', partition)
        disk.addPartition(partition=partition,
                          constraint=device.optimalAlignedConstraint)
        partition.setFlag(parted.PARTITION_BOOT)
        disk.commit()

    def wipe_dev(self, dev_path):
        """
        Wipe a device (partition or otherwise) of meta-data, be it file system,
        LVM, etc.

        @param dev_path:    Device path of the partition to be wiped.
        @type dev_path:     str
        """
        self.logger.debug('wiping %s', dev_path)
        with open(dev_path, 'wb') as p:
            p.write(bytearray(1024))

    def wipe(self):
        """
        Wipe the block device of meta-data, be it file system, LVM, etc.

        This is not intended to be secure, but rather to ensure that
        auto-discovery tools don't recognize anything here.
        """
        self.logger.info('wiping partitions and other meta-data')
        for partition in self.partition_names:
            self.wipe_dev(partition)
        self.wipe_dev(self.path)


if __name__ == '__main__':
    # It should be plenty obvious what needs to be done here.  If it's not
    # obvious to you, please don't run this!!!
    my_unimportant_test_disk = '/dev/sdX__FIXME'

    # Set up a logger for nice visibility.
    logger = getLogger(__name__)
    logger.setLevel(logging.DEBUG)
    logger.addHandler(_ConsoleHandler())

    # Ok, now for the good stuff, the actual example.
    guinea_pig = ExampleDevice(my_unimportant_test_disk)
    guinea_pig.wipe()
    guinea_pig.partition()
