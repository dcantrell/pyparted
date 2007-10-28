/*
 * pynatmath.h
 * pyparted type definitions for pynatmath.c
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
 * Red Hat Author(s): David Cantrell <dcantrell@redhat.com>
 */

#ifndef PYNATMATH_H_INCLUDED
#define PYNATMATH_H_INCLUDED

#include <Python.h>
#include <structmember.h>

#include <parted/parted.h>

/* _ped.Alignment type is the Python equivalent of PedAlignment in libparted */
typedef struct {
    PyObject_HEAD

    /* PedAlignment members */
    PyObject *offset;              /* _ped.Sector() */
    PyObject *grain_size;          /* _ped.Sector() */
} _ped_Alignment;

static PyMemberDef _ped_Alignment_members[] = {
    {"offset", T_OBJECT, offsetof(_ped_Alignment, offset), 0, NULL},
    {"grain_size", T_OBJECT, offsetof(_ped_Alignment, grain_size), 0, NULL},
    {NULL}
};

static PyMethodDef _ped_Alignment_methods[] = {
    {NULL}
};

void _ped_Alignment_dealloc(_ped_Alignment *self);
PyObject *_ped_Alignment_new(PyTypeObject *type, PyObject *args,
                             PyObject *kwds);
int _ped_Alignment_init(_ped_Alignment *self, PyObject *args, PyObject *kwds);

static PyGetSetDef _ped_Alignment_getset[] = {
    {NULL}  /* Sentinel */
};

static PyTypeObject _ped_Alignment_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    0,                                         /* ob_size */
    "_ped.Alignment",                          /* tp_name */
    sizeof(_ped_Alignment),                    /* tp_basicsize */
    0,                                         /* tp_itemsize */
    (destructor) _ped_Alignment_dealloc,       /* tp_dealloc */
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
    "PedAlignment objects",                    /* tp_doc */
    0,                                         /* tp_traverse */
    0,                                         /* tp_clear */
    0,                                         /* tp_richcompare */
    0,                                         /* tp_weaklistoffset */
    0,                                         /* tp_iter */
    0,                                         /* tp_iternext */
    _ped_Alignment_methods,                    /* tp_methods */
    _ped_Alignment_members,                    /* tp_members */
    _ped_Alignment_getset,                     /* tp_getset */
    0,                                         /* tp_base */
    0,                                         /* tp_dict */
    0,                                         /* tp_descr_get */
    0,                                         /* tp_descr_set */
    0,                                         /* tp_dictoffset */
    (initproc) _ped_Alignment_init,            /* tp_init */
    0,                                         /* tp_alloc */
    _ped_Alignment_new,                        /* tp_new */
};

/* 1:1 function mappings for natmath.h in libparted */
PyObject *py_ped_round_up_to(PyObject *s, PyObject *args);
PyObject *py_ped_round_down_to(PyObject *s, PyObject *args);
PyObject *py_ped_round_to_nearest(PyObject *s, PyObject *args);
PyObject *py_ped_greatest_common_divisor(PyObject *s, PyObject *args);
PyObject *py_ped_alignment_init(PyObject *s, PyObject *args);
PyObject *py_ped_alignment_new(PyObject *s, PyObject *args);
PyObject *py_ped_alignment_destroy(PyObject *s, PyObject *args);
PyObject *py_ped_alignment_duplicate(PyObject *s, PyObject *args);
PyObject *py_ped_alignment_intersect(PyObject *s, PyObject *args);
PyObject *py_ped_alignment_align_up(PyObject *s, PyObject *args);
PyObject *py_ped_alignment_align_down(PyObject *s, PyObject *args);
PyObject *py_ped_alignment_align_nearest(PyObject *s, PyObject *args);
PyObject *py_ped_alignment_is_aligned(PyObject *s, PyObject *args);
PyObject *py_ped_div_round_up(PyObject *s, PyObject *args);
PyObject *py_ped_div_round_to_nearest(PyObject *s, PyObject *args);

#endif /* PYNATMATH_H_INCLUDED */
