/*
 * pydevice.h
 * pyparted docstrings for pydevice.c
 *
 * Copyright (C) 2007, 2008, 2009  Red Hat, Inc.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions of
 * the GNU General Public License v.2, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY expressed or implied, including the implied warranties of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.  You should have received a copy of the
 * GNU General Public License along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.  Any Red Hat trademarks that are incorporated in the
 * source code or documentation are not subject to the GNU General Public
 * License and may only be used or replicated with the express permission of
 * Red Hat, Inc.
 *
 * Red Hat Author(s): David Cantrell <dcantrell@redhat.com>
 */

#ifndef DOCSTRINGS_PYDEVICE_H_INCLUDED
#define DOCSTRINGS_PYDEVICE_H_INCLUDED

#include <Python.h>

PyDoc_STRVAR(disk_probe_doc,
"disk_probe(self) -> DiskType\n\n"
"Return the type of partition table detected, or raise _ped.IOException if\n"
"there is an error reading self.");

PyDoc_STRVAR(device_is_busy_doc,
"is_busy(self) -> bool\n\n"
"Return True if this Device is currently in use, False otherwise.");

PyDoc_STRVAR(device_open_doc,
"open(self) -> bool\n\n"
"Attempt to open this Device to allow use of read(), write(), and sync()\n"
"methods.  The open() call is architecture-dependent.  Apart from\n"
"requesting access to the device from the operating system, it does things\n"
"flushing caches.\n\n"
"This method may allocate internal resources depending on the architecture\n"
"All allocated resources are freed when you call the close() method.\n\n"
"Return True if the Device could be opened, False otherwise.");

PyDoc_STRVAR(device_close_doc,
"close(self) -> bool\n\n"
"Close this Device.  All allocated resources are freed.  If a failure\n"
"occurs while closing the Device, this method returns False.  The method\n"
"returns True on success.");

PyDoc_STRVAR(device_destroy_doc,
"destroy(self) -> None\n\n"
"Destroys the Device, removes it from the device list, destroys all\n"
"allocated resources associated with it, and destroys the object.");

PyDoc_STRVAR(device_cache_remove_doc,
"cache_remove(self) -> None\n\n"
"Remove the Device from the device list, but does not destroy it or any\n"
"allocated resources associated with it.  USE WITH CAUTION.");

PyDoc_STRVAR(device_begin_external_access_doc,
"begin_external_accessself() -> bool\n\n"
"Begins external access mode for this Device.  External access mode allows\n"
"you to safely do I/O on the device.  If a Device is open, then you should\n"
"not do any I/O on that Device, e.g. by calling an external program like\n"
"e2fsck, unless you put it in external access mode.  You should not use\n"
"any commands that do I/O to a Device while it is in external access mode.\n\n"
"Also, you should not close a Device while it is in external access mode.\n\n"
"Return True if the Device was successfully put in external access mode,\n"
"False otherwise.");

PyDoc_STRVAR(device_end_external_access_doc,
"end_external_access(self) -> bool\n\n"
"Ends external access mode for this Device.  Returns True on success,\n"
"False on failure.");

PyDoc_STRVAR(device_read_doc,
"read(self, start, count) -> bool\n\n"
"Read and return count sectors from this Device, starting at sector start.\n"
"Both start and count are long integers and buffer is a Python object large\n"
"enough to hold what you want to read.");

PyDoc_STRVAR(device_write_doc,
"write(self, buffer, start, count) -> bool\n\n"
"Write count sectors from buffer to this Device, starting at sector start.\n"
"Both start and count are long integers and buffer is a Python object holding\n"
"what you want to write to this Device.\n\n"
"Return True if the write was successful, False otherwise.");

PyDoc_STRVAR(device_sync_doc,
"sync(self) -> bool\n\n"
"Flushes all write-behind caches that might be holding up writes.  It is\n"
"slow because it guarantees cache coherency among all relevant caches.\n"
"Return True on success, False otherwise.");

PyDoc_STRVAR(device_sync_fast_doc,
"sync_fast(self) -> bool\n\n"
"Flushes all write-behind caches that might be holding writes.  WARNING:\n"
"Does NOT ensure cache coherency with other caches.  If you need cache\n"
"coherency, use sync() instead.  Return True on success, False otherwise.");

PyDoc_STRVAR(device_check_doc,
"check(self) -> long int\n\n"
"Architecture-dependent function that returns the number of sectors on\n"
"this Device that are ok.");

PyDoc_STRVAR(disk_clobber_doc,
"clobber(self) -> boolean\n\n"
"Remove all identifying information from a partition table.  If the partition\n"
"table cannot be cleared, a _ped.DiskException is raised.");

