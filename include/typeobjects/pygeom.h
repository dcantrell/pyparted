/*
 * pygeom.h
 * pyparted type objects for pygeom.c
 *
 * Copyright The pyparted Project Authors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef TYPEOBJECTS_PYGEOM_H_INCLUDED
#define TYPEOBJECTS_PYGEOM_H_INCLUDED

#include <Python.h>
#include <structmember.h>

/* _ped.Geometry type object */
static PyMemberDef _ped_Geometry_members[] = {
    {"dev", T_OBJECT, offsetof(_ped_Geometry, dev), READONLY,
            "The _ped.Device described by this _ped.Geometry object."},
    {NULL}
};

static PyMethodDef _ped_Geometry_methods[] = {
    {"duplicate", (PyCFunction) py_ped_geometry_duplicate, METH_VARARGS,
                  geometry_duplicate_doc},
    {"intersect", (PyCFunction) py_ped_geometry_intersect, METH_VARARGS,
                  geometry_intersect_doc},
    {"set", (PyCFunction) py_ped_geometry_set, METH_VARARGS,
            geometry_set_doc},
    {"set_start", (PyCFunction) py_ped_geometry_set_start, METH_VARARGS,
                  geometry_set_start_doc},
    {"set_end", (PyCFunction) py_ped_geometry_set_end, METH_VARARGS,
                geometry_set_end_doc},
    {"test_overlap", (PyCFunction) py_ped_geometry_test_overlap,
                     METH_VARARGS, geometry_test_overlap_doc},
    {"test_inside", (PyCFunction) py_ped_geometry_test_inside,
                    METH_VARARGS, geometry_test_inside_doc},
    {"test_equal", (PyCFunction) py_ped_geometry_test_equal,
                   METH_VARARGS, geometry_test_equal_doc},
    {"test_sector_inside", (PyCFunction) py_ped_geometry_test_sector_inside,
                           METH_VARARGS, geometry_test_sector_inside_doc},
    {"read", (PyCFunction) py_ped_geometry_read, METH_VARARGS,
             geometry_read_doc},
    {"sync", (PyCFunction) py_ped_geometry_sync, METH_VARARGS,
             geometry_sync_doc},
    {"sync_fast", (PyCFunction) py_ped_geometry_sync_fast, METH_VARARGS,
                  geometry_sync_fast_doc},
    {"write", (PyCFunction) py_ped_geometry_write, METH_VARARGS,
              geometry_write_doc},
    {"check", (PyCFunction) py_ped_geometry_check, METH_VARARGS,
              geometry_check_doc},
    {"map", (PyCFunction) py_ped_geometry_map, METH_VARARGS,
            geometry_map_doc},
    {NULL}
};

static PyGetSetDef _ped_Geometry_getset[] = {
    {"start", (getter) _ped_Geometry_get,
              (setter) _ped_Geometry_set,
              "The starting Sector of the region.", "start"},
    {"length", (getter) _ped_Geometry_get,
               (setter) _ped_Geometry_set,
               "The length of the region described by this Geometry object.",
               "length"},
    {"end", (getter) _ped_Geometry_get,
            (setter) _ped_Geometry_set,
            "The ending Sector of the region.", "end"},
    {NULL}  /* Sentinel */
};

PyTypeObject _ped_Geometry_Type_obj = {
    PyVarObject_HEAD_INIT(&PyType_Type,0)
    .tp_name = "_ped.Geometry",
    .tp_basicsize = sizeof(_ped_Geometry),
 /* .tp_itemsize = XXX */
    .tp_dealloc = (destructor) _ped_Geometry_dealloc,
 /* .tp_getattr = XXX */
 /* .tp_setattr = XXX */
 /* .tp_repr = XXX */
 /* .tp_as_number = XXX */
 /* .tp_as_sequence = XXX */
 /* .tp_as_mapping = XXX */
    .tp_hash = PyObject_HashNotImplemented,
    .tp_call = NULL,
    .tp_str = (reprfunc) _ped_Geometry_str,
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_setattro = PyObject_GenericSetAttr,
 /* .tp_as_buffer = XXX */
    .tp_flags = TP_FLAGS,
    .tp_doc = _ped_Geometry_doc,
    .tp_traverse = (traverseproc) _ped_Geometry_traverse,
    .tp_clear = (inquiry) _ped_Geometry_clear,
    .tp_richcompare = (richcmpfunc) _ped_Geometry_richcompare,
 /* .tp_weaklistoffset = XXX */
 /* .tp_iter = XXX */
 /* .tp_iternext = XXX */
    .tp_methods = _ped_Geometry_methods,
    .tp_members = _ped_Geometry_members,
    .tp_getset = _ped_Geometry_getset,
    .tp_base = NULL,
    .tp_dict = NULL,
 /* .tp_descr_get = XXX */
 /* .tp_descr_set = XXX */
 /* .tp_dictoffset = XXX */
    .tp_init = (initproc) _ped_Geometry_init,
    .tp_alloc = PyType_GenericAlloc,
    .tp_new = PyType_GenericNew,
 /* .tp_free = XXX */
 /* .tp_is_gc = XXX */
    .tp_bases = NULL,
 /* .tp_del = XXX */
};

#endif /* TYPEOBJECTS_PYGEOM_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4
 */
