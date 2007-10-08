/*
 * pygeom.h
 * pyparted type definitions for pygeom.c
 *
 * Copyright (C) 2007  Red Hat, Inc.
 * All rights reserved.
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
 * Red Hat Author(s): David Cantrell
 */

#ifndef PYGEOM_H_INCLUDED
#define PYGEOM_H_INCLUDED

#include <Python.h>
#include <structmember.h>

#include <parted/parted.h>

/* _ped.Geometry type is the Python equivalent of PedGeometry in libparted */
typedef struct {
    PyObject_HEAD

    /* PedGeometry members */
    PyObject *dev;                /* _ped.Device */
    PyObject *start;              /* _ped.Sector */
    PyObject *length;             /* _ped.Sector */
    PyObject *end;                /* _ped.Sector */
} _ped_Geometry;

static PyMemberDef _ped_Geometry_members[] = {
    {"dev", T_OBJECT, offsetof(_ped_Geometry, dev), 0, NULL},
    {"start", T_OBJECT, offsetof(_ped_Geometry, start), 0, NULL},
    {"length", T_OBJECT, offsetof(_ped_Geometry, length), 0, NULL},
    {"end", T_OBJECT, offsetof(_ped_Geometry, end), 0, NULL},
    {NULL}
};

static PyMethodDef _ped_Geometry_methods[] = {
    {NULL}
};

void _ped_Geometry_dealloc(_ped_Geometry *self);
PyObject *_ped_Geometry_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
int _ped_Geometry_init(_ped_Geometry *self, PyObject *args, PyObject *kwds);

static PyGetSetDef _ped_Geometry_getset[] = {
    {NULL}  /* Sentinel */
};

static PyTypeObject _ped_Geometry_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    0,                                         /* ob_size */
    "_ped.Geometry",                           /* tp_name */
    sizeof(_ped_Geometry),                     /* tp_basicsize */
    0,                                         /* tp_itemsize */
    (destructor) _ped_Geometry_dealloc,        /* tp_dealloc */
    0,                                         /* tp_print */
    0,                                         /* tp_getattr */
    0,                                         /* tp_setattr */
    0,                                         /* tp_compare */
    0,                                         /* tp_repr */
    0,                                         /* tp_as_number */
    0,                                         /* tp_as_sequence */
    0,                                         /* tp_as_mapping */
    0,                                         /* tp_hash */
    0,                                         /* tp_call */
    0,                                         /* tp_str */
    0,                                         /* tp_getattro */
    0,                                         /* tp_setattro */
    0,                                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,  /* tp_flags */
    "PedGeometry objects",                     /* tp_doc */
    0,                                         /* tp_traverse */
    0,                                         /* tp_clear */
    0,                                         /* tp_richcompare */
    0,                                         /* tp_weaklistoffset */
    0,                                         /* tp_iter */
    0,                                         /* tp_iternext */
    _ped_Geometry_methods,                     /* tp_methods */
    _ped_Geometry_members,                     /* tp_members */
    _ped_Geometry_getset,                      /* tp_getset */
    0,                                         /* tp_base */
    0,                                         /* tp_dict */
    0,                                         /* tp_descr_get */
    0,                                         /* tp_descr_set */
    0,                                         /* tp_dictoffset */
    (initproc) _ped_Geometry_init,             /* tp_init */
    0,                                         /* tp_alloc */
    _ped_Geometry_new,                         /* tp_new */
};

/* 1:1 function mappings for geom.h in libparted */
PyObject *py_ped_geometry_init(PyObject *s, PyObject *args);
PyObject *py_ped_geometry_new(PyObject *s, PyObject *args);
PyObject *py_ped_geometry_duplicate(PyObject *s, PyObject *args);
PyObject *py_ped_geometry_intersect(PyObject *s, PyObject *args);
PyObject *py_ped_geometry_destroy(PyObject *s, PyObject *args);
PyObject *py_ped_geometry_set(PyObject *s, PyObject *args);
PyObject *py_ped_geometry_set_start(PyObject *s, PyObject *args);
PyObject *py_ped_geometry_set_end(PyObject *s, PyObject *args);
PyObject *py_ped_geometry_test_overlap(PyObject *s, PyObject *args);
PyObject *py_ped_geometry_test_inside(PyObject *s, PyObject *args);
PyObject *py_ped_geometry_test_equal(PyObject *s, PyObject *args);
PyObject *py_ped_geometry_test_sector_inside(PyObject *s, PyObject *args);
PyObject *py_ped_geometry_read(PyObject *s, PyObject *args);
PyObject *py_ped_geometry_sync(PyObject *s, PyObject *args);
PyObject *py_ped_geometry_sync_fast(PyObject *s, PyObject *args);
PyObject *py_ped_geometry_write(PyObject *s, PyObject *args);
PyObject *py_ped_geometry_check(PyObject *s, PyObject *args);
PyObject *py_ped_geometry_map(PyObject *s, PyObject *args);

#endif /* PYGEOM_H_INCLUDED */
