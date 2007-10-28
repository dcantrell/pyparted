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
 * Red Hat Author(s): David Cantrell <dcantrell@redhat.com>
 */

#include <Python.h>

#include "pydevice.h"

/* _ped.Sector functions */
void _ped_Sector_dealloc(_ped_Sector *self) {
    PyObject_Del(self);
}

PyObject *_ped_Sector_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    _ped_Sector *self;

    self = PyObject_New(_ped_Sector, &_ped_Sector_Type_obj);
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
        return -1;
    }

    self->val = val;
    return 0;
}

/* _ped.CHSGeometry functions */
void _ped_CHSGeometry_dealloc(_ped_CHSGeometry *self) {
    PyObject_Del(self);
}

PyObject *_ped_CHSGeometry_new(PyTypeObject *type, PyObject *args,
                               PyObject *kwds) {
    _ped_CHSGeometry *self;

    self = PyObject_New(_ped_CHSGeometry, &_ped_CHSGeometry_Type_obj);
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
        return -1;
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
        return -1;
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
        return -1;
    }

    self->sectors = val;
    return 0;
}

/* _ped.Device functions */
void _ped_Device_dealloc(_ped_Device *self) {
    PyObject_Del(self);
}

PyObject *_ped_Device_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    _ped_Device *self;

    self = PyObject_New(_ped_Device, &_ped_Device_Type_obj);
    return (PyObject *) self;
}

int _ped_Device_init(_ped_Device *self, PyObject *args, PyObject *kwds) {
    /* FIXME */
    return 0;
}

/* 1:1 function mappings for device.h in libparted */
PyObject *py_ped_device_probe_all(PyObject *s, PyObject *args)  {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_device_free_all(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_device_get(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_device_get_next(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_device_is_busy(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_device_open(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_device_close(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_device_destroy(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_device_cache_remove(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_device_begin_external_access(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_device_end_external_access(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_device_read(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_device_write(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_device_sync(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_device_sync_fast(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_device_check(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_device_get_constraint(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}
