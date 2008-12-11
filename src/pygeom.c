/*
 * pygeom.c
 *
 * Copyright (C) 2007  Red Hat, Inc.
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
 * Red Hat Author(s): David Cantrell <dcantrell@redhat.com>
 *                    Chris Lumens <clumens@redhat.com>
 */

#include <Python.h>

#include "convert.h"
#include "exceptions.h"
#include "pygeom.h"
#include "pynatmath.h"

/* _ped.Geometry type object */
PyTypeObject _ped_Geometry_Type_obj = {
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
    .tp_setattro = PyObject_GenericSetAttr,
 /* .tp_as_buffer = XXX */
    .tp_flags = Py_TPFLAGS_HAVE_CLASS | Py_TPFLAGS_BASETYPE |
                Py_TPFLAGS_HAVE_GC,
    .tp_doc = _ped_Geometry_doc,
    .tp_traverse = (traverseproc) _ped_Geometry_traverse,
    .tp_clear = (inquiry) _ped_Geometry_clear,
 /* .tp_richcompare = XXX */
 /* .tp_weaklistoffset = XXX */
 /* .tp_iter = XXX */
 /* .tp_iternext = XXX */
    .tp_methods = _ped_Geometry_methods,
    .tp_members = _ped_Geometry_members,
    .tp_getset = _ped_Geometry_getset,
    .tp_base = NULL,
 /* .tp_dict = XXX */
 /* .tp_descr_get = XXX */
 /* .tp_descr_set = XXX */
 /* .tp_dictoffset = XXX */
    .tp_init = (initproc) _ped_Geometry_init,
    .tp_alloc = PyType_GenericAlloc,
    .tp_new = PyType_GenericNew,
 /* .tp_free = XXX */
 /* .tp_is_gc = XXX */
    .tp_bases = NULL,
 /* .tp_mro = XXX */
 /* .tp_cache = XXX */
 /* .tp_subclasses = XXX */
 /* .tp_weaklist = XXX */
 /* .tp_del = XXX */
};

/* _ped.Geometry functions */
void _ped_Geometry_dealloc(_ped_Geometry *self) {
    PyObject_GC_UnTrack(self);
    Py_XDECREF(self->dev);
    PyObject_GC_Del(self);
}

int _ped_Geometry_traverse(_ped_Geometry *self, visitproc visit, void *arg) {
    int err;

    if (self->dev) {
        if ((err = visit(self->dev, arg))) {
            return err;
        }
    }

    return 0;
}

int _ped_Geometry_clear(_ped_Geometry *self) {
    Py_XDECREF(self->dev);
    self->dev = NULL;

    return 0;
}

int _ped_Geometry_init(_ped_Geometry *self, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"dev", "start", "length", "end", NULL};
    PedGeometry *geometry = NULL;
    PedDevice *device = NULL;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!ll|l", kwlist,
                                     &_ped_Device_Type_obj, &self->dev,
                                     &self->start, &self->length, &self->end)) {
        return -1;
    } else {
        device = _ped_Device2PedDevice(self->dev);
        geometry = ped_geometry_new(device, self->start, self->length);

        if (geometry) {
            self->end = geometry->end;
        } else {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException)) {
                    PyErr_SetString(CreateException, partedExnMessage);
                }
            }

            ped_device_destroy(device);
            return -1;
        }

        Py_XDECREF(self);
        self = PedGeometry2_ped_Geometry(geometry);

        ped_device_destroy(device);
        ped_geometry_destroy(geometry);
        return 0;
    }
}

PyObject *_ped_Geometry_get(_ped_Geometry *self, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Geometry()");
        return NULL;
    }

    if (!strcmp(member, "start")) {
        return PyLong_FromLongLong(self->start);
    } else if (!strcmp(member, "length")) {
        return PyLong_FromLongLong(self->length);
    } else if (!strcmp(member, "end")) {
        return PyLong_FromLongLong(self->end);
    } else {
        PyErr_Format(PyExc_AttributeError, "_ped.Geometry object has no attribute %s", member);
        return NULL;
    }
}

