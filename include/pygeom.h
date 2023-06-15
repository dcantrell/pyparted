/*
 * pygeom.h
 * pyparted type definitions for pygeom.c
 *
 * Copyright The pyparted Project Authors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef PYGEOM_H_INCLUDED
#define PYGEOM_H_INCLUDED

#include <Python.h>

#include <parted/parted.h>

/* 1:1 function mappings for geom.h in libparted */
PyObject *py_ped_geometry_duplicate(PyObject *, PyObject *);
PyObject *py_ped_geometry_intersect(PyObject *, PyObject *);
PyObject *py_ped_geometry_set(PyObject *, PyObject *);
PyObject *py_ped_geometry_set_start(PyObject *, PyObject *);
PyObject *py_ped_geometry_set_end(PyObject *, PyObject *);
PyObject *py_ped_geometry_test_overlap(PyObject *, PyObject *);
PyObject *py_ped_geometry_test_inside(PyObject *, PyObject *);
PyObject *py_ped_geometry_test_equal(PyObject *, PyObject *);
PyObject *py_ped_geometry_test_sector_inside(PyObject *, PyObject *);
PyObject *py_ped_geometry_read(PyObject *, PyObject *);
PyObject *py_ped_geometry_sync(PyObject *, PyObject *);
PyObject *py_ped_geometry_sync_fast(PyObject *, PyObject *);
PyObject *py_ped_geometry_write(PyObject *, PyObject *);
PyObject *py_ped_geometry_check(PyObject *, PyObject *);
PyObject *py_ped_geometry_map(PyObject *, PyObject *);

/* _ped.Geometry type is the Python equivalent of PedGeometry in libparted */
typedef struct {
    PyObject_HEAD

    /* PedGeometry members */
    PyObject *dev;                /* _ped.Device */

    /* store the PedGeometry from libparted */
    PedGeometry *ped_geometry;
} _ped_Geometry;

void _ped_Geometry_dealloc(_ped_Geometry *);
int _ped_Geometry_compare(_ped_Geometry *, PyObject *);
PyObject *_ped_Geometry_richcompare(_ped_Geometry *, PyObject *, int);
PyObject *_ped_Geometry_str(_ped_Geometry *);
int _ped_Geometry_traverse(_ped_Geometry *, visitproc, void *);
int _ped_Geometry_clear(_ped_Geometry *);
int _ped_Geometry_init(_ped_Geometry *, PyObject *, PyObject *);
PyObject *_ped_Geometry_get(_ped_Geometry *, void *);
int _ped_Geometry_set(_ped_Geometry *, PyObject *, void *);

extern PyTypeObject _ped_Geometry_Type_obj;

#endif /* PYGEOM_H_INCLUDED */
