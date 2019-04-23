/*
 * pygeom.c
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

#include <Python.h>

#include "convert.h"
#include "exceptions.h"
#include "pygeom.h"
#include "pynatmath.h"
#include "docstrings/pygeom.h"
#include "typeobjects/pygeom.h"

/* _ped.Geometry functions */
void _ped_Geometry_dealloc(_ped_Geometry *self) {
    if (self->ped_geometry)
        ped_geometry_destroy(self->ped_geometry);

    PyObject_GC_UnTrack(self);

    Py_CLEAR(self->dev);
    self->dev = NULL;

    PyObject_GC_Del(self);
}

int _ped_Geometry_compare(_ped_Geometry *self, PyObject *obj) {
    _ped_Geometry *comp = NULL;
    int check = PyObject_IsInstance(obj, (PyObject *) &_ped_Geometry_Type_obj);

    if (PyErr_Occurred()) {
        return -1;
    }

    if (!check) {
        PyErr_SetString(PyExc_ValueError, "object comparing to must be a _ped.Geometry");
        return -1;
    }

    comp = (_ped_Geometry *) obj;
    if (PyObject_RichCompareBool(self->dev, comp->dev, Py_EQ) &&
        (self->ped_geometry->start == comp->ped_geometry->start) &&
        (self->ped_geometry->length == comp->ped_geometry->length) &&
        (self->ped_geometry->end == comp->ped_geometry->end)) {
        return 0;
    } else {
        return 1;
    }
}

PyObject *_ped_Geometry_richcompare(_ped_Geometry *a, PyObject *b, int op) {
    if (op == Py_EQ || op == Py_NE) {
        int rv = _ped_Geometry_compare(a, b);
        if (PyErr_Occurred())
            return NULL;
        return PyBool_FromLong(op == Py_EQ ? rv == 0 : rv != 0);
    } else if ((op == Py_LT) || (op == Py_LE) ||
               (op == Py_GT) || (op == Py_GE)) {
        PyErr_SetString(PyExc_TypeError, "comparison operator not supported for _ped.Geometry");
        return NULL;
    } else {
        PyErr_SetString(PyExc_ValueError, "unknown richcompare op");
        return NULL;
    }
}

PyObject *_ped_Geometry_str(_ped_Geometry *self) {
    char *ret = NULL;
    char *dev = NULL;

    dev = (char *) PyUnicode_AsUTF8(_ped_Device_Type_obj.tp_repr(self->dev));
    if (dev == NULL) {
        return NULL;
    }

    if (asprintf(&ret, "_ped.Geometry instance --\n"
                       "  start: %lld  end: %lld  length: %lld\n"
                       "  device: %s",
                 self->ped_geometry->start, self->ped_geometry->end,
                 self->ped_geometry->length, dev) == -1) {
        return PyErr_NoMemory();
    }

    return Py_BuildValue("s", ret);
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
    Py_CLEAR(self->dev);
    self->dev = NULL;

    return 0;
}

int _ped_Geometry_init(_ped_Geometry *self, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"dev", "start", "length", "end", NULL};
    PedDevice *device = NULL;
    long long start, length, end;

    self->dev = NULL;
    self->ped_geometry = NULL;

    if (kwds == NULL) {
        if (!PyArg_ParseTuple(args, "O!LL|L", &_ped_Device_Type_obj, &self->dev,
                              &start, &length, &end)) {
            self->dev = NULL;
            return -1;
        }
    } else {
        if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!LL|L", kwlist,
                                         &_ped_Device_Type_obj, &self->dev,
                                         &start, &length, &end)) {
            self->dev = NULL;
            return -2;
        }
    }

    device = _ped_Device2PedDevice(self->dev);
    if (device == NULL) {
        self->dev = NULL;
        return -3;
    }
    self->ped_geometry = ped_geometry_new(device, start, length);

    if (self->ped_geometry == NULL) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError)) {
                PyErr_SetString(CreateException, partedExnMessage);
            }
        } else {
            PyErr_SetString(CreateException, "Could not create new geometry");
        }

        self->dev = NULL;
        return -3;
    }

    Py_INCREF(self->dev);

    return 0;
}