int _ped_Geometry_set(_ped_Geometry *self, PyObject *value, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Geometry()");
        return -1;
    }

    if (!strcmp(member, "start")) {
        self->start = PyLong_AsLongLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else if (!strcmp(member, "length")) {
        self->length = PyLong_AsLongLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else if (!strcmp(member, "end")) {
        self->end = PyLong_AsLongLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else {
        PyErr_Format(PyExc_AttributeError, "_ped.Geometry object has no attribute %s", member);
        return -1;
    }

    return 0;
}

/* 1:1 function mappings for geom.h in libparted */
PyObject *py_ped_geometry_duplicate(PyObject *s, PyObject *args) {
    PedGeometry *geometry = NULL, *geom = NULL;
    _ped_Geometry *ret = NULL;

    geometry = _ped_Geometry2PedGeometry(s);
    if (geometry == NULL) {
        return NULL;
    }

    geom = ped_geometry_duplicate(geometry);
    if (geom) {
        ret = PedGeometry2_ped_Geometry(geom);
    }
    else {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(CreateException, partedExnMessage);
        }
        else
            PyErr_SetString(CreateException, "Could not duplicate geometry");

        return NULL;
    }

    ped_geometry_destroy(geometry);
    ped_geometry_destroy(geom);

    return (PyObject *) ret;
}

PyObject *py_ped_geometry_intersect(PyObject *s, PyObject *args) {
    PyObject *in_b = NULL;
    PedGeometry *out_a = NULL, *out_b = NULL, *geom = NULL;
    _ped_Geometry *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Geometry_Type_obj, &in_b)) {
        return NULL;
    }

    out_a = _ped_Geometry2PedGeometry(s);
    if (out_a == NULL) {
        return NULL;
    }

    out_b = _ped_Geometry2PedGeometry(in_b);
    if (out_b == NULL) {
        return NULL;
    }

    geom = ped_geometry_intersect (out_a, out_b);
    if (geom) {
        ret = PedGeometry2_ped_Geometry(geom);
    }
    else {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(CreateException, partedExnMessage);
        }
        else
            PyErr_SetString(PyExc_ArithmeticError, "Could not find geometry intersection");

        return NULL;
    }

    return (PyObject *) ret;
}

