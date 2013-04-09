/*
 * pynatmath.h
 * pyparted type definitions for pynatmath.c
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

#ifndef PYNATMATH_H_INCLUDED
#define PYNATMATH_H_INCLUDED

#include <Python.h>

#include <parted/parted.h>

/* 1:1 function mappings for natmath.h in libparted */
PyObject *py_ped_alignment_duplicate(PyObject *, PyObject *);
PyObject *py_ped_alignment_intersect(PyObject *, PyObject *);
PyObject *py_ped_alignment_align_up(PyObject *, PyObject *);
PyObject *py_ped_alignment_align_down(PyObject *, PyObject *);
PyObject *py_ped_alignment_align_nearest(PyObject *, PyObject *);
PyObject *py_ped_alignment_is_aligned(PyObject *, PyObject *);

/* _ped.Alignment type is the Python equivalent of PedAlignment in libparted */
typedef struct {
    PyObject_HEAD

    /* PedAlignment members */
    long long offset;              /* PedSector */
    long long grain_size;          /* PedSector */
} _ped_Alignment;

void _ped_Alignment_dealloc(_ped_Alignment *);
int _ped_Alignment_compare(_ped_Alignment *, PyObject *);
PyObject *_ped_Alignment_richcompare(_ped_Alignment *, PyObject *, int);
PyObject *_ped_Alignment_str(_ped_Alignment *);
int _ped_Alignment_traverse(_ped_Alignment *, visitproc, void *);
int _ped_Alignment_clear(_ped_Alignment *);
int _ped_Alignment_init(_ped_Alignment *, PyObject *, PyObject *);
PyObject *_ped_Alignment_get(_ped_Alignment *, void *);
int _ped_Alignment_set(_ped_Alignment *, PyObject *, void *);

extern PyTypeObject _ped_Alignment_Type_obj;

#endif /* PYNATMATH_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4
 */
