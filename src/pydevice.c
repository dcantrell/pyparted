/*
 * pydevice.c
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

#include "pydevice.h"

/* _ped.Sector functions */
void _ped_Sector_dealloc(_ped_Sector *self) {
    self->ob_type->tp_free((PyObject *) self);
}

PyObject *_ped_Sector_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    _ped_Sector *self;

    self = (_ped_Sector *) type->tp_alloc(type, 0);
    return (PyObject *) self;
}

int _ped_Sector_init(_ped_Sector *self, PyObject *args, PyObject *kwds) {
    long long val;

    if (PyArg_ParseTuple(args, "L", &val)) {
        self->val = val;
    }

    return 0;
}

PyObject *_ped_Sector_getval(_ped_Sector *self, void *closure) {
    return Py_BuildValue("L", self->val);
}

int _ped_Sector_setval(_ped_Sector *self, PyObject *value, void *closure) {
    long long val;

    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the val attribute");
        return -1;
    }

    if (!PyArg_ParseTuple(value, "L", &val)) {
        PyErr_SetString(PyExc_TypeError, "The val attribute must be a long long");
        return NULL;
    }

    self->val = val;
    return 0;
}

/* _ped.CHSGeometry functions */
void _ped_CHSGeometry_dealloc(_ped_CHSGeometry *self) {
    self->ob_type->tp_free((PyObject *) self);
}

PyObject *_ped_CHSGeometry_new(PyTypeObject *type, PyObject *args,
                               PyObject *kwds) {
    _ped_CHSGeometry *self;

    self = (_ped_CHSGeometry *) type->tp_alloc(type, 0);
    return (PyObject *) self;
}

int _ped_CHSGeometry_init(_ped_CHSGeometry *self, PyObject *args,
                          PyObject *kwds) {
    /* FIXME - should handle keywords (cylinders=, heads=, sectors=) */
    return 0;
}

PyObject *_ped_CHSGeometry_getcylinders(_ped_CHSGeometry *self, void *closure) {
    return Py_BuildValue("i", self->cylinders);
}

int _ped_CHSGeometry_setcylinders(_ped_CHSGeometry *self, PyObject *value,
                                  void *closure) {
    int val;

    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the cylinders attribute");
        return -1;
    }

    if (!PyInt_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "The cylinders attribute must be an int");
        return -1;
    }

    if (!PyArg_ParseTuple(value, "i", &val)) {
        /* FIXME - this should throw an exception probably */
        return NULL;
    }

    self->cylinders = val;
    return 0;
}

PyObject *_ped_CHSGeometry_getheads(_ped_CHSGeometry *self, void *closure) {
    return Py_BuildValue("i", self->heads);
}

int _ped_CHSGeometry_setheads(_ped_CHSGeometry *self, PyObject *value,
                                  void *closure) {
    int val;

    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the heads attribute");
        return -1;
    }

    if (!PyInt_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "The heads attribute must be an int");
        return -1;
    }

    if (!PyArg_ParseTuple(value, "i", &val)) {
        /* FIXME - this should throw an exception probably */
        return NULL;
    }

    self->heads = val;
    return 0;
}

PyObject *_ped_CHSGeometry_getsectors(_ped_CHSGeometry *self, void *closure) {
    return Py_BuildValue("i", self->sectors);
}

int _ped_CHSGeometry_setsectors(_ped_CHSGeometry *self, PyObject *value,
                                  void *closure) {
    int val;

    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the sectors attribute");
        return -1;
    }

    if (!PyInt_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "The sectors attribute must be an int");
        return -1;
    }

    if (!PyArg_ParseTuple(value, "i", &val)) {
        /* FIXME - this should throw an exception probably */
        return NULL;
    }

    self->sectors = val;
    return 0;
}

/* 1:1 function mappings for device.h in libparted */