PyObject *_ped_Geometry_get(_ped_Geometry *self, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Geometry()");
        return NULL;
    }

    if (!strcmp(member, "start")) {
        return PyLong_FromLongLong(self->ped_geometry->start);
    } else if (!strcmp(member, "length")) {
        return PyLong_FromLongLong(self->ped_geometry->length);
    } else if (!strcmp(member, "end")) {
        return PyLong_FromLongLong(self->ped_geometry->end);
    } else {
        PyErr_Format(PyExc_AttributeError, "_ped.Geometry object has no attribute %s", member);
        return NULL;
    }
}

int _ped_Geometry_set(_ped_Geometry *self, PyObject *value, void *closure) {
    char *member = (char *) closure;
    long long val;
    int ret;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Geometry()");
        return -1;
    }

    if (!strcmp(member, "start")) {
        val = PyLong_AsLongLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
        ret = ped_geometry_set_start(self->ped_geometry, val);
    } else if (!strcmp(member, "length")) {
        val = PyLong_AsLongLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
        ret = ped_geometry_set(self->ped_geometry, self->ped_geometry->start,
                               val);
    } else if (!strcmp(member, "end")) {
        val = PyLong_AsLongLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
        ret = ped_geometry_set_end(self->ped_geometry, val);
    } else {
        PyErr_Format(PyExc_AttributeError, "_ped.Geometry object has no attribute %s", member);
        return -1;
    }

    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_Occurred()) {
                PyErr_SetString(PyExc_ValueError, partedExnMessage);
            }
        } else {
            PyErr_SetString(PyExc_ValueError, "Could not set geometry");
        }
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

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                PyErr_SetString(CreateException, partedExnMessage);
        }
        else
            PyErr_SetString(CreateException, "Could not duplicate geometry");

        return NULL;
    }

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

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                PyErr_SetString(CreateException, partedExnMessage);
        }
        else
            PyErr_SetString(PyExc_ArithmeticError, "Could not find geometry intersection");

        return NULL;
    }

    return (PyObject *) ret;
}

