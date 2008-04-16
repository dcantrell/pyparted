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

/* _ped.Geometry functions */
void _ped_Geometry_dealloc(_ped_Geometry *self) {
    PyObject_Del(self);
}

PyObject *_ped_Geometry_new(PyTypeObject *type, PyObject *args,
                            PyObject *kwds) {
    _ped_Geometry *self;

    self = PyObject_New(_ped_Geometry, &_ped_Geometry_Type_obj);
    return (PyObject *) self;
}

int _ped_Geometry_init(_ped_Geometry *self, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"dev", "start", "length", "end", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O!lll", kwlist,
                                     &_ped_Device_Type_obj, &self->dev,
                                     &self->start, &self->length, &self->end))
        return -1;
    else
        return 0;
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
PyObject *py_ped_geometry_init(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_geometry, *in_device;
    PedGeometry *out_geometry;
    PedDevice *out_device;
    PedSector start, length;

    if (!PyArg_ParseTuple(args, "O!O!ll", &_ped_Geometry_Type_obj, &in_geometry,
                          &_ped_Device_Type_obj, &in_device, &start, &length)) {
        return NULL;
    }

    out_geometry = _ped_Geometry2PedGeometry(in_geometry);
    if (out_geometry == NULL) {
        return NULL;
    }

    out_device = _ped_Device2PedDevice(in_device);
    if (out_device == NULL) {
        return NULL;
    }

    ret = ped_geometry_init(out_geometry, out_device, start, length);
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

    ped_geometry_destroy(out_geometry);
    ped_device_destroy(out_device);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_new(PyObject *s, PyObject *args) {
    PyObject *in_device;
    PedGeometry *geom;
    PedDevice *out_device;
    PedSector start, length;
    _ped_Geometry *ret;

    if (!PyArg_ParseTuple(args, "O!ll", &_ped_Device_Type_obj, &in_device,
                          &start, &length)) {
        return NULL;
    }

    out_device = _ped_Device2PedDevice(in_device);
    if (out_device == NULL) {
        return NULL;
    }

    geom = ped_geometry_new(out_device, start, length);
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
            PyErr_SetString(CreateException, "Could not create new geometry");

        return NULL;
    }

    ped_geometry_destroy(geom);
    ped_device_destroy(out_device);

    return (PyObject *) ret;
}

PyObject *py_ped_geometry_duplicate(PyObject *s, PyObject *args) {
    PyObject *in_geometry;
    PedGeometry *out_geometry, *geom;
    _ped_Geometry *ret;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Geometry_Type_obj, &in_geometry)) {
        return NULL;
    }

    out_geometry = _ped_Geometry2PedGeometry(in_geometry);
    if (out_geometry == NULL) {
        return NULL;
    }

    geom = ped_geometry_duplicate(out_geometry);
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

    ped_geometry_destroy(out_geometry);
    ped_geometry_destroy(geom);

    return (PyObject *) ret;
}