PyDoc_STRVAR(device_get_constraint_doc,
"get_constraint(self) -> Constraint\n\n"
"Get a constraint that represents hardware requirements on geometry.\n"
"This method will return a constraint representing the limits imposed by\n"
"the size of the disk, it will *not* provide any alignment constraints.\n"
"\n"
"Alignment constraints may be desirable when using media that have a\n"
"physical sector size that is a multiple of the logical sector size, as\n"
"in this case proper partition alignment can benefit disk performance\n"
"signigicantly.\n");

PyDoc_STRVAR(device_get_minimal_aligned_constraint_doc,
"get_minimal_aligned_constraint(self) -> Constraint\n\n"
"Get a constraint that represents hardware requirements on geometry and\n"
"alignment. This method returns a constraint representing the limits\n"
"imposed by the size of the disk and the minimal alignment requirements\n"
"for proper performance of the disk.\n");

PyDoc_STRVAR(device_get_optimal_aligned_constraint_doc,
"get_optimal_aligned_constraint(self) -> Constraint\n\n"
"Get a constraint that represents hardware requirements on geometry and\n"
"alignment. This method returns a constraint representing the limits\n"
"imposed by the size of the disk and the alignment requirements for\n"
"optimal performance of the disk.\n");

PyDoc_STRVAR(device_get_minimum_alignment_doc,
"get_minimum_alignment(self) -> Alignment\n\n"
"Get an alignment that represents minimum hardware requirements on\n"
"alignment. When for example using media that has a physical sector size\n"
"that is a multiple of the logical sector size, it is desirable to have\n"
"disk accesses (and thus partitions) properly aligned. Having partitions\n"
"not aligned to the minimum hardware requirements may lead to a\n"
"performance penalty.\n\n"
"The returned alignment describes the alignment for the start sector of\n"
"the partition, the end sector should be aligned too, to get the end\n"
"sector alignment decrease the returned alignment's offset by 1.\n");

PyDoc_STRVAR(device_get_optimum_alignment_doc,
"get_optimum_alignment(self) -> Alignment\n\n"
"Get an alignment that represents the hardware requirements for optimal\n"
"performance.\n\n"
"The returned alignment describes the alignment for the start sector of\n"
"the partition, the end sector should be aligned too, to get the end\n"
"sector alignment decrease the returned alignment's offset by 1.\n");

PyDoc_STRVAR(unit_get_size_doc,
"unit_get_size(self, Unit) -> long\n\n"
"Returns the byte size of self in the specified Unit.  The Unit\n"
"is any of the _ped.UNIT_* constants.");

PyDoc_STRVAR(unit_format_custom_byte_doc,
"unit_format_custom_byte(Sector, Unit) -> string\n\n"
"Return a string that describes the location of the byte Sector on\n"
"self, as described by Unit.  The Unit is any of the _ped.UNIT_*\n"
"constants.");

PyDoc_STRVAR(unit_format_byte_doc,
"unit_format_byte(Sector) -> string\n\n"
"Return a string that describes the location of the byte Sector on\n"
"self, as described by the default Unit.");

PyDoc_STRVAR(unit_format_custom_doc,
"unit_format_custom(Sector, Unit) -> string\n\n"
"Return a string that describes the location of Sector on self, as\n"
"described by Unit.  The Unit is any of the _ped.UNIT_* constants.");

PyDoc_STRVAR(unit_format_doc,
"unit_format(Device, Sector) -> string\n\n"
"Return a string that describes the location of Sector on self, as\n"
"described by the default Unit.");

PyDoc_STRVAR(unit_parse_doc,
"unit_parse(string, Sector, Geometry) -> boolean\n\n"
"Given a string providing a valid description of a location on self,\n"
"create a Geometry and Sector describing it.  Geometry will be two units\n"
"large, centered on Sector.  If this makes the Geometry exist partially\n"
"outside self, the Geometry will be intersected with the whole device\n"
"geometry.  This uses the default unit.");

PyDoc_STRVAR(unit_parse_custom_doc,
"unit_parse(string, Unit, Sector, Geometry) -> boolean\n\n"
"Follows the same description as unit_parse_doc, but takes a Unit as\n"
"well.  The Unit is any of the _ped.UNIT_* constants.");

PyDoc_STRVAR(_ped_CHSGeometry_doc,
"A _ped.CHSGeometry object describes a disk using the older CHS style\n"
"of defining disk geometry.  CHS stands for cylinders-heads-sectors.\n\n"
"The _ped.CHSGeometry objects are created automatically when devices are\n"
"probed by libparted.  They are used for reference purposes to get the\n"
"number of cylinders, heads, or sectors on a disk.  They cannot be used\n"
"to change the CHS values on a device.");

PyDoc_STRVAR(_ped_Device_doc,
"A _ped.Device object describes a block device accessible via the\n"
"operating system.  On Linux, an example block device is /dev/sda.\n\n"
"It is important to note that _ped.Device objects describe entire\n"
"block devices and not just partitions.");

#endif /* DOCSTRINGS_PYDEVICE_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4
 */
