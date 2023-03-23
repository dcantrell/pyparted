/*
 * pyfilesys.h
 * pyparted docstrings for pyfilesys.c
 *
 * Copyright The pyparted Project Authors
 * SPDX-License-Identifier: GPL-2.0-or-later
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
