/*
 * pygeom.h
 * pyparted type definitions for pygeom.c
 *
 * Copyright (C) 2007, 2008 Red Hat, Inc.
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

#ifndef PYGEOM_H_INCLUDED
#define PYGEOM_H_INCLUDED

#include <Python.h>
#include <structmember.h>

#include <parted/parted.h>

/* 1:1 function mappings for geom.h in libparted */
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

PyDoc_STRVAR(geometry_duplicate_doc,
"duplicate(self) -> _ped.Geometry\n\n"
"Create an identical copy of self.  Raises _ped.CreateException if the\n"
"operation fails");

PyDoc_STRVAR(geometry_intersect_doc,
"intersect(self, Geometry) -> _ped.Geometry\n\n"
"Create a new Geometry describing the region common to both self and\n"
"Geometry.  Raises ArithmeticError if the two regions do not intersect.");

PyDoc_STRVAR(geometry_destroy_doc,
"destroy(self) -> None\n\n"
"Destroys the Geometry object.");

PyDoc_STRVAR(geometry_set_doc,
"set(self, start, length) -> boolean\n\n"
"Sets a new start Sector and length Sector in the Geometry object,\n"
"also implicitly setting the end Sector as well.");

PyDoc_STRVAR(geometry_set_start_doc,
"set_start(self, start) -> boolean\n\n"
"Sets a new start Sector without modifying the end Sector.  Length\n"
"will be modified to match the new starting position.");

PyDoc_STRVAR(geometry_set_end_doc,
"set_end(self, end) -> boolean\n\n"
"Sets a new ending Sector without modifying the start Sector.  Length\n"
"will be modified to match the new ending position.");

PyDoc_STRVAR(geometry_test_overlap_doc,
"test_overlap(self, Geometry) -> boolean\n\n"
"Return whether self and Geometry are on the same physical device and\n"
"share at least part of the same region.");

PyDoc_STRVAR(geometry_test_inside_doc,
"test_inside(self, Geometry) -> boolean\n\n"
"Return whether Geometry is entirely within self and on the same physical\n"
"device.");

PyDoc_STRVAR(geometry_test_equal_doc,
"test_equal(self, Geometry) -> boolean\n\n"
"Return whether self and Geometry are on the same device and have the same\n"
"region.");

PyDoc_STRVAR(geometry_test_sector_inside_doc,
"test_sector_inside(self, Sector) -> boolean\n\n"
"Return whether Sector is entirely within the region described by self.");

PyDoc_STRVAR(geometry_read_doc,
"read(self, buffer, offset, count) -> boolean\n\n"
"Read data from the region described by self.  This method reads count\n"
"Sectors starting at Sector offset (from the start of the region, not\n"
"from the start of the disk) into buffer.  This method raises\n"
"_ped.IOException on error.");

PyDoc_STRVAR(geometry_sync_doc,
"sync(self) -> boolean\n\n"
"Flushes all caches on the device described by self.  This operation can be\n"
"slow because it must guarantee cache coherency among multiple caches.  This\n"
"method raises _ped.IOException on error.");

PyDoc_STRVAR(geometry_sync_fast_doc,
"sync_fast(self) -> boolean\n\n"
"Flushes all caches on the device described by self without guaranteeing\n"
"cache coherency.  This makes it fast but more prone to error.  This method\n"
"raises _ped.IOException on error.");

PyDoc_STRVAR(geometry_write_doc,
"write(self, buffer, offset, count) -> boolean\n\n"
"Write data into the region described by self.  This method writes count\n"
"Sectors of buffer into the region starting at Sector offset.  The offset is\n"
"from the beginning of the region, not of the disk.  This method raises\n"
"_ped.IOException on error.");

PyDoc_STRVAR(geometry_check_doc,
"check(self, buffer, buffer_size, offset, granularity, count,\n"
"      timer=None) -> Sector\n\n"
"This method checks the region described by self for errors on the disk.\n"
"buffer is a temporary storage space used internally by check().  Do not rely\n"
"on its contents.  buffer_size describes the size of buffer in Sectors.  The\n"
"region to check starts at offset Sectors from the beginning of the region\n"
"and is count Sectors long.  granularity specifies how Sectors should be\n"
"grouped together.\n\n"
"This method returns the first bad sector, or 0 if there are no errors.");

PyDoc_STRVAR(geometry_map_doc,
"map(self, Geometry, Sector) -> integer\n\n"
"Given a Geometry that overlaps with self and a Sector inside Geometry,\n"
"this method translates the address of Sector into an address inside self.\n"
"The new address is returned, or ArithmeticError is raised if Sector does\n"
"not exist within self.");

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
    {"dev", T_OBJECT, offsetof(_ped_Geometry, dev), 0,
            "The _ped.Device described by this _ped.Geometry object."},
    {NULL}
};

static PyMethodDef _ped_Geometry_methods[] = {
    {"duplicate", (PyCFunction) py_ped_geometry_duplicate, METH_VARARGS,
                  geometry_duplicate_doc},
    {"intersect", (PyCFunction) py_ped_geometry_intersect, METH_VARARGS,
                  geometry_intersect_doc},
    {"destroy", (PyCFunction) py_ped_geometry_destroy, METH_VARARGS,
                geometry_destroy_doc},
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

void _ped_Geometry_dealloc(_ped_Geometry *self);
int _ped_Geometry_init(_ped_Geometry *self, PyObject *args, PyObject *kwds);
PyObject *_ped_Geometry_get(_ped_Geometry *self, void *closure);
int _ped_Geometry_set(_ped_Geometry *self, PyObject *value, void *closure);

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

PyDoc_STRVAR(_ped_Geometry_doc,
"A _ped.Geometry object describes a continuous region on a physical device.\n"
"This device is given by the dev attribute when the Geometry is created.\n"
"Most methods on this object involve creating new Geometry objects as needed\n"
"and can therefore raise _ped.CreateException when an error occurs creating\n"
"the new object.  Most methods can also raise _ped.IOException when reading\n"
"or writing the underlying physical device fails.\n\n"
"libparted (and therefore pyparted) attempts to enforce the following\n"
"conditions on Geometry objects:\n\n"
"\t- start + length - 1 == end\n"
"\t- length > 0\n"
"\t- start >= 0\n"
"\t- end < dev.length");

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
    .tp_getattro = PyObject_GenericGetAttr,
 /* .tp_setattro = XXX */
 /* .tp_as_buffer = XXX */
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_doc = _ped_Geometry_doc,
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
    .tp_alloc = PyType_GenericAlloc,
    .tp_new = PyType_GenericNew,
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
