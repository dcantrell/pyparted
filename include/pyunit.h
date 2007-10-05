/*
 * pyunit.h
 * pyparted type definitions for pyunit.c
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

#ifndef PYUNIT_H_INCLUDED
#define PYUNIT_H_INCLUDED

#include <Python.h>
#include <structmember.h>

#include <parted/parted.h>

/* _ped.Unit type is the Python equivalent of PedUnit in libparted */
typedef struct {
    PyObject_HEAD

    /* a PedUnit is an enumerated type, so we'll just store a long here */
    long val;
} _ped_Unit;

static PyMemberDef _ped_Unit_members[] = {
    {"val", T_LONG, offsetof(_ped_Unit, val), 0, "PedUnit value"},
    {NULL}
};

static PyMethodDef _ped_Unit_methods[] = {
    {NULL}
};

void _ped_Unit_dealloc(_ped_Unit *self);
PyObject *_ped_Unit_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
int _ped_Unit_init(_ped_Unit *self, PyObject *args, PyObject *kwds);
PyObject *_ped_Unit_getval(_ped_Unit *self, void *closure);
int _ped_Unit_setval(_ped_Unit *self, PyObject *value, void *closure);

static PyGetSetDef _ped_Unit_getset[] = {
    {"val", (getter) _ped_Unit_getval, (setter) _ped_Unit_setval,
     "PedUnit value", NULL},
    {NULL}  /* Sentinel */
};

static PyTypeObject _ped_UnitType = {
    PyObject_HEAD_INIT(&PyType_Type)
    0,                                         /* ob_size */
    "_ped.Unit",                               /* tp_name */
    sizeof(_ped_Unit),                         /* tp_basicsize */
    0,                                         /* tp_itemsize */
    (destructor) _ped_Unit_dealloc,            /* tp_dealloc */
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
    "PedUnit objects",                         /* tp_doc */
    0,                                         /* tp_traverse */
    0,                                         /* tp_clear */
    0,                                         /* tp_richcompare */
    0,                                         /* tp_weaklistoffset */
    0,                                         /* tp_iter */
    0,                                         /* tp_iternext */
    _ped_Unit_methods,                         /* tp_methods */
    _ped_Unit_members,                         /* tp_members */
    _ped_Unit_getset,                          /* tp_getset */
    0,                                         /* tp_base */
    0,                                         /* tp_dict */
    0,                                         /* tp_descr_get */
    0,                                         /* tp_descr_set */
    0,                                         /* tp_dictoffset */
    (initproc) _ped_Unit_init,                 /* tp_init */
    0,                                         /* tp_alloc */
    _ped_Unit_new,                             /* tp_new */
};

/* 1:1 function mappings for unit.h in libparted */
PyObject *py_ped_unit_set_default(PyObject *s, PyObject *args);
PyObject *py_ped_unit_get_default(PyObject *s, PyObject *args);
PyObject *py_ped_unit_get_size(PyObject *s, PyObject *args);
PyObject *py_ped_unit_get_name(PyObject *s, PyObject *args);
PyObject *py_ped_unit_get_by_name(PyObject *s, PyObject *args);
PyObject *py_ped_unit_format_custom_byte(PyObject *s, PyObject *args);
PyObject *py_ped_unit_format_byte(PyObject *s, PyObject *args);
PyObject *py_ped_unit_format_custom(PyObject *s, PyObject *args);
PyObject *py_ped_unit_format(PyObject *s, PyObject *args);
PyObject *py_ped_unit_parse(PyObject *s, PyObject *args);
PyObject *py_ped_unit_parse_custom(PyObject *s, PyObject *args);

#endif /* PYUNIT_H_INCLUDED */
