/*
 * pyunit.h
 * pyparted type definitions for pyunit.c
 *
 * Copyright The pyparted Project Authors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef PYUNIT_H_INCLUDED
#define PYUNIT_H_INCLUDED

#include <Python.h>

#include <parted/parted.h>

/* a PedUnit is a long int in C, so we store it that way in Python */

/* 1:1 function mappings for unit.h in libparted */
PyObject *py_ped_unit_set_default(PyObject *, PyObject *);
PyObject *py_ped_unit_get_default(PyObject *, PyObject *);
PyObject *py_ped_unit_get_name(PyObject *, PyObject *);
PyObject *py_ped_unit_get_by_name(PyObject *, PyObject *);

#endif /* PYUNIT_H_INCLUDED */