PyObject *py_ped_geometry_intersect(PyObject *s, PyObject *args) {
    PyObject *in_a, *in_b;
    PedGeometry *out_a, *out_b, *geom;
    _ped_Geometry *ret;

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_Geometry_Type_obj, &in_a,
                          &_ped_Geometry_Type_obj, &in_b)) {
        return NULL;
    }

    out_a = _ped_Geometry2PedGeometry(in_a);
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
    PyObject *in_geometry;
    PedGeometry *out_geometry;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Geometry_Type_obj, &in_geometry)) {
        return NULL;
    }

    out_geometry = _ped_Geometry2PedGeometry(in_geometry);
    if (out_geometry == NULL) {
        return NULL;
    }

    ped_geometry_destroy(out_geometry);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_geometry_set(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_geom;
    PedGeometry *out_geom;
    PedSector start, length;

    if (!PyArg_ParseTuple(args, "O!ll", &_ped_Geometry_Type_obj, &in_geom,
                          &start, &length)) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    ret = ped_geometry_set(out_geom, start, length);
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

    ped_geometry_destroy(out_geom);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_set_start(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_geom;
    PedGeometry *out_geom;
    PedSector start;

    if (!PyArg_ParseTuple(args, "O!l", &_ped_Geometry_Type_obj, &in_geom,
                          &start)) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    ret = ped_geometry_set_start(out_geom, start);
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

    ped_geometry_destroy(out_geom);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_set_end(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_geom;
    PedGeometry *out_geom;
    PedSector end;

    if (!PyArg_ParseTuple(args, "O!l", &_ped_Geometry_Type_obj, &in_geom,
                          &end)) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    ret = ped_geometry_set_end(out_geom, end);
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

    ped_geometry_destroy(out_geom);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_test_overlap(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_a, *in_b;
    PedGeometry *out_a, *out_b;

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_Geometry_Type_obj ,&in_a,
                          &_ped_Geometry_Type_obj, &in_b)) {
        return NULL;
    }

    out_a = _ped_Geometry2PedGeometry(in_a);
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
    PyObject *in_a, *in_b;
    PedGeometry *out_a, *out_b;

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_Geometry_Type_obj, &in_a,
                          &_ped_Geometry_Type_obj, &in_b)) {
        return NULL;
    }

    out_a = _ped_Geometry2PedGeometry(in_a);
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
    PyObject *in_a, *in_b;
    PedGeometry *out_a, *out_b;

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_Geometry_Type_obj, &in_a,
                          &_ped_Geometry_Type_obj, &in_b)) {
        return NULL;
    }

    out_a = _ped_Geometry2PedGeometry(in_a);
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
    PyObject *in_geom;
    PedGeometry *out_geom;
    PedSector sector;

    if (!PyArg_ParseTuple(args, "O!l", &_ped_Geometry_Type_obj, &in_geom,
                          &sector)) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    ret = ped_geometry_test_sector_inside(out_geom, sector);
    ped_geometry_destroy(out_geom);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_read(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_geom, *in_buf;
    PedGeometry *out_geom;
    void *out_buf;
    PedSector offset, count;

    if (!PyArg_ParseTuple(args, "O!Oll", &_ped_Geometry_Type_obj, &in_geom,
                          &in_buf, &offset, &count)) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    out_buf = PyCObject_AsVoidPtr(in_buf);
    if (out_buf == NULL) {
        return NULL;
    }

    ret = ped_geometry_read(out_geom, out_buf, offset, count);
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

    ped_geometry_destroy(out_geom);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_sync(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_geom;
    PedGeometry *out_geom;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Geometry_Type_obj, &in_geom)) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    ret = ped_geometry_sync(out_geom);
    if (ret == 0) {
        PyErr_SetString(IOException, "Could not sync");
        return NULL;
    }

    ped_geometry_destroy(out_geom);
    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_sync_fast(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_geom;
    PedGeometry *out_geom;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Geometry_Type_obj, &in_geom)) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    ret = ped_geometry_sync_fast(out_geom);
    if (ret == 0) {
        PyErr_SetString(IOException, "Could not sync");
        return NULL;
    }

    ped_geometry_destroy(out_geom);
    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_write(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_geom, *in_buf;
    PedGeometry *out_geom;
    void *out_buf;
    PedSector offset, count;

    if (!PyArg_ParseTuple(args, "O!Oll", &_ped_Geometry_Type_obj, &in_geom,
                          &in_buf, &offset, &count)) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    out_buf = PyCObject_AsVoidPtr(in_buf);
    if (out_buf == NULL) {
        return NULL;
    }

    ret = ped_geometry_write(out_geom, out_buf, offset, count);
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

    ped_geometry_destroy(out_geom);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_geometry_check(PyObject *s, PyObject *args) {
    PyObject *in_geom, *in_buf, *in_timer = NULL;
    PedGeometry *out_geom;
    void *out_buf;
    PedSector buffer_size, offset, granularity, count, ret;
    PedTimer *out_timer;

    if (!PyArg_ParseTuple(args, "O!Ollll|O!", &_ped_Geometry_Type_obj, &in_geom,
                          &in_buf, &buffer_size, &offset, &granularity, &count,
                          &_ped_Timer_Type_obj, &in_timer)) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
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

    ret = ped_geometry_check(out_geom, out_buf, buffer_size, offset,
                             granularity, count, out_timer);
    ped_geometry_destroy(out_geom);
    ped_timer_destroy(out_timer);

    return PyLong_FromLongLong(ret);
}

PyObject *py_ped_geometry_map(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_dst, *in_src;
    PedGeometry *out_dst, *out_src;
    PedSector sector;

    if (!PyArg_ParseTuple(args, "O!O!l", &_ped_Geometry_Type_obj, &in_dst,
                          &_ped_Geometry_Type_obj, &in_src, &sector)) {
        return NULL;
    }

    out_dst = _ped_Geometry2PedGeometry(in_dst);
    if (out_dst == NULL) {
        return NULL;
    }

    out_src = _ped_Geometry2PedGeometry(in_src);
    if (out_src == NULL) {
        return NULL;
    }

    ret = ped_geometry_map(out_dst, out_src, sector);
    if (ret == -1) {
        PyErr_SetString(PyExc_ArithmeticError, "Sector must exist within region given by geometry");
        return NULL;
    }

    ped_geometry_destroy(out_dst);
    ped_geometry_destroy(out_src);

    return Py_BuildValue("i", ret);
}
