/*
 * pynatmath.h
 * pyparted type definitions for pynatmath.c
 *
 * Copyright The pyparted Project Authors
 * SPDX-License-Identifier: GPL-2.0-or-later
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
