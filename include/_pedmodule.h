/*
 * _pedmodule.h
 *
 * Copyright The pyparted Project Authors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef _PARTEDMODULE_H_INCLUDED
#define _PARTEDMODULE_H_INCLUDED

#include <Python.h>

extern PyObject *py_libparted_get_version(PyObject *, PyObject *);
extern PyObject *py_pyparted_version(PyObject *, PyObject *);
extern PyMODINIT_FUNC PyInit__ped(void);

#endif /* _PARTEDMODULE_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4
 */
