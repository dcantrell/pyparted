/*
 * pyfilesys.h
 * pyparted type definitions for pyfilesys.c
 *
 * Copyright (C) 2007-2013 Red Hat, Inc.
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
 * Author(s): David Cantrell <dcantrell@redhat.com>
 *            Chris Lumens <clumens@redhat.com>
 *            Alex Skinner <alex@lx.lc>
 */

#ifndef PYFILESYS_H_INCLUDED
#define PYFILESYS_H_INCLUDED

#include <Python.h>

#include <parted/parted.h>

/* These functions need to be added to libparted.  Remove when that's done. */
#define ped_file_system_destroy(fs)

/* 1:1 function mappings for filesys.h in libparted */
PyObject *py_ped_file_system_type_get(PyObject *, PyObject *);
PyObject *py_ped_file_system_type_get_next(PyObject *, PyObject *);
PyObject *py_ped_file_system_probe_specific(PyObject *, PyObject *);
PyObject *py_ped_file_system_probe(PyObject *, PyObject *);

/* _ped.FileSystemType type is the Python equivalent of PedFileSystemType
 * in libparted */
typedef struct {
    PyObject_HEAD

    /* PedFileSystemType members */
    char *name;
} _ped_FileSystemType;

void _ped_FileSystemType_dealloc(_ped_FileSystemType *);
int _ped_FileSystemType_compare(_ped_FileSystemType *, PyObject *);
PyObject *_ped_FileSystemType_richcompare(_ped_FileSystemType *, PyObject *,
                                          int);
PyObject *_ped_FileSystemType_str(_ped_FileSystemType *);
int _ped_FileSystemType_traverse(_ped_FileSystemType *, visitproc, void *);
int _ped_FileSystemType_clear(_ped_FileSystemType *);
PyObject *_ped_FileSystemType_get(_ped_FileSystemType *, void *);

extern PyTypeObject _ped_FileSystemType_Type_obj;

/* _ped.FileSystem type is the Python equiv of PedFileSystem in libparted */
typedef struct {
    PyObject_HEAD

    /* PedFileSystem members */
    PyObject *type;                /* _ped.FileSystemType */
    PyObject *geom;                /* _ped.Geometry */
    int checked;

    /* store the PedFileSystem from libparted */
    PedFileSystem *ped_filesystem;
} _ped_FileSystem;

void _ped_FileSystem_dealloc(_ped_FileSystem *);
int _ped_FileSystem_compare(_ped_FileSystem *, PyObject *);
PyObject *_ped_FileSystem_richcompare(_ped_FileSystem *, PyObject *, int);
PyObject *_ped_FileSystem_str(_ped_FileSystem *);
int _ped_FileSystem_traverse(_ped_FileSystem *, visitproc, void *);
int _ped_FileSystem_clear(_ped_FileSystem *);
int _ped_FileSystem_init(_ped_FileSystem *, PyObject *, PyObject *);
PyObject *_ped_FileSystem_get(_ped_FileSystem *, void *);

extern PyTypeObject _ped_FileSystem_Type_obj;

#endif /* PYFILESYS_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4
 */
