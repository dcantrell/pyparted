/*
 * pynatmath.h
 * pyparted type definitions for pynatmath.c
 *
 * Copyright (C) 2007, 2008 Red Hat, Inc.
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
 *                    Chris Lumens <clumens@redhat.com>
 */

#ifndef PYNATMATH_H_INCLUDED
#define PYNATMATH_H_INCLUDED

#include <Python.h>
#include <structmember.h>

#include <parted/parted.h>

/* 1:1 function mappings for natmath.h in libparted */
PyObject *py_ped_round_up_to(PyObject *s, PyObject *args);
PyObject *py_ped_round_down_to(PyObject *s, PyObject *args);
PyObject *py_ped_round_to_nearest(PyObject *s, PyObject *args);
PyObject *py_ped_greatest_common_divisor(PyObject *s, PyObject *args);
PyObject *py_ped_alignment_destroy(PyObject *s, PyObject *args);
PyObject *py_ped_alignment_duplicate(PyObject *s, PyObject *args);
PyObject *py_ped_alignment_intersect(PyObject *s, PyObject *args);
PyObject *py_ped_alignment_align_up(PyObject *s, PyObject *args);
PyObject *py_ped_alignment_align_down(PyObject *s, PyObject *args);
PyObject *py_ped_alignment_align_nearest(PyObject *s, PyObject *args);
PyObject *py_ped_alignment_is_aligned(PyObject *s, PyObject *args);
PyObject *py_ped_div_round_up(PyObject *s, PyObject *args);
PyObject *py_ped_div_round_to_nearest(PyObject *s, PyObject *args);

/* _ped.Alignment type is the Python equivalent of PedAlignment in libparted */
typedef struct {
    PyObject_HEAD

    /* PedAlignment members */
    long long offset;              /* PedSector */
    long long grain_size;          /* PedSector */
} _ped_Alignment;

void _ped_Alignment_dealloc(_ped_Alignment *self);
int _ped_Alignment_traverse(_ped_Alignment *self, visitproc visit, void *arg);
int _ped_Alignment_clear(_ped_Alignment *self);
int _ped_Alignment_init(_ped_Alignment *self, PyObject *args, PyObject *kwds);
PyObject *_ped_Alignment_get(_ped_Alignment *self, void *closure);
int _ped_Alignment_set(_ped_Alignment *self, PyObject *value, void *closure);

extern PyTypeObject _ped_Alignment_Type_obj;

#endif /* PYNATMATH_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4
 */
