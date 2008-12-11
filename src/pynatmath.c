/*
 * pynatmath.c
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
#include "pydevice.h"
#include "pynatmath.h"
#include "docstrings/pynatmath.h"

/* _ped.Alignment type object */
static PyMemberDef _ped_Alignment_members[] = {
    {NULL}
};

static PyMethodDef _ped_Alignment_methods[] = {
    {"destroy", (PyCFunction) py_ped_alignment_destroy, METH_VARARGS,
                 alignment_destroy_doc},
    {"duplicate", (PyCFunction) py_ped_alignment_duplicate, METH_VARARGS,
                  alignment_duplicate_doc},
    {"intersect", (PyCFunction) py_ped_alignment_intersect, METH_VARARGS,
                  alignment_intersect_doc},
    {"align_up", (PyCFunction) py_ped_alignment_align_up, METH_VARARGS,
                 alignment_align_up_doc},
    {"align_down", (PyCFunction) py_ped_alignment_align_down,
                   METH_VARARGS, alignment_align_down_doc},
    {"align_nearest", (PyCFunction) py_ped_alignment_align_nearest,
                      METH_VARARGS, alignment_align_nearest_doc},
    {"is_aligned", (PyCFunction) py_ped_alignment_is_aligned,
                   METH_VARARGS, alignment_is_aligned_doc},
    {NULL}
};

static PyGetSetDef _ped_Alignment_getset[] = {
    {"offset", (getter) _ped_Alignment_get,
               (setter) _ped_Alignment_set,
               "Offset in sectors from the start of a _ped.Geometry.", "offset"},
    {"grain_size", (getter) _ped_Alignment_get,
                   (setter) _ped_Alignment_set,
                   "Alignment grain_size", "grain_size"},
    {NULL}  /* Sentinel */
};

PyTypeObject _ped_Alignment_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    .tp_name = "_ped.Alignment",
    .tp_basicsize = sizeof(_ped_Alignment),
 /* .tp_itemsize = XXX */
    .tp_dealloc = (destructor) _ped_Alignment_dealloc,
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
    .tp_doc = _ped_Alignment_doc,
    .tp_traverse = (traverseproc) _ped_Alignment_traverse,
    .tp_clear = (inquiry) _ped_Alignment_clear,
 /* .tp_richcompare = XXX */
 /* .tp_weaklistoffset = XXX */
 /* .tp_iter = XXX */
 /* .tp_iternext = XXX */
    .tp_methods = _ped_Alignment_methods,
    .tp_members = _ped_Alignment_members,
    .tp_getset = _ped_Alignment_getset,
    .tp_base = NULL,
 /* .tp_dict = XXX */
 /* .tp_descr_get = XXX */
 /* .tp_descr_set = XXX */
 /* .tp_dictoffset = XXX */
    .tp_init = (initproc) _ped_Alignment_init,
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

/* _ped.Alignment functions */
void _ped_Alignment_dealloc(_ped_Alignment *self) {
    PyObject_GC_UnTrack(self);
    PyObject_GC_Del(self);
}

int _ped_Alignment_traverse(_ped_Alignment *self, visitproc visit, void *arg) {
    return 0;
}

int _ped_Alignment_clear(_ped_Alignment *self) {
    return 0;
}

int _ped_Alignment_init(_ped_Alignment *self, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"offset", "grain_size", NULL};
    PedAlignment *alignment = NULL;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "ll", kwlist,
                                     &self->offset, &self->grain_size)) {
        return -1;
    } else {
        alignment = ped_alignment_new(self->offset, self->grain_size);
        if (!alignment) {
            PyErr_SetString(CreateException, "Could not create new alignment");
            return -1;
        }

        ped_alignment_destroy(alignment);
        return 0;
    }
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
        return -1;
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

    if (grain_size == 0) {
       PyErr_SetNone(PyExc_ZeroDivisionError);
       return NULL;
    }

    return PyLong_FromLongLong(ped_round_up_to(sector, grain_size));
}

PyObject *py_ped_round_down_to(PyObject *s, PyObject *args) {
    PedSector sector, grain_size;

    if (!PyArg_ParseTuple(args, "ll", &sector, &grain_size)) {
        return NULL;
    }

    if (grain_size == 0) {
       PyErr_SetNone(PyExc_ZeroDivisionError);
       return NULL;
    }

    return PyLong_FromLongLong(ped_round_down_to(sector, grain_size));
}

