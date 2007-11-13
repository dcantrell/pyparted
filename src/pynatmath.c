/*
 * pynatmath.c
 *
 * Copyright (C) 2007  Red Hat, Inc.
 * All rights reserved.
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
#include "pydevice.h"
#include "pynatmath.h"

/* _ped.Alignment functions */
void _ped_Alignment_dealloc(_ped_Alignment *self) {
    PyObject_Del(self);
}

PyObject *_ped_Alignment_new(PyTypeObject *type, PyObject *args,
                             PyObject *kwds) {
    _ped_Alignment *self;

    self = PyObject_New(_ped_Alignment, &_ped_Alignment_Type_obj);
    return (PyObject *) self;
}

int _ped_Alignment_init(_ped_Alignment *self, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"offset", "grain_size", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|ll", kwlist,
                                     &self->offset, &self->grain_size))
        return -1;
    else
        return 0;
}

PyObject *_ped_Alignment_get(_ped_Alignment *self, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Alignment()");
        return NULL;
    }

    if (!strcmp(member, "offset")) {
        return PyLong_FromLongLong(self->offset);
    } else if (!strcmp(member, "grain_size")) {
        return PyLong_FromLongLong(self->grain_size);
    } else {
        PyErr_Format(PyExc_AttributeError, "_ped.Alignment object has no attribute %s", member);
        return NULL;
    }
}

int _ped_Alignment_set(_ped_Alignment *self, PyObject *value, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        return NULL;
    }

    if (!strcmp(member, "offset")) {
        self->offset = PyLong_AsLongLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else if (!strcmp(member, "grain_size")) {
        self->grain_size = PyLong_AsLongLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else {
        return -1;
    }

    return 0;
}

/* 1:1 function mappings for natmath.h in libparted */
PyObject *py_ped_round_up_to(PyObject *s, PyObject *args) {
    PedSector sector, grain_size;

    if (!PyArg_ParseTuple(args, "ll", &sector, &grain_size)) {
        return NULL;
    }

    return PyLong_FromLongLong(ped_round_up_to(sector, grain_size));
}

PyObject *py_ped_round_down_to(PyObject *s, PyObject *args) {
    PedSector sector, grain_size;

    if (!PyArg_ParseTuple(args, "ll", &sector, &grain_size)) {
        return NULL;
    }

    return PyLong_FromLongLong(ped_round_down_to(sector, grain_size));
}

PyObject *py_ped_round_to_nearest(PyObject *s, PyObject *args) {
    PedSector sector, grain_size;

    if (!PyArg_ParseTuple(args, "ll", &sector, &grain_size)) {
        return NULL;
    }

    return PyLong_FromLongLong(ped_round_to_nearest(sector, grain_size));
}

PyObject *py_ped_greatest_common_divisor(PyObject *s, PyObject *args) {
    PedSector a, b;

    if (!PyArg_ParseTuple(args, "ll", &a, &b)) {
        return NULL;
    }

    return PyLong_FromLongLong(ped_greatest_common_divisor(a, b));
}

PyObject *py_ped_alignment_init(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_alignment;
    PedAlignment *out_alignment;
    PedSector offset, grain_size;

    if (!PyArg_ParseTuple(args, "O!ll", &_ped_Alignment_Type_obj, &in_alignment,
                          &offset, &grain_size)) {
        return NULL;
    }

    out_alignment = _ped_Alignment2PedAlignment(in_alignment);
    if (out_alignment == NULL) {
        return NULL;
    }

    ret = ped_alignment_init(out_alignment, offset, grain_size);
    if (ret == 0) {
        PyErr_SetString(CreateException, "Could not create new alignment: grain_size must be >= 0");
        return NULL;
    }

    ped_alignment_destroy(out_alignment);

    return Py_BuildValue("i", ret);
}

PyObject *py_ped_alignment_new(PyObject *s, PyObject *args) {
    PedSector offset, grain_size;
    PedAlignment *align;
    _ped_Alignment *ret;

    if (!PyArg_ParseTuple(args, "ll", &offset, &grain_size)) {
        return NULL;
    }

    align = ped_alignment_new(offset, grain_size);
    if (align) {
        ret = PedAlignment2_ped_Alignment(align);
        if (ret == NULL) {
            return NULL;
        }
    }
    else {
        PyErr_SetString(CreateException, "Could not create new alignment");
        return NULL;
    }

    ped_alignment_destroy(align);

    return (PyObject *) ret;
}

PyObject *py_ped_alignment_destroy(PyObject *s, PyObject *args) {
    PyObject *in_alignment;
    PedAlignment *out_alignment;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Alignment_Type_obj, &in_alignment))
        return NULL;

    out_alignment = _ped_Alignment2PedAlignment(in_alignment);
    if (out_alignment == NULL) {
        return NULL;
    }

    ped_alignment_destroy(out_alignment);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_alignment_duplicate(PyObject *s, PyObject *args) {
    PyObject *in_alignment;
    PedAlignment *out_alignment, *align;
    _ped_Alignment *ret;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Alignment_Type_obj, &in_alignment))
        return NULL;

    out_alignment = _ped_Alignment2PedAlignment(in_alignment);
    if (out_alignment == NULL) {
        return NULL;
    }

    align = ped_alignment_duplicate(out_alignment);
    if (align) {
        ret = PedAlignment2_ped_Alignment(align);
        if (ret == NULL) {
            return NULL;
        }
    }
    else {
        PyErr_SetString(CreateException, "Could not duplicate alignment");
        return NULL;
    }

    ped_alignment_destroy(out_alignment);
    ped_alignment_destroy(align);

    return (PyObject *) ret;
}

