/*
 * pytimer.h
 * pyparted type definitions for pytimer.c
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
 *            Alex Skinner <alex@lx.lc>
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
