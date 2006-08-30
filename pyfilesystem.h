/* -*- Mode: c; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*-
 * Matt Wilson <msw@redhat.com>
 *
 * Copyright 2000 Red Hat, Inc.
 *
 * This software may be freely redistributed under the terms of the GNU
 * library public license.
 *
 * You should have received a copy of the GNU Library Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
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

