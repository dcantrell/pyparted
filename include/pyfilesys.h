/*
 * pyfilesys.h
 * pyparted type definitions for pyfilesys.c
 *
 * Copyright (C) 2007  Red Hat, Inc.
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
 * Red Hat Author(s): David Cantrell <dcantrell@redhat.com>
 *                    Chris Lumens <clumens@redhat.com>
 */

#ifndef PYFILESYS_H_INCLUDED
#define PYFILESYS_H_INCLUDED

#include <Python.h>

#include <parted/parted.h>

/* These functions need to be added to libparted.  Remove when that's done. */
#define ped_file_system_destroy(fs)

/* 1:1 function mappings for filesys.h in libparted */
PyObject *py_ped_file_system_type_register(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_type_unregister(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_type_get(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_type_get_next(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_probe_specific(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_probe(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_clobber(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_open(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_create(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_close(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_check(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_copy(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_resize(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_get_create_constraint(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_get_resize_constraint(PyObject *s, PyObject *args);
PyObject *py_ped_file_system_get_copy_constraint(PyObject *s, PyObject *args);

/* _ped.FileSystemType type is the Python equivalent of PedFileSystemType
 * in libparted */
typedef struct {
    PyObject_HEAD

    /* PedFileSystemType members */
    char *name;
} _ped_FileSystemType;

void _ped_FileSystemType_dealloc(_ped_FileSystemType *self);
int _ped_FileSystemType_traverse(_ped_FileSystemType *self, visitproc visit, void *arg);
int _ped_FileSystemType_clear(_ped_FileSystemType *self);
PyObject *_ped_FileSystemType_get(_ped_FileSystemType *self, void *closure);
int _ped_FileSystemType_set(_ped_FileSystemType *self, PyObject *value,
                            void *closure);

extern PyTypeObject _ped_FileSystemType_Type_obj;

/* _ped.FileSystem type is the Python equiv of PedFileSystem in libparted */
typedef struct {
    PyObject_HEAD

    /* PedFileSystem members */
    PyObject *type;              /* _ped.FileSystemType */
    PyObject *geom;              /* _ped.Geometry */
    int checked;
} _ped_FileSystem;

void _ped_FileSystem_dealloc(_ped_FileSystem *self);
int _ped_FileSystem_traverse(_ped_FileSystem *self, visitproc visit, void *arg);
int _ped_FileSystem_clear(_ped_FileSystem *self);
int _ped_FileSystem_init(_ped_FileSystem *self, PyObject *args, PyObject *kwds);
PyObject *_ped_FileSystem_get(_ped_FileSystem *self, void *closure);
int _ped_FileSystem_set(_ped_FileSystem *self, PyObject *value, void *closure);

extern PyTypeObject _ped_FileSystem_Type_obj;

#endif /* PYFILESYS_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4
 */
