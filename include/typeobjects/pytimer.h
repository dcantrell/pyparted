/*
 * pytimer.h
 * pyparted type objects for pytimer.c
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

/* vim:tw=78:ts=4:et:sw=4
 */
