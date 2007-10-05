/*
 * pytimer.h
 * pyparted type definitions for pytimer.c
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

#ifndef PYTIMER_H_INCLUDED
#define PYTIMER_H_INCLUDED

#include <Python.h>
#include <structmember.h>

#include <parted/parted.h>

/* _ped.Timer type is the Python equivalent of PedTimer in libparted */
typedef struct {
    PyObject_HEAD

    /* PedTimer members */
    float frac;
    time_t start;
    time_t now;
    time_t predicted_end;
    char *state_name;
    PedTimerHandler *handler;
    void *context;
} _ped_Timer;

static PyMemberDef _ped_Timer_members[] = {
    {"frac", T_FLOAT, offsetof(_ped_Timer, frac), 0, NULL},
    {"state_name", T_STRING_INPLACE, offsetof(_ped_Timer, state_name), 0, NULL},
    {NULL}
};

static PyMethodDef _ped_Timer_methods[] = {
    {NULL}
};

void _ped_Timer_dealloc(_ped_Timer *self);
PyObject *_ped_Timer_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
int _ped_Timer_init(_ped_Timer *self, PyObject *args, PyObject *kwds);

static PyGetSetDef _ped_Timer_getset[] = {
    {NULL}  /* Sentinel */
};

static PyTypeObject _ped_Timer_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    0,                                         /* ob_size */
    "_ped.Timer",                              /* tp_name */
    sizeof(_ped_Timer),                        /* tp_basicsize */
    0,                                         /* tp_itemsize */
    (destructor) _ped_Timer_dealloc,           /* tp_dealloc */
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
    "PedTimer objects",                        /* tp_doc */
    0,                                         /* tp_traverse */
    0,                                         /* tp_clear */
    0,                                         /* tp_richcompare */
    0,                                         /* tp_weaklistoffset */
    0,                                         /* tp_iter */
    0,                                         /* tp_iternext */
    _ped_Timer_methods,                        /* tp_methods */
    _ped_Timer_members,                        /* tp_members */
    _ped_Timer_getset,                         /* tp_getset */
    0,                                         /* tp_base */
    0,                                         /* tp_dict */
    0,                                         /* tp_descr_get */
    0,                                         /* tp_descr_set */
    0,                                         /* tp_dictoffset */
    (initproc) _ped_Timer_init,                /* tp_init */
    0,                                         /* tp_alloc */
    _ped_Timer_new,                            /* tp_new */
};

/* 1:1 function mappings for timer.h in libparted */

#endif /* PYTIMER_H_INCLUDED */
