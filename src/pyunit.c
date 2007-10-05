/*
 * pyunit.c
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

#include "pyunit.h"

/* _ped.Unit functions */
void _ped_Unit_dealloc(_ped_Unit *self) {
    self->ob_type->tp_free((PyObject *) self);
}

PyObject *_ped_Unit_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    _ped_Unit *self;

    self = (_ped_Unit *) type->tp_alloc(type, 0);
    return (PyObject *) self;
}

int _ped_Unit_init(_ped_Unit *self, PyObject *args, PyObject *kwds) {
    long val;

    if (PyArg_ParseTuple(args, "l", &val)) {
        self->val = val;
    }

    return 0;
}

PyObject *_ped_Unit_getval(_ped_Unit *self, void *closure) {
    return Py_BuildValue("l", self->val);
}

int _ped_Unit_setval(_ped_Unit *self, PyObject *value, void *closure) {
    long val;

    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the val attribute");
        return -1;
    }

    if (!PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "The val attribute must be a long");
        return -1;
    }

    /* make sure the range is good */
    if (!PyArg_ParseTuple(value, "l", &val)) {
        /* FIXME - throw range exception */
        return -1;
    }

    self->val = val;
    return 0;
}

/* 1:1 function mappings for unit.h in libparted */
PyObject *py_ped_unit_set_default(PyObject *s, PyObject *args) {
    _ped_Unit *in;
    PedUnit out;

    if (!PyArg_ParseTuple(args, "O", (_ped_Unit *) &in)) {
        /* FIXME - throw exception */
        return NULL;
    }

    if (in->val < PED_UNIT_FIRST || in->val > PED_UNIT_TEBIBYTE) {
        /* FIXME - throw range exception */
        return NULL;
    }

    out = in->val;
    ped_unit_set_default(out);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_unit_get_default(PyObject *s, PyObject *args) {
    PedUnit in;
    _ped_Unit *out;

    in = ped_unit_get_default();
    out = PyObject_NEW(_ped_Unit, &_ped_UnitType);
    out->val = (long) in;

    Py_INCREF(out);
    return (PyObject *) out;
}

PyObject *py_ped_unit_get_size(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_unit_get_name(PyObject *s, PyObject *args) {
    _ped_Unit *in;
    PedUnit out;

    if (!PyArg_ParseTuple(args, "O", (_ped_Unit *) &in)) {
        /* FIXME - throw exception */
        return NULL;
    }

    if (in->val < PED_UNIT_FIRST || in->val > PED_UNIT_TEBIBYTE) {
        /* FIXME - throw range exception */
        return NULL;
    }

    out = in->val;
    return Py_BuildValue("s", ped_unit_get_name(out));
}

PyObject *py_ped_unit_get_by_name(PyObject *s, PyObject *args) {
    char *name;

    if (!PyArg_ParseTuple(args, "s", &name)) {
        /* FIXME - throw missing name exception */
        return NULL;
    }

    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_unit_format_custom_byte(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_unit_format_byte(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_unit_format_custom(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_unit_format(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_unit_parse(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_unit_parse_custom(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}
