/*
 * pytimer.c
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
#include "pytimer.h"

/* _ped.Timer functions */
void _ped_Timer_dealloc(_ped_Timer *self) {
    PyObject_Del(self);
}

PyObject *_ped_Timer_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    _ped_Timer *self;

    self = PyObject_New(_ped_Timer, &_ped_Timer_Type_obj);
    return (PyObject *) self;
}

int _ped_Timer_init(_ped_Timer *self, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"frac", "start", "now", "predicted_end",
                             "state_name", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|fddds", kwlist,
                                     &self->frac, &self->start, &self->now,
                                     &self->predicted_end, &self->state_name))
        return -1;
    else
        return 0;
}

PyObject *_ped_Timer_get(_ped_Timer *self, char *member) {
    if (member == NULL) {
        return NULL;
    }

    if (!strcmp(member, "frac")) {
        return Py_BuildValue("f", self->frac);
    } else if (!strcmp(member, "start")) {
        return Py_BuildValue("d", self->start);
    } else if (!strcmp(member, "now")) {
        return Py_BuildValue("d", self->now);
    } else if (!strcmp(member, "predicted_end")) {
        return Py_BuildValue("d", self->predicted_end);
    } else if (!strcmp(member, "state_name")) {
        return Py_BuildValue("s", self->state_name);
    } else {
        return NULL;
    }
}

/* 1:1 function mappings for timer.h in libparted */
PyObject *py_ped_timer_new(PyObject *s, PyObject *args) {
    /* XXX */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return NULL;
}

PyObject *py_ped_timer_destroy(PyObject *s, PyObject *args) {
    PyObject *in_timer;
    PedTimer *out_timer;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Timer_Type_obj, &in_timer)) {
        return NULL;
    }

    out_timer = _ped_Timer2PedTimer(in_timer);
    if (out_timer == NULL) {
        return NULL;
    }

    ped_timer_destroy(out_timer);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_timer_new_nested(PyObject *s, PyObject *args) {
    PyObject *in_parent;
    float nest_frac;
    PedTimer *out_parent, *timer;
    _ped_Timer *ret;

    if (!PyArg_ParseTuple(args, "O!f", &_ped_Timer_Type_obj, &in_parent,
                          &nest_frac))
        return NULL;

    out_parent = _ped_Timer2PedTimer(in_parent);
    if (out_parent == NULL) {
        return NULL;
    }

    timer = ped_timer_new_nested(out_parent, nest_frac);
    if (timer) {
        ret = PedTimer2_ped_Timer(timer);
        if (ret == NULL) {
            return NULL;
        }
    }
    else {
        PyErr_SetString(TimerException, "Could not create new nested timer");
        return NULL;
    }

    ped_timer_destroy(out_parent);
    ped_timer_destroy(timer);

    return (PyObject *) ret;
}

PyObject *py_ped_timer_destroy_nested(PyObject *s, PyObject *args) {
    PyObject *in_timer;
    PedTimer *out_timer;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Timer_Type_obj, &in_timer)) {
        return NULL;
    }

    out_timer = _ped_Timer2PedTimer(in_timer);
    if (out_timer == NULL) {
        return NULL;
    }

    ped_timer_destroy_nested(out_timer);
    ped_timer_destroy(out_timer);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_timer_touch(PyObject *s, PyObject *args) {
    PyObject *in_timer;
    PedTimer *out_timer;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Timer_Type_obj, &in_timer)) {
        return NULL;
    }

    out_timer = _ped_Timer2PedTimer(in_timer);
    if (out_timer == NULL) {
        return NULL;
    }

    ped_timer_touch(out_timer);
    ped_timer_destroy(out_timer);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_timer_reset(PyObject *s, PyObject *args) {
    PyObject *in_timer;
    PedTimer *out_timer;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Timer_Type_obj, &in_timer)) {
        return NULL;
    }

    out_timer = _ped_Timer2PedTimer(in_timer);
    if (out_timer == NULL) {
        return NULL;
    }

    ped_timer_reset(out_timer);
    ped_timer_destroy(out_timer);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_timer_update(PyObject *s, PyObject *args) {
    PyObject *in_timer;
    float frac;
    PedTimer *out_timer;

    if (!PyArg_ParseTuple(args, "O!f", &_ped_Timer_Type_obj, &in_timer, &frac))
        return NULL;

    out_timer = _ped_Timer2PedTimer(in_timer);
    if (out_timer == NULL) {
        return NULL;
    }

    ped_timer_update(out_timer, frac);
    ped_timer_destroy(out_timer);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_timer_set_state_name(PyObject *s, PyObject *args) {
    PyObject *in_timer;
    char *str;
    PedTimer *out_timer;

    if (!PyArg_ParseTuple(args, "O!s", &_ped_Timer_Type_obj, &in_timer, &str)) {
        return NULL;
    }

    out_timer = _ped_Timer2PedTimer(in_timer);
    if (out_timer == NULL) {
        return NULL;
    }

    ped_timer_set_state_name(out_timer, str);

    ped_timer_destroy(out_timer);
    free(str);

    Py_INCREF(Py_None);
    return Py_None;
}
