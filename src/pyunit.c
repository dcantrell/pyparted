/*
 * pyunit.c
 *
 * Copyright (C) 2007, 2008, 2009  Red Hat, Inc.
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
 * Red Hat Author(s): David Cantrell <dcantrell@redhat.com>
 *                    Chris Lumens <clumens@redhat.com>
 */

#include <Python.h>

#include "convert.h"
#include "exceptions.h"
#include "pydevice.h"
#include "pyunit.h"

/* 1:1 function mappings for unit.h in libparted */
PyObject *py_ped_unit_set_default(PyObject *s, PyObject *args) {
    long unit;

    if (!PyArg_ParseTuple(args, "i", &unit)) {
        return NULL;
    }

    if (unit < PED_UNIT_FIRST || unit > PED_UNIT_LAST) {
        PyErr_SetString(PyExc_ValueError, "Invalid unit provided.");
        return NULL;
    }

    ped_unit_set_default(unit);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_unit_get_default(PyObject *s, PyObject *args) {
    return PyLong_FromLongLong(ped_unit_get_default());
}

PyObject *py_ped_unit_get_size(_ped_Device *s, PyObject *args) {
    long long ret = -1;
    PedDevice *dev = NULL;
    int unit;

    if (!PyArg_ParseTuple(args, "i", &unit))
        return NULL;

    if (unit < PED_UNIT_FIRST || unit > PED_UNIT_LAST) {
        PyErr_SetString(PyExc_ValueError, "Invalid unit provided.");
        return NULL;
    }

    dev = _ped_Device2PedDevice(s);
    if (dev == NULL) {
        return NULL;
    }

    ret = ped_unit_get_size(dev, unit);
    if (ret == 0) {
        /* Re-raise the libparted exception. */
        partedExnRaised = 0;
        return NULL;
    }

    return PyLong_FromLongLong(ret);
}

PyObject *py_ped_unit_get_name(PyObject *s, PyObject *args) {
    const char *name;
    int unit;

    if (!PyArg_ParseTuple(args, "i", &unit)) {
        return NULL;
    }

    if (unit < PED_UNIT_FIRST || unit > PED_UNIT_LAST) {
        PyErr_SetString(PyExc_ValueError, "Invalid unit provided.");
        return NULL;
    }

    name = ped_unit_get_name(unit);
    if (name != NULL)
        return PyString_FromString(name);
    else
        return PyString_FromString("");
}

PyObject *py_ped_unit_get_by_name(PyObject *s, PyObject *args) {
    long ret;
    char *name = NULL;

    if (!PyArg_ParseTuple(args, "z", &name)) {
        return NULL;
    }

    ret = ped_unit_get_by_name(name);
    if (ret < PED_UNIT_FIRST || ret > PED_UNIT_LAST) {
        PyErr_SetString(UnknownTypeException, name);
        return NULL;
    }

    return PyLong_FromLongLong(ret);
}

PyObject *py_ped_unit_format_custom_byte(PyObject *s, PyObject *args) {
    char *ret = NULL;
    PyObject *in_dev = NULL;
    long unit;
    PedSector sector;
    PedDevice *out_dev = NULL;

    if (!PyArg_ParseTuple(args, "O!Li", &_ped_Device_Type_obj, &in_dev, &sector,
                          &unit)) {
        return NULL;
    }

    if (unit < PED_UNIT_FIRST || unit > PED_UNIT_LAST) {
        PyErr_SetString(PyExc_ValueError, "Invalid unit provided.");
        return NULL;
    }

    out_dev = _ped_Device2PedDevice(in_dev);
    if (out_dev == NULL) {
        return NULL;
    }

    ret = ped_unit_format_custom_byte(out_dev, sector, unit);
    if (ret != NULL)
        return PyString_FromString(ret);
    else
        return PyString_FromString("");
}

PyObject *py_ped_unit_format_byte(PyObject *s, PyObject *args) {
    char *ret = NULL;
    PyObject *in_dev = NULL;
    PedSector sector;
    PedDevice *out_dev = NULL;

    if (!PyArg_ParseTuple(args, "O!L", &_ped_Device_Type_obj, &in_dev, &sector))
        return NULL;

    out_dev = _ped_Device2PedDevice(in_dev);
    if (out_dev == NULL) {
        return NULL;
    }

    ret = ped_unit_format_byte(out_dev, sector);
    if (ret != NULL)
        return PyString_FromString(ret);
    else
        return PyString_FromString("");
}

PyObject *py_ped_unit_format_custom(PyObject *s, PyObject *args) {
    char *ret = NULL;
    PyObject *in_dev = NULL;
    PedDevice *out_dev = NULL;
    PedSector sector;
    long unit;

    if (!PyArg_ParseTuple(args, "O!Li", &_ped_Device_Type_obj, &in_dev, &sector,
                          &unit)) {
        return NULL;
    }

    out_dev = _ped_Device2PedDevice(in_dev);
    if (out_dev == NULL) {
        return NULL;
    }

    ret = ped_unit_format_custom(out_dev, sector, unit);
    if (ret != NULL)
        return PyString_FromString(ret);
    else
        return PyString_FromString("");
}

PyObject *py_ped_unit_format(PyObject *s, PyObject *args) {
    char *ret = NULL;
    PyObject *in_dev = NULL;
    PedDevice *out_dev = NULL;
    PedSector sector;

    if (!PyArg_ParseTuple(args, "O!L", &_ped_Device_Type_obj, &in_dev, &sector)) {
        return NULL;
    }

    out_dev = _ped_Device2PedDevice(in_dev);
    if (out_dev == NULL) {
        return NULL;
    }

    ret = ped_unit_format(out_dev, sector);
    if (ret != NULL)
        return PyString_FromString(ret);
    else
        return PyString_FromString("");
}

PyObject *py_ped_unit_parse(PyObject *s, PyObject *args) {
    int ret;
    char *str = NULL;
    PyObject *in_dev = NULL;
    PedDevice *out_dev = NULL;
    PedSector sector;
    PyObject *in_geom = NULL;
    PedGeometry *out_geom = NULL;

    if (!PyArg_ParseTuple(args, "zOLO", &str, &in_dev, &sector, &in_geom)) {
        return NULL;
    }

    out_dev = _ped_Device2PedDevice(in_dev);
    if (out_dev == NULL) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    ret = ped_unit_parse(str, out_dev, &sector, &out_geom);

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_unit_parse_custom(PyObject *s, PyObject *args) {
    int ret;
    char *str = NULL;
    PyObject *in_dev = NULL;
    PedDevice *out_dev = NULL;
    long unit;
    PedSector sector;
    PyObject *in_geom = NULL;
    PedGeometry *out_geom = NULL;

    if (!PyArg_ParseTuple(args, "zOiLO", &str, &in_dev, &unit, &sector,
                          &in_geom)) {
        return NULL;
    }

    if (unit < PED_UNIT_FIRST || unit > PED_UNIT_LAST) {
        PyErr_SetString(PyExc_ValueError, "Invalid unit provided.");
        return NULL;
    }

    out_dev = _ped_Device2PedDevice(in_dev);
    if (out_dev == NULL) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    ret = ped_unit_parse_custom(str, out_dev, unit, &sector, &out_geom);

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

/* vim:tw=78:ts=4:et:sw=4
 */
