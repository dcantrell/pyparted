/*
 * pytimer.c
 *
 * Copyright The pyparted Project Authors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <Python.h>

#include "convert.h"
#include "exceptions.h"
#include "pytimer.h"
#include "typeobjects/pytimer.h"

/* _ped.Timer functions */
void _ped_Timer_dealloc(_ped_Timer *self)
{
    PyObject_GC_UnTrack(self);
    free(self->state_name);
    PyObject_GC_Del(self);
}

int _ped_Timer_compare(_ped_Timer *self, PyObject *obj)
{
    _ped_Timer *comp = NULL;
    int check = PyObject_IsInstance(obj, (PyObject *) &_ped_Timer_Type_obj);

    if (PyErr_Occurred()) {
        return -1;
    }

    if (!check) {
        PyErr_SetString(PyExc_ValueError, "object comparing to must be a _ped.Timer");
        return -1;
    }

    comp = (_ped_Timer *) obj;

    if ((self->frac == comp->frac) &&
        (self->start == comp->start) &&
        (self->now == comp->now) &&
        (self->predicted_end == comp->predicted_end) &&
        (!strcmp(self->state_name, comp->state_name)) &&
        (self->handler == comp->handler) &&
        (self->context == comp->context)) {
        return 0;
    } else {
        return 1;
    }
}

PyObject *_ped_Timer_richcompare(_ped_Timer *a, PyObject *b, int op)
{
    int rv;

    if (op == Py_EQ || op == Py_NE) {
        rv = _ped_Timer_compare(a, b);

        if (PyErr_Occurred()) {
            return NULL;
        }

        return PyBool_FromLong(op == Py_EQ ? rv == 0 : rv != 0);
    } else if ((op == Py_LT) || (op == Py_LE) || (op == Py_GT) || (op == Py_GE)) {
        PyErr_SetString(PyExc_TypeError, "comparison operator not supported for _ped.Timer");
        return NULL;
    } else {
        PyErr_SetString(PyExc_ValueError, "unknown richcompare op");
        return NULL;
    }
}

PyObject *_ped_Timer_str(_ped_Timer *self)
{
    PyObject *ret = NULL;
    char *buf = NULL;

    if (asprintf(&buf, "_ped.Timer instance --\n"
                       "  start: %s  now:  %s\n"
                       "  predicted_end: %s  frac: %f\n"
                       "  state_name: %s",
                 ctime(&(self->start)), ctime(&(self->now)),
                 ctime(&(self->predicted_end)), self->frac,
                 self->state_name) == -1) {
        return PyErr_NoMemory();
    }

    ret = Py_BuildValue("s", buf);
    free(buf);
    return ret;
}

int _ped_Timer_traverse(_ped_Timer *self, visitproc visit, void *arg)
{
    return 0;
}

int _ped_Timer_clear(_ped_Timer *self)
{
    return 0;
}

int _ped_Timer_init(_ped_Timer *self, PyObject *args, PyObject *kwds)
{
    /* XXX: timers aren't really done yet in pyparted */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return -1;
}

PyObject *_ped_Timer_get(_ped_Timer *self, void *closure)
{
    char *member = (char *) closure;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Timer()");
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
        if (self->state_name != NULL) {
            return PyUnicode_FromString(self->state_name);
        } else {
            return PyUnicode_FromString("");
        }
    } else {
        PyErr_Format(PyExc_AttributeError, "_ped.Timer object has no attribute %s", member);
        return NULL;
    }
}

