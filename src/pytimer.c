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

#include <Python.h>

#include "pytimer.h"

/* _ped.Timer functions */
void _ped_Timer_dealloc(_ped_Timer *self) {
    self->ob_type->tp_free((PyObject *) self);
}

PyObject *_ped_Timer_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    _ped_Timer *self;

    self = (_ped_Timer *) type->tp_alloc(type, 0);
    return (PyObject *) self;
}

int _ped_Timer_init(_ped_Timer *self, PyObject *args, PyObject *kwds) {
    /* FIXME */
    return 0;
}

/* 1:1 function mappings for timer.h in libparted */
