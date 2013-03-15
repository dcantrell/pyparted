/*
 * pyfilesys.h
 * pyparted docstrings for pyfilesys.c
 *
 * Copyright (C) 2007-2011  Red Hat, Inc.
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

#ifndef DOCSTRINGS_PYFILESYS_H_INCLUDED
#define DOCSTRINGS_PYFILESYS_H_INCLUDED

#include <Python.h>

PyDoc_STRVAR(_ped_FileSystemType_doc,
"A _ped.FileSystemType object gives a name to a single filesystem that parted\n"
"understands.  parted maintains a list of these objects which can be\n"
"traversed with the self.get_next method or accessed directly via self.get().");

PyDoc_STRVAR(_ped_FileSystem_doc,
"A _ped.FileSystem object describes a filesystem that exists in a given\n"
"region on a device.  The region is given by a _ped.Geometry object, and\n"
"the filesystem is further described by a _ped.FileSystemType object.\n\n"
"Filesystem operations are especially prone to failures, and pyparted raises\n"
"a variety of exceptions when error conditions are encountered.  The most\n"
"common is _ped.FileSystemException, though _ped.IOException and\n"
"_ped.CreateException may also be raised.");

#endif /* DOCSTRINGS_PYFILESYS_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4
 */
