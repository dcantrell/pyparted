/*
 * pytimer.h
 * pyparted type definitions for pytimer.c
 *
 * Copyright The pyparted Project Authors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef PYTIMER_H_INCLUDED
#define PYTIMER_H_INCLUDED

#include <Python.h>

#include <parted/parted.h>

/* 1:1 function mappings for timer.h in libparted */
PyObject *py_ped_timer_destroy(PyObject *, PyObject *);
PyObject *py_ped_timer_new_nested(PyObject *, PyObject *);
PyObject *py_ped_timer_destroy_nested(PyObject *, PyObject *);
PyObject *py_ped_timer_touch(PyObject *, PyObject *);
PyObject *py_ped_timer_reset(PyObject *, PyObject *);
PyObject *py_ped_timer_update(PyObject *, PyObject *);
PyObject *py_ped_timer_set_state_name(PyObject *, PyObject *);

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

void _ped_Timer_dealloc(_ped_Timer *);
int _ped_Timer_compare(_ped_Timer *, PyObject *);
PyObject *_ped_Timer_richcompare(_ped_Timer *, PyObject *, int);
PyObject *_ped_Timer_str(_ped_Timer *);
int _ped_Timer_traverse(_ped_Timer *, visitproc, void *);
int _ped_Timer_clear(_ped_Timer *);
int _ped_Timer_init(_ped_Timer *, PyObject *, PyObject *);
PyObject *_ped_Timer_get(_ped_Timer *, void *);
int _ped_Timer_set(_ped_Timer *, PyObject *, void *);

extern PyTypeObject _ped_Timer_Type_obj;

#endif /* PYTIMER_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4
 */