int _ped_Timer_set(_ped_Timer *self, PyObject *value, void *closure)
{
    char *member = (char *) closure;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Timer()");
        return -1;
    }

    if (!strcmp(member, "frac")) {
        if (!PyArg_ParseTuple(value, "f", &self->frac)) {
            return -1;
        }
    } else if (!strcmp(member, "start")) {
        self->start = PyFloat_AsDouble(value);

        if (PyErr_Occurred()) {
            return -1;
        }
    } else if (!strcmp(member, "now")) {
        self->now = PyFloat_AsDouble(value);

        if (PyErr_Occurred()) {
            return -1;
        }
    } else if (!strcmp(member, "predicted_end")) {
        self->predicted_end = PyFloat_AsDouble(value);

        if (PyErr_Occurred()) {
            return -1;
        }
    } else if (!strcmp(member, "state_name")) {
        self->state_name = (char *) PyUnicode_AsUTF8(value);

        if (PyErr_Occurred()) {
            return -1;
        }
        /* self->state_name now points to the internal buffer of a PyUnicode obj
         * which may be freed when its refcount drops to zero, so strdup it.
         */
        if (self->state_name) {
            self->state_name = strdup(self->state_name);

            if (!self->state_name) {
                PyErr_NoMemory();
                return -2;
            }
        }
    } else {
        PyErr_Format(PyExc_AttributeError, "_ped.Timer object has no attribute %s", member);
        return -1;
    }

    return 0;
}

/* 1:1 function mappings for timer.h in libparted */
PyObject *py_ped_timer_destroy(PyObject *s, PyObject *args)
{
    Py_CLEAR(s);

    Py_RETURN_NONE;
}

PyObject *py_ped_timer_new_nested(PyObject *s, PyObject *args)
{
    float nest_frac;
    PedTimer *parent = NULL, *timer = NULL;
    _ped_Timer *ret = NULL;

    if (!PyArg_ParseTuple(args, "f", &nest_frac)) {
        return NULL;
    }

    parent = _ped_Timer2PedTimer(s);

    if (parent == NULL) {
        return NULL;
    }

    timer = ped_timer_new_nested(parent, nest_frac);
    ped_timer_destroy(parent);

    if (timer) {
        ret = PedTimer2_ped_Timer(timer);
    } else {
        PyErr_SetString(CreateException, "Could not create new nested timer");
        return NULL;
    }

    ped_timer_destroy(timer);
    return (PyObject *) ret;
}

PyObject *py_ped_timer_destroy_nested(PyObject *s, PyObject *args)
{
    PedTimer *timer = NULL;

    timer = _ped_Timer2PedTimer(s);

    if (timer == NULL) {
        return NULL;
    }

    ped_timer_destroy_nested(timer);
    ped_timer_destroy(timer);
    Py_CLEAR(s);

    Py_RETURN_NONE;
}

PyObject *py_ped_timer_touch(PyObject *s, PyObject *args)
{
    PedTimer *timer = NULL;

    timer = _ped_Timer2PedTimer(s);

    if (timer == NULL) {
        return NULL;
    }

    ped_timer_touch(timer);
    ped_timer_destroy(timer);

    Py_RETURN_NONE;
}

PyObject *py_ped_timer_reset(PyObject *s, PyObject *args)
{
    PedTimer *timer = NULL;

    timer = _ped_Timer2PedTimer(s);

    if (timer == NULL) {
        return NULL;
    }

    ped_timer_reset(timer);
    ped_timer_destroy(timer);

    Py_RETURN_NONE;
}

PyObject *py_ped_timer_update(PyObject *s, PyObject *args)
{
    float frac;
    PedTimer *timer = NULL;

    if (!PyArg_ParseTuple(args, "f", &frac)) {
        return NULL;
    }

    timer = _ped_Timer2PedTimer(s);

    if (timer == NULL) {
        return NULL;
    }

    ped_timer_update(timer, frac);
    ped_timer_destroy(timer);

    Py_RETURN_NONE;
}

PyObject *py_ped_timer_set_state_name(PyObject *s, PyObject *args)
{
    char *str = NULL;
    PedTimer *timer = NULL;

    if (!PyArg_ParseTuple(args, "z", &str)) {
        return NULL;
    }

    timer = _ped_Timer2PedTimer(s);

    if (timer == NULL) {
        return NULL;
    }

    ped_timer_set_state_name(timer, str);
    ped_timer_destroy(timer);
    free(str);

    Py_RETURN_NONE;
}
