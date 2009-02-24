/*
 * pyfilesys.h
 * pyparted docstrings for pyfilesys.c
 *
 * Copyright (C) 2007  Red Hat, Inc.
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

PyDoc_STRVAR(file_system_clobber_doc,
"clobber(self) -> boolean\n\n"
"This method erases any file system signatures found in the region given by\n"
"self.geom, effectively removing the file system from the partition.  After\n"
"calling this method, _ped.file_system_probe() won't detect any filesystem.\n"
"This method is called by self.create() before creating a new filesystem.\n"
"Raises _ped.IOException on any internal parted errors or\n"
"_ped.FileSystemException if no filesystem exists in self.geom");

PyDoc_STRVAR(file_system_open_doc,
"open(self) -> _ped.FileSystem\n\n"
"Open and return the file system in the region given by self.geom, if one\n"
"exists.  If no file system is found, _ped.FileSystemException is raised.\n"
"For all other error conditions, _ped.IOException is raised.  This method is\n"
"not necessarily implemented for all filesystem types parted understands.");

PyDoc_STRVAR(file_system_create_doc,
"create(self, timer=None) -> _ped.FileSystem\n\n"
"Initialize a new filesystem of type self.type on the region given by\n"
"self.geom and return that new filesystem.  If the filesystem cannot be\n"
"created, a _ped.FileSystemException is rasied.  For all other error\n"
"conditions, _ped.IOException is raised.  This method is not necesssarily\n"
"implemented for all filesystem types parted understands.");

PyDoc_STRVAR(file_system_close_doc,
"close(self) -> boolean\n\n"
"Close the filesystem, raising _ped.FileSystemException on error.");

PyDoc_STRVAR(file_system_check_doc,
"check(self, timer=None) -> boolean\n\n"
"Check the filesystem for errors, returning False if any are found.  This\n"
"method is not necessarily implemented for all filesystem types parted\n"
"understands.");

PyDoc_STRVAR(file_system_copy_doc,
"copy(self, Geometry, timer=None) -> _ped.FileSystem\n\n"
"Create and return a new filesystem of the same type on the region given by\n"
"Geometry, and copy the contents of the existing filesystem into the new\n"
"one.  If an error occurrs creating or copying the new filesystem,\n"
"_ped.FileSystemException is raised.  This method is not necessarily\n"
"implemented for all filesystem types parted understands\n");

PyDoc_STRVAR(file_system_resize_doc,
"resize(self, Geometry, timer=None) -> boolean\n\n"
"Resize self to the new region described by Geometry.  It is highly\n"
"recommended that Geometry satisfy self.get_resize_constraint(), though\n"
"parted does not enforce this recommendation.  In this case, the resize\n"
"operation will most likely fail.  On error, _ped.FileSystemException is\n"
"raised.  This method is not necessarily implemented for all filesystem\n"
"types parted understands.");

PyDoc_STRVAR(file_system_get_resize_constraint_doc,
"get_resize_constraint(self) -> Constraint\n\n"
"Return a constraint that represents all possible ways self can be resized\n"
"with self.resize().  This takes into account the amount of space already\n"
"in use on the filesystem.");

PyDoc_STRVAR(_ped_FileSystemType_doc,
"A _ped.FileSystemType object gives a name to a single filesystem that parted\n"
"understands.  parted maintains a list of these objects which can be\n"
"traversed with the self.get_next method or accessed directly via self.get().");

PyDoc_STRVAR(_ped_FileSystem_doc,
"A _ped.FileSystem object describes a filesystem that exists in a given\n"
"region on a device.  The region is given by a _ped.Geometry object, and\n"
"the filesystem is further described by a _ped.FileSystemType object.\n\n"
"It is recommended that self.check() be called before any of the create,\n"
"resize, or copy operations are called.\n\n"
"Filesystem operations are especially prone to failures, and pyparted raises\n"
"a variety of exceptions when error conditions are encountered.  The most\n"
"common is _ped.FileSystemException, though _ped.IOException and\n"
"_ped.CreateException may also be raised.\n\n"
"parted knows about a variety of filesystems, but supports them to varying\n"
"degrees.  For some filesystems, it supports the full range of reading,\n"
"copying, resizing, and checking operations.  Other filesystems may only\n"
"support reading but no write operations, or all operations but resize.\n"
"If an operation is not supported, a NotImplementedError will be raised.");

#endif /* DOCSTRINGS_PYFILESYS_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4
 */