PyObject *py_ped_geometry_set(PyObject *s, PyObject *args) {
    int ret = -1;
    PedGeometry *geom = NULL;
    PedSector start, length;

    if (!PyArg_ParseTuple(args, "LL", &start, &length)) {
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

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                PyErr_SetString(CreateException, partedExnMessage);
        }
        else
            PyErr_SetString(CreateException, "Could not create new geometry");

        return NULL;
    }

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_geometry_set_start(PyObject *s, PyObject *args) {
    int ret = -1;
    PedGeometry *geom = NULL;
    PedSector start;

    if (!PyArg_ParseTuple(args, "L", &start)) {
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

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                PyErr_SetString(CreateException, partedExnMessage);
        }
        else
            PyErr_SetString(CreateException, "Could not create new geometry");

        return NULL;
    }

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_geometry_set_end(PyObject *s, PyObject *args) {
    int ret = -1;
    PedGeometry *geom = NULL;
    PedSector end;

    if (!PyArg_ParseTuple(args, "L", &end)) {
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

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                PyErr_SetString(CreateException, partedExnMessage);
        }
        else
            PyErr_SetString(CreateException, "Could not create new geometry");

        return NULL;
    }

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
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

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
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

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
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

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_geometry_test_sector_inside(PyObject *s, PyObject *args) {
    int ret = -1;
    PedGeometry *geom = NULL;
    PedSector sector;

    if (!PyArg_ParseTuple(args, "L", &sector)) {
        return NULL;
    }

    geom = _ped_Geometry2PedGeometry(s);
    if (geom == NULL) {
        return NULL;
    }

    ret = ped_geometry_test_sector_inside(geom, sector);

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_geometry_read(PyObject *s, PyObject *args) {
    PyObject *ret = NULL;
    PedGeometry *geom = NULL;
    char *out_buf = NULL;
    PedSector offset, count;

    if (!PyArg_ParseTuple(args, "LL", &offset, &count)) {
        return NULL;
    }

    geom = _ped_Geometry2PedGeometry(s);
    if (geom == NULL) {
        return NULL;
    }

    /* py_device_read will ASSERT if the device isn't open yet. */
    if (geom->dev->open_count <= 0) {
        PyErr_SetString(IOException, "Attempting to read from a unopened device");
        return NULL;
    }

    /* And then py_geometry_read will ASSERT on these things too. */
    if (offset < 0 || count < 0) {
        PyErr_SetString(IOException, "offset and count cannot be negative.");
        return NULL;
    }

    if ((out_buf = malloc(geom->dev->sector_size * count)) == NULL) {
        return PyErr_NoMemory();
    }

    if (ped_geometry_read(geom, out_buf, offset, count) == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_SetString(IOException, "Could not read from given region");

        free(out_buf);
        return NULL;
    }

    ret = PyUnicode_FromString(out_buf);
    free(out_buf);

    return ret;
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

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
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

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_geometry_write(PyObject *s, PyObject *args) {
    int ret = -1;
    char *in_buf = NULL;
    Py_ssize_t in_buflen;
    PedGeometry *geom = NULL;
    PedSector offset, count;

    if (!PyArg_ParseTuple(args, "s#LL", &in_buf, &in_buflen, &offset, &count)) {
        return NULL;
    }

    geom = _ped_Geometry2PedGeometry(s);
    if (geom == NULL) {
        return NULL;
    }

    /* py_device_write will ASSERT if the device isn't open yet. */
    if (geom->dev->open_count <= 0) {
        PyErr_SetString(IOException, "Attempting to write to a unopened device");
        return NULL;
    }

    /* And then py_geometry_wriet will ASSERT on these things too. */
    if (offset < 0 || count < 0) {
        PyErr_SetString(IOException, "offset and count cannot be negative.");
        return NULL;
    }

    ret = ped_geometry_write(geom, in_buf, offset, count);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_SetString(IOException, "Could not write to given region");

        return NULL;
    }

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_geometry_check(PyObject *s, PyObject *args) {
    PyObject *in_timer = NULL;
    PedGeometry *geom = NULL;
    PedSector offset, granularity, count, ret;
    PedTimer *out_timer = NULL;
    char *out_buf = NULL;

    if (!PyArg_ParseTuple(args, "LLL|O!", &offset, &granularity, &count,
                          &_ped_Timer_Type_obj, &in_timer)) {
        return NULL;
    }

    geom = _ped_Geometry2PedGeometry(s);
    if (geom == NULL) {
        return NULL;
    }

    if (!geom->dev->open_count) {
        PyErr_Format(IOException, "Device %s is not open.", geom->dev->path);
        return NULL;
    }

    if (geom->dev->external_mode) {
        PyErr_Format(IOException, "Device %s is already open for external access.", geom->dev->path);
        return NULL;
    }

    if (in_timer)
       out_timer = _ped_Timer2PedTimer(in_timer);
    else
       out_timer = NULL;

    if ((out_buf = malloc(geom->dev->sector_size * 32)) == NULL) {
        ped_timer_destroy(out_timer);
        return PyErr_NoMemory();
    }

    ret = ped_geometry_check(geom, out_buf, 32, offset,
                             granularity, count, out_timer);
    ped_timer_destroy(out_timer);
    free(out_buf);

    return PyLong_FromLongLong(ret);
}

PyObject *py_ped_geometry_map(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_dst = NULL;
    PedGeometry *out_dst = NULL, *src = NULL;
    PedSector sector;

    if (!PyArg_ParseTuple(args, "O!L", &_ped_Geometry_Type_obj, &in_dst,
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

    return Py_BuildValue("i", ret);
}

/* vim:tw=78:ts=4:et:sw=4
 */