PyObject *py_ped_round_to_nearest(PyObject *s, PyObject *args) {
    PedSector sector, grain_size;

    if (!PyArg_ParseTuple(args, "ll", &sector, &grain_size)) {
        return NULL;
    }

    if (grain_size == 0) {
       PyErr_SetNone(PyExc_ZeroDivisionError);
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

PyObject *py_ped_alignment_destroy(PyObject *s, PyObject *args) {
    PyObject_GC_Del(s);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_alignment_duplicate(PyObject *s, PyObject *args) {
    PedAlignment *alignment = NULL, *align = NULL;
    _ped_Alignment *ret = NULL;

    alignment = _ped_Alignment2PedAlignment(s);
    if (alignment == NULL) {
        return NULL;
    }

    align = ped_alignment_duplicate(alignment);
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

    ped_alignment_destroy(alignment);
    ped_alignment_destroy(align);

    return (PyObject *) ret;
}

PyObject *py_ped_alignment_intersect(PyObject *s, PyObject *args) {
    PyObject *in_b = NULL;
    PedAlignment *out_a = NULL, *out_b = NULL, *align = NULL;
    _ped_Alignment *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Alignment_Type_obj, &in_b)) {
        return NULL;
    }

    out_a = _ped_Alignment2PedAlignment(s);
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
    PyObject *in_geom = NULL;
    PedAlignment *align = NULL;
    PedGeometry *out_geom = NULL;
    PedSector sector, ret;

    if (!PyArg_ParseTuple(args, "O!l", &_ped_Geometry_Type_obj, &in_geom, &sector)) {
        return NULL;
    }

    align = _ped_Alignment2PedAlignment(s);
    if (align == NULL) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    ret = ped_alignment_align_up(align, out_geom, sector);
    if (ret == -1) {
        PyErr_SetString(PyExc_ArithmeticError, "Could not align up to sector");
        return NULL;
    }

    ped_alignment_destroy(align);
    ped_geometry_destroy(out_geom);

    return PyLong_FromLongLong(ret);
}

PyObject *py_ped_alignment_align_down(PyObject *s, PyObject *args) {
    PyObject *in_geom = NULL;
    PedAlignment *align = NULL;
    PedGeometry *out_geom = NULL;
    PedSector sector, ret;

    if (!PyArg_ParseTuple(args, "O!l", &_ped_Geometry_Type_obj, &in_geom, &sector)) {
        return NULL;
    }

    align = _ped_Alignment2PedAlignment(s);
    if (align == NULL) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    ret = ped_alignment_align_down(align, out_geom, sector);
    if (ret == -1) {
        PyErr_SetString(PyExc_ArithmeticError, "Could not align down to sector");
        return NULL;
    }

    ped_alignment_destroy(align);
    ped_geometry_destroy(out_geom);

    return PyLong_FromLongLong(ret);
}

PyObject *py_ped_alignment_align_nearest(PyObject *s, PyObject *args) {
    PyObject *in_geom = NULL;
    PedAlignment *align = NULL;
    PedGeometry *out_geom = NULL;
    PedSector sector, ret;

    if (!PyArg_ParseTuple(args, "O!l", &_ped_Geometry_Type_obj, &in_geom, &sector)) {
        return NULL;
    }

    align = _ped_Alignment2PedAlignment(s);
    if (align == NULL) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    ret = ped_alignment_align_nearest(align, out_geom, sector);
    if (ret == -1) {
        PyErr_SetString(PyExc_ArithmeticError, "Could not align to closest sector");
        return NULL;
    }

    ped_alignment_destroy(align);
    ped_geometry_destroy(out_geom);

    return PyLong_FromLongLong(ret);
}

PyObject *py_ped_alignment_is_aligned(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_geom = NULL;
    PedAlignment *align = NULL;
    PedGeometry *out_geom = NULL;
    PedSector sector;

    if (!PyArg_ParseTuple(args, "O!l", &_ped_Geometry_Type_obj, &in_geom, &sector)) {
        return NULL;
    }

    align = _ped_Alignment2PedAlignment(s);
    if (align == NULL) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    ret = ped_alignment_is_aligned(align, out_geom, sector);
    ped_alignment_destroy(align);
    ped_geometry_destroy(out_geom);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_div_round_up(PyObject *s, PyObject *args) {
    PedSector numerator, divisor;

    if (!PyArg_ParseTuple(args, "ll", &numerator, &divisor)) {
        return NULL;
    }

    if (divisor == 0) {
       PyErr_SetNone(PyExc_ZeroDivisionError);
       return NULL;
    }

    return PyLong_FromLongLong(ped_div_round_up(numerator, divisor));
}

PyObject *py_ped_div_round_to_nearest(PyObject *s, PyObject *args) {
    PedSector numerator, divisor;

    if (!PyArg_ParseTuple(args, "ll", &numerator, &divisor)) {
        return NULL;
    }

    if (divisor == 0) {
       PyErr_SetNone(PyExc_ZeroDivisionError);
       return NULL;
    }

    return PyLong_FromLongLong(ped_div_round_to_nearest(numerator, divisor));
}

/* vim:tw=78:ts=4:et:sw=4
 */