PyObject *py_ped_alignment_intersect(PyObject *s, PyObject *args) {
    PyObject *in_a, *in_b;
    PedAlignment *out_a, *out_b, *align;
    _ped_Alignment *ret;

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_Alignment_Type_obj, &in_a,
                          &_ped_Alignment_Type_obj, &in_b)) {
        return NULL;
    }

    out_a = _ped_Alignment2PedAlignment(in_a);
    if (out_a == NULL) {
        return NULL;
    }

    out_b = _ped_Alignment2PedAlignment(in_b);
    if (out_b == NULL) {
        return NULL;
    }

    align = ped_alignment_intersect(out_a, out_b);
    if (align) {
        ret = PedAlignment2_ped_Alignment(align);
        if (ret == NULL) {
            return NULL;
        }
    }
    else {
        PyErr_SetString(PyExc_ArithmeticError, "Could not find alignment intersection");
        return NULL;
    }

    ped_alignment_destroy(out_a);
    ped_alignment_destroy(out_b);
    ped_alignment_destroy(align);

    return (PyObject *) ret;
}

PyObject *py_ped_alignment_align_up(PyObject *s, PyObject *args) {
    PyObject *in_align, *in_geom;
    PedAlignment *out_align;
    PedGeometry *out_geom;
    PedSector sector, ret;

    if (!PyArg_ParseTuple(args, "O!O!l", &_ped_Alignment_Type_obj, &in_align,
                          &_ped_Geometry_Type_obj, &in_geom, &sector)) {
        return NULL;
    }

    out_align = _ped_Alignment2PedAlignment(in_align);
    if (out_align == NULL) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    ret = ped_alignment_align_up(out_align, out_geom, sector);
    if (ret == -1) {
        PyErr_SetString(PyExc_ArithmeticError, "Could not align up to sector");
        return NULL;
    }

    ped_alignment_destroy(out_align);
    ped_geometry_destroy(out_geom);

    return PyLong_FromLongLong(ret);
}

PyObject *py_ped_alignment_align_down(PyObject *s, PyObject *args) {
    PyObject *in_align, *in_geom;
    PedAlignment *out_align;
    PedGeometry *out_geom;
    PedSector sector, ret;

    if (!PyArg_ParseTuple(args, "O!O!l", &_ped_Alignment_Type_obj, &in_align,
                          &_ped_Geometry_Type_obj, &in_geom, &sector)) {
        return NULL;
    }

    out_align = _ped_Alignment2PedAlignment(in_align);
    if (out_align == NULL) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    ret = ped_alignment_align_down(out_align, out_geom, sector);
    if (ret == -1) {
        PyErr_SetString(PyExc_ArithmeticError, "Could not align down to sector");
        return NULL;
    }

    ped_alignment_destroy(out_align);
    ped_geometry_destroy(out_geom);

    return PyLong_FromLongLong(ret);
}

PyObject *py_ped_alignment_align_nearest(PyObject *s, PyObject *args) {
    PyObject *in_align, *in_geom;
    PedAlignment *out_align;
    PedGeometry *out_geom;
    PedSector sector, ret;

    if (!PyArg_ParseTuple(args, "O!O!l", &_ped_Alignment_Type_obj, &in_align,
                          &_ped_Geometry_Type_obj, &in_geom, &sector)) {
        return NULL;
    }

    out_align = _ped_Alignment2PedAlignment(in_align);
    if (out_align == NULL) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    ret = ped_alignment_align_nearest(out_align, out_geom, sector);
    if (ret == -1) {
        PyErr_SetString(PyExc_ArithmeticError, "Could not align to closest sector");
        return NULL;
    }

    ped_alignment_destroy(out_align);
    ped_geometry_destroy(out_geom);

    return PyLong_FromLongLong(ret);
}

PyObject *py_ped_alignment_is_aligned(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_align, *in_geom;
    PedAlignment *out_align;
    PedGeometry *out_geom;
    PedSector sector;

    if (!PyArg_ParseTuple(args, "O!O!l", &_ped_Alignment_Type_obj, &in_align,
                          &_ped_Geometry_Type_obj, &in_geom, &sector)) {
        return NULL;
    }

    out_align = _ped_Alignment2PedAlignment(in_align);
    if (out_align == NULL) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    ret = ped_alignment_is_aligned(out_align, out_geom, sector);
    ped_alignment_destroy(out_align);
    ped_geometry_destroy(out_geom);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_div_round_up(PyObject *s, PyObject *args) {
    PedSector numerator, divisor;

    if (!PyArg_ParseTuple(args, "ll", &numerator, &divisor)) {
        return NULL;
    }

    return PyLong_FromLongLong(ped_div_round_up(numerator, divisor));
}

PyObject *py_ped_div_round_to_nearest(PyObject *s, PyObject *args) {
    PedSector numerator, divisor;

    if (!PyArg_ParseTuple(args, "ll", &numerator, &divisor)) {
        return NULL;
    }

    return PyLong_FromLongLong(ped_div_round_to_nearest(numerator, divisor));
}