PyObject *py_ped_geometry_destroy(PyObject *s, PyObject *args) {
    PyObject_GC_Del(s);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_geometry_set(PyObject *s, PyObject *args) {
    int ret = -1;
    PedGeometry *geom = NULL;
    PedSector start, length;

    if (!PyArg_ParseTuple(args, "ll", &start, &length)) {
        return NULL;
    }

    geom = _ped_Geometry2PedGeometry(s);
    if (geom == NULL) {
        return NULL;
    }

    ret = ped_geometry_set(geom, start, length);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(CreateException, partedExnMessage);
        }
        else
            PyErr_SetString(CreateException, "Could not create new geometry");

        return NULL;
    }

    ped_geometry_destroy(geom);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_set_start(PyObject *s, PyObject *args) {
    int ret = -1;
    PedGeometry *geom = NULL;
    PedSector start;

    if (!PyArg_ParseTuple(args, "l", &start)) {
        return NULL;
    }

    geom = _ped_Geometry2PedGeometry(s);
    if (geom == NULL) {
        return NULL;
    }

    ret = ped_geometry_set_start(geom, start);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(CreateException, partedExnMessage);
        }
        else
            PyErr_SetString(CreateException, "Could not create new geometry");

        return NULL;
    }

    ped_geometry_destroy(geom);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_set_end(PyObject *s, PyObject *args) {
    int ret = -1;
    PedGeometry *geom = NULL;
    PedSector end;

    if (!PyArg_ParseTuple(args, "l", &end)) {
        return NULL;
    }

    geom = _ped_Geometry2PedGeometry(s);
    if (geom == NULL) {
        return NULL;
    }

    ret = ped_geometry_set_end(geom, end);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(CreateException, partedExnMessage);
        }
        else
            PyErr_SetString(CreateException, "Could not create new geometry");

        return NULL;
    }

    ped_geometry_destroy(geom);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_test_overlap(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_b = NULL;
    PedGeometry *out_a = NULL, *out_b = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Geometry_Type_obj, &in_b)) {
        return NULL;
    }

    out_a = _ped_Geometry2PedGeometry(s);
    if (out_a == NULL) {
        return NULL;
    }

    out_b = _ped_Geometry2PedGeometry(in_b);
    if (out_b == NULL) {
        return NULL;
    }

    ret = ped_geometry_test_overlap(out_a, out_b);
    ped_geometry_destroy(out_a);
    ped_geometry_destroy(out_b);
    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_test_inside(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_b = NULL;
    PedGeometry *out_a = NULL, *out_b = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Geometry_Type_obj, &in_b)) {
        return NULL;
    }

    out_a = _ped_Geometry2PedGeometry(s);
    if (out_a == NULL) {
        return NULL;
    }

    out_b = _ped_Geometry2PedGeometry(in_b);
    if (out_b == NULL) {
        return NULL;
    }

    ret = ped_geometry_test_inside(out_a, out_b);
    ped_geometry_destroy(out_a);
    ped_geometry_destroy(out_b);
    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_test_equal(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_b = NULL;
    PedGeometry *out_a = NULL, *out_b = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Geometry_Type_obj, &in_b)) {
        return NULL;
    }

    out_a = _ped_Geometry2PedGeometry(s);
    if (out_a == NULL) {
        return NULL;
    }

    out_b = _ped_Geometry2PedGeometry(in_b);
    if (out_b == NULL) {
        return NULL;
    }

    ret = ped_geometry_test_equal(out_a, out_b);
    ped_geometry_destroy(out_a);
    ped_geometry_destroy(out_b);
    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_test_sector_inside(PyObject *s, PyObject *args) {
    int ret = -1;
    PedGeometry *geom = NULL;
    PedSector sector;

    if (!PyArg_ParseTuple(args, "l", &sector)) {
        return NULL;
    }

    geom = _ped_Geometry2PedGeometry(s);
    if (geom == NULL) {
        return NULL;
    }

    ret = ped_geometry_test_sector_inside(geom, sector);
    ped_geometry_destroy(geom);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_read(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_buf = NULL;
    PedGeometry *geom = NULL;
    void *out_buf = NULL;
    PedSector offset, count;

    if (!PyArg_ParseTuple(args, "Oll", &in_buf, &offset, &count)) {
        return NULL;
    }

    geom = _ped_Geometry2PedGeometry(s);
    if (geom == NULL) {
        return NULL;
    }

    out_buf = PyCObject_AsVoidPtr(in_buf);
    if (out_buf == NULL) {
        return NULL;
    }

    ret = ped_geometry_read(geom, out_buf, offset, count);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_SetString(IOException, "Could not read from given region");

        return NULL;
    }

    ped_geometry_destroy(geom);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_sync(PyObject *s, PyObject *args) {
    int ret = -1;
    PedGeometry *geom = NULL;

    geom = _ped_Geometry2PedGeometry(s);
    if (geom == NULL) {
        return NULL;
    }

    ret = ped_geometry_sync(geom);
    if (ret == 0) {
        PyErr_SetString(IOException, "Could not sync");
        return NULL;
    }

    ped_geometry_destroy(geom);
    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_sync_fast(PyObject *s, PyObject *args) {
    int ret = -1;
    PedGeometry *geom = NULL;

    geom = _ped_Geometry2PedGeometry(s);
    if (geom == NULL) {
        return NULL;
    }

    ret = ped_geometry_sync_fast(geom);
    if (ret == 0) {
        PyErr_SetString(IOException, "Could not sync");
        return NULL;
    }

    ped_geometry_destroy(geom);
    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_write(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_buf = NULL;
    PedGeometry *geom = NULL;
    void *out_buf = NULL;
    PedSector offset, count;

    if (!PyArg_ParseTuple(args, "Oll", &in_buf, &offset, &count)) {
        return NULL;
    }

    geom = _ped_Geometry2PedGeometry(s);
    if (geom == NULL) {
        return NULL;
    }

    out_buf = PyCObject_AsVoidPtr(in_buf);
    if (out_buf == NULL) {
        return NULL;
    }

    ret = ped_geometry_write(geom, out_buf, offset, count);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_SetString(IOException, "Could not write to given region");

        return NULL;
    }

    ped_geometry_destroy(geom);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_check(PyObject *s, PyObject *args) {
    PyObject *in_buf = NULL, *in_timer = NULL;
    PedGeometry *geom = NULL;
    void *out_buf = NULL;
    PedSector buffer_size, offset, granularity, count, ret;
    PedTimer *out_timer = NULL;

    if (!PyArg_ParseTuple(args, "Ollll|O!", &in_buf, &buffer_size, &offset,
                          &granularity, &count, &_ped_Timer_Type_obj,
                          &in_timer)) {
        return NULL;
    }

    geom = _ped_Geometry2PedGeometry(s);
    if (geom == NULL) {
        return NULL;
    }

    if (in_timer)
       out_timer = _ped_Timer2PedTimer(in_timer);
    else
       out_timer = NULL;

    out_buf = PyCObject_AsVoidPtr(in_buf);
    if (out_buf == NULL) {
        return NULL;
    }

    ret = ped_geometry_check(geom, out_buf, buffer_size, offset,
                             granularity, count, out_timer);
    ped_geometry_destroy(geom);
    ped_timer_destroy(out_timer);

    return PyLong_FromLongLong(ret);
}

PyObject *py_ped_geometry_map(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_dst = NULL;
    PedGeometry *out_dst = NULL, *src = NULL;
    PedSector sector;

    if (!PyArg_ParseTuple(args, "O!l", &_ped_Geometry_Type_obj, &in_dst,
                          &sector)) {
        return NULL;
    }

    src = _ped_Geometry2PedGeometry(s);
    if (src == NULL) {
        return NULL;
    }

    out_dst = _ped_Geometry2PedGeometry(in_dst);
    if (out_dst == NULL) {
        return NULL;
    }

    ret = ped_geometry_map(out_dst, src, sector);
    if (ret == -1) {
        PyErr_SetString(PyExc_ArithmeticError, "Sector must exist within region given by geometry");
        return NULL;
    }

    ped_geometry_destroy(src);
    ped_geometry_destroy(out_dst);

    return Py_BuildValue("i", ret);
}

/* vim:tw=78:ts=4:et:sw=4
 */
