/*
 * pyfilesys.h
 * pyparted type definitions for pyfilesys.c
 *
 * Copyright The pyparted Project Authors
 * SPDX-License-Identifier: GPL-2.0-or-later
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
