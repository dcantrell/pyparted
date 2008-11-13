/*
 * pygeom.h
 * pyparted type definitions for pygeom.c
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
 */

#ifndef PYGEOM_H_INCLUDED
#define PYGEOM_H_INCLUDED

#include <Python.h>
#include <structmember.h>

#include <parted/parted.h>

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

/* _ped.Geometry type is the Python equivalent of PedGeometry in libparted */
typedef struct {
    PyObject_HEAD

    /* PedGeometry members */
    PyObject *dev;                /* _ped.Device */
    long long start;              /* PedSector   */
    long long length;             /* PedSector   */
    long long end;                /* PedSector   */
} _ped_Geometry;

static PyMemberDef _ped_Geometry_members[] = {
    {"dev", T_OBJECT, offsetof(_ped_Geometry, dev), 0, NULL},
    {NULL}
};

static PyMethodDef _ped_Geometry_methods[] = {
    {"init", (PyCFunction) py_ped_geometry_init, METH_VARARGS, NULL},
    {"new", (PyCFunction) py_ped_geometry_new, METH_VARARGS, NULL},
    {"duplicate", (PyCFunction) py_ped_geometry_duplicate, METH_VARARGS, NULL},
    {"intersect", (PyCFunction) py_ped_geometry_intersect, METH_VARARGS, NULL},
    {"destroy", (PyCFunction) py_ped_geometry_destroy, METH_VARARGS, NULL},
    {"set", (PyCFunction) py_ped_geometry_set, METH_VARARGS, NULL},
    {"set_start", (PyCFunction) py_ped_geometry_set_start, METH_VARARGS, NULL},
    {"set_end", (PyCFunction) py_ped_geometry_set_end, METH_VARARGS, NULL},
    {"test_overlap", (PyCFunction) py_ped_geometry_test_overlap,
                     METH_VARARGS, NULL},
    {"test_inside", (PyCFunction) py_ped_geometry_test_inside,
                    METH_VARARGS, NULL},
    {"test_equal", (PyCFunction) py_ped_geometry_test_equal,
                   METH_VARARGS, NULL},
    {"test_sector_inside", (PyCFunction) py_ped_geometry_test_sector_inside,
                           METH_VARARGS, NULL},
    {"read", (PyCFunction) py_ped_geometry_read, METH_VARARGS, NULL},
    {"sync", (PyCFunction) py_ped_geometry_sync, METH_VARARGS, NULL},
    {"sync_fast", (PyCFunction) py_ped_geometry_sync_fast, METH_VARARGS, NULL},
    {"write", (PyCFunction) py_ped_geometry_write, METH_VARARGS, NULL},
    {"check", (PyCFunction) py_ped_geometry_check, METH_VARARGS, NULL},
    {"map", (PyCFunction) py_ped_geometry_map, METH_VARARGS, NULL},
    {NULL}
};

void _ped_Geometry_dealloc(_ped_Geometry *self);
PyObject *_ped_Geometry_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
int _ped_Geometry_init(_ped_Geometry *self, PyObject *args, PyObject *kwds);
PyObject *_ped_Geometry_get(_ped_Geometry *self, void *closure);
int _ped_Geometry_set(_ped_Geometry *self, PyObject *value, void *closure);

static PyGetSetDef _ped_Geometry_getset[] = {
    {"start", (getter) _ped_Geometry_get,
              (setter) _ped_Geometry_set,
              "Geometry start", "start"},
    {"length", (getter) _ped_Geometry_get,
               (setter) _ped_Geometry_set,
               "Geometry length", "length"},
    {"end", (getter) _ped_Geometry_get,
            (setter) _ped_Geometry_set,
            "Geometry end", "end"},
    {NULL}  /* Sentinel */
};

static PyTypeObject _ped_Geometry_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    .tp_name = "_ped.Geometry",
    .tp_basicsize = sizeof(_ped_Geometry),
 /* .tp_itemsize = XXX */
    .tp_dealloc = (destructor) _ped_Geometry_dealloc,
 /* .tp_print = XXX */
 /* .tp_getattr = XXX */
 /* .tp_setattr = XXX */
 /* .tp_compare = XXX */
 /* .tp_repr = XXX */
 /* .tp_as_number = XXX */
 /* .tp_as_sequence = XXX */
 /* .tp_as_mapping = XXX */
 /* .tp_hash = XXX */
 /* .tp_call = XXX */
 /* .tp_str = XXX */
 /* .tp_getattro = XXX */
 /* .tp_setattro = XXX */
 /* .tp_as_buffer = XXX */
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_doc = "PedGeometry objects",
 /* .tp_traverse = XXX */
 /* .tp_clear = XXX */
 /* .tp_richcompare = XXX */
 /* .tp_weaklistoffset = XXX */
 /* .tp_iter = XXX */
 /* .tp_iternext = XXX */
    .tp_methods = _ped_Geometry_methods,
    .tp_members = _ped_Geometry_members,
    .tp_getset = _ped_Geometry_getset,
 /* .tp_base = XXX */
 /* .tp_dict = XXX */
 /* .tp_descr_get = XXX */
 /* .tp_descr_set = XXX */
 /* .tp_dictoffset = XXX */
    .tp_init = (initproc) _ped_Geometry_init,
 /* .tp_alloc = XXX */
    .tp_new = _ped_Geometry_new,
 /* .tp_free = XXX */
 /* .tp_is_gc = XXX */
 /* .tp_bases = XXX */
 /* .tp_mro = XXX */
 /* .tp_cache = XXX */
 /* .tp_subclasses = XXX */
 /* .tp_weaklist = XXX */
 /* .tp_del = XXX */
};

#endif /* PYGEOM_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4
 */
