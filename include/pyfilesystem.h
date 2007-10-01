/*
 * pyfilesystem.h
 *
 * Copyright (C) 2000  Red Hat, Inc.  All rights reserved.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions of
 * the GNU General Public License v.2, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY expressed or implied, including the implied warranties of
 * MERCHANTABILITY or FITNESS FOR A * PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.  You should have received a copy of the
 * GNU General Public License along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.  Any Red Hat trademarks that are incorporated in the
 * source code or documentation are not subject to the GNU General Public
 * License and may only be used or replicated with the express permission of
 * Red Hat, Inc.
 *
 * Red Hat Author(s): Matt Wilson
 *                    David Cantrell
 */

#ifndef PYFILESYSTEM_H_INCLUDED
#define PYFILESYSTEM_H_INCLUDED

#include <Python.h>
#include <parted/constraint.h>
#include <parted/disk.h>
#include "pygeometry.h"

typedef struct _PyPedFileSystem PyPedFileSystem;

struct _PyPedFileSystem {
        PyObject_HEAD;
        PedFileSystem* fs;
        PyPedGeometry *geom;
};

extern PyTypeObject PyPedFileSystemType;

PyPedFileSystem *
py_ped_file_system_obj_new (PedFileSystem* fs,
                            PyPedGeometry *geom);

typedef struct _PyPedFileSystemTypeObj PyPedFileSystemTypeObj;

struct _PyPedFileSystemTypeObj {
        PyObject_HEAD;
        const PedFileSystemType* fs_type;
};

extern PyTypeObject PyPedFileSystemTypeType;

PyPedFileSystemTypeObj*
py_ped_file_system_type_obj_new (const PedFileSystemType* fs_type);

#endif /* PYFILESYSTEM_H_INCLUDED */

