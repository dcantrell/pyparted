/*
 * pytimer.h
 * pyparted type objects for pytimer.c
 *
 * Copyright The pyparted Project Authors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef TYPEOBJECTS_PYTIMER_H_INCLUDED
#define TYPEOBJECTS_PYTIMER_H_INCLUDED

#include <Python.h>
#include <structmember.h>

/* _ped.Timer type object */
static PyMemberDef _ped_Timer_members[] = {
    {NULL}
};

static PyMethodDef _ped_Timer_methods[] = {
    {"destroy", (PyCFunction) py_ped_timer_destroy, METH_VARARGS, NULL},
    {"new_nested", (PyCFunction) py_ped_timer_new_nested, METH_VARARGS, NULL},
    {"destroy_nested", (PyCFunction) py_ped_timer_destroy_nested,
                       METH_VARARGS, NULL},
    {"touch", (PyCFunction) py_ped_timer_touch, METH_VARARGS, NULL},
    {"reset", (PyCFunction) py_ped_timer_reset, METH_VARARGS, NULL},
    {"update", (PyCFunction) py_ped_timer_update, METH_VARARGS, NULL},
    {"set_state_name", (PyCFunction) py_ped_timer_set_state_name,
                       METH_VARARGS, NULL},
    {NULL}
};

static PyGetSetDef _ped_Timer_getset[] = {
    {"frac", (getter) _ped_Timer_get, (setter) _ped_Timer_set,
             "PedTimer frac", "frac"},
    {"start", (getter) _ped_Timer_get, (setter) _ped_Timer_set,
              "PedTimer.start", "start"},
    {"now", (getter) _ped_Timer_get, (setter) _ped_Timer_set,
            "PedTimer.now", "now"},
    {"predicted_end", (getter) _ped_Timer_get, (setter) _ped_Timer_set,
                      "PedTimer.predicted_end", "predicted_end"},
    {"state_name", (getter) _ped_Timer_get, (setter) _ped_Timer_set,
                   "PedTimer.state_name", "state_name"},
    {NULL}  /* Sentinel */
};

PyTypeObject _ped_Timer_Type_obj = {
    PyVarObject_HEAD_INIT(&PyType_Type,0)
    .tp_name = "_ped.Timer",
    .tp_basicsize = sizeof(_ped_Timer),
 /* .tp_itemsize = XXX */
    .tp_dealloc = (destructor) _ped_Timer_dealloc,
 /* .tp_getattr = XXX */
 /* .tp_setattr = XXX */
 /* .tp_repr = XXX */
 /* .tp_as_number = XXX */
 /* .tp_as_sequence = XXX */
 /* .tp_as_mapping = XXX */
    .tp_hash = PyObject_HashNotImplemented,
    .tp_call = NULL,
    .tp_str = (reprfunc) _ped_Timer_str,
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_setattro = PyObject_GenericSetAttr,
 /* .tp_as_buffer = XXX */
    .tp_flags = TP_FLAGS,
    .tp_doc = "PedTimer objects",
    .tp_traverse = (traverseproc) _ped_Timer_traverse,
    .tp_clear = (inquiry) _ped_Timer_clear,
    .tp_richcompare = (richcmpfunc) _ped_Timer_richcompare,
 /* .tp_weaklistoffset = XXX */
 /* .tp_iter = XXX */
 /* .tp_iternext = XXX */
    .tp_methods = _ped_Timer_methods,
    .tp_members = _ped_Timer_members,
    .tp_getset = _ped_Timer_getset,
    .tp_base = NULL,
    .tp_dict = NULL,
 /* .tp_descr_get = XXX */
 /* .tp_descr_set = XXX */
 /* .tp_dictoffset = XXX */
    .tp_init = (initproc) _ped_Timer_init,
    .tp_alloc = PyType_GenericAlloc,
    .tp_new = PyType_GenericNew,
 /* .tp_free = XXX */
 /* .tp_is_gc = XXX */
    .tp_bases = NULL,
 /* .tp_del = XXX */
};

#endif /* TYPEOBJECTS_PYTIMER_H_INCLUDED */
