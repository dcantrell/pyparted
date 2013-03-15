/*
 * pygeom.h
 * pyparted docstrings for pygeom.c
 *
 * Copyright (C) 2007, 2008 Red Hat, Inc.
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
 *                    Chris Lumens <clumens@redhat.com>
 */

#ifndef DOCSTRINGS_PYGEOM_H_INCLUDED
#define DOCSTRINGS_PYGEOM_H_INCLUDED

#include <Python.h>

PyDoc_STRVAR(geometry_duplicate_doc,
"duplicate(self) -> _ped.Geometry\n\n"
"Create an identical copy of self.  Raises _ped.CreateException if the\n"
"operation fails");

PyDoc_STRVAR(geometry_intersect_doc,
"intersect(self, Geometry) -> _ped.Geometry\n\n"
"Create a new Geometry describing the region common to both self and\n"
"Geometry.  Raises ArithmeticError if the two regions do not intersect.");

PyDoc_STRVAR(geometry_set_doc,
"set(self, start, length) -> boolean\n\n"
"Sets a new start Sector and length Sector in the Geometry object,\n"
"also implicitly setting the end Sector as well.");

PyDoc_STRVAR(geometry_set_start_doc,
"set_start(self, start) -> boolean\n\n"
"Sets a new start Sector without modifying the end Sector.  Length\n"
"will be modified to match the new starting position.");

PyDoc_STRVAR(geometry_set_end_doc,
"set_end(self, end) -> boolean\n\n"
"Sets a new ending Sector without modifying the start Sector.  Length\n"
"will be modified to match the new ending position.");

PyDoc_STRVAR(geometry_test_overlap_doc,
"test_overlap(self, Geometry) -> boolean\n\n"
"Return whether self and Geometry are on the same physical device and\n"
"share at least part of the same region.");

PyDoc_STRVAR(geometry_test_inside_doc,
"test_inside(self, Geometry) -> boolean\n\n"
"Return whether Geometry is entirely within self and on the same physical\n"
"device.");

PyDoc_STRVAR(geometry_test_equal_doc,
"test_equal(self, Geometry) -> boolean\n\n"
"Return whether self and Geometry are on the same device and have the same\n"
"region.");

PyDoc_STRVAR(geometry_test_sector_inside_doc,
"test_sector_inside(self, Sector) -> boolean\n\n"
"Return whether Sector is entirely within the region described by self.");

PyDoc_STRVAR(geometry_read_doc,
"read(self, buffer, offset, count) -> boolean\n\n"
"Read data from the region described by self.  This method reads count\n"
"Sectors starting at Sector offset (from the start of the region, not\n"
"from the start of the disk) into buffer.  This method raises\n"
"_ped.IOException on error.");

PyDoc_STRVAR(geometry_sync_doc,
"sync(self) -> boolean\n\n"
"Flushes all caches on the device described by self.  This operation can be\n"
"slow because it must guarantee cache coherency among multiple caches.  This\n"
"method raises _ped.IOException on error.");

PyDoc_STRVAR(geometry_sync_fast_doc,
"sync_fast(self) -> boolean\n\n"
"Flushes all caches on the device described by self without guaranteeing\n"
"cache coherency.  This makes it fast but more prone to error.  This method\n"
"raises _ped.IOException on error.");

PyDoc_STRVAR(geometry_write_doc,
"write(self, buffer, offset, count) -> boolean\n\n"
"Write data into the region described by self.  This method writes count\n"
"Sectors of buffer into the region starting at Sector offset.  The offset is\n"
"from the beginning of the region, not of the disk.  This method raises\n"
"_ped.IOException on error.");

PyDoc_STRVAR(geometry_check_doc,
"check(self, offset, granularity, count, timer=None) -> Sector\n\n"
"This method checks the region described by self for errors on the disk.\n"
"The region to check starts at offset Sectors from the beginning of the\n"
"region and is count Sectors long.  granularity specifies how Sectors should\n"
"be grouped together.\n\n"
"This method returns the first bad sector, or 0 if there are no errors.");

PyDoc_STRVAR(geometry_map_doc,
"map(self, Geometry, Sector) -> integer\n\n"
"Given a Geometry that overlaps with self and a Sector inside Geometry,\n"
"this method translates the address of Sector into an address inside self.\n"
"The new address is returned, or ArithmeticError is raised if Sector does\n"
"not exist within self.");

PyDoc_STRVAR(_ped_Geometry_doc,
"A _ped.Geometry object describes a continuous region on a physical device.\n"
"This device is given by the dev attribute when the Geometry is created.\n"
"Most methods on this object involve creating new Geometry objects as needed\n"
"and can therefore raise _ped.CreateException when an error occurs creating\n"
"the new object.  Most methods can also raise _ped.IOException when reading\n"
"or writing the underlying physical device fails.\n\n"
"libparted (and therefore pyparted) attempts to enforce the following\n"
"conditions on Geometry objects:\n\n"
"\t- start + length - 1 == end\n"
"\t- length > 0\n"
"\t- start >= 0\n"
"\t- end < dev.length");

#endif /* DOCSTRINGS_PYGEOM_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4
 */
