/*
 * pyunit.c
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

#include <Python.h>

#include "convert.h"
#include "exceptions.h"
#include "pydevice.h"
#include "pyunit.h"

/* 1:1 function mappings for unit.h in libparted */
PyObject *py_ped_unit_set_default(PyObject *s, PyObject *args) {
    int unit;

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
    return PyLong_FromLong(ped_unit_get_default());
}

PyObject *py_ped_unit_get_size(PyObject *s, PyObject *args) {
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
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_Occurred()) {
                PyErr_SetString(PyExc_ValueError, partedExnMessage);
            }
        } else {
            PyErr_SetString(PyExc_ValueError, "Could not get size");
        }

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

    /*
     * DO NOT free the result from ped_unit_get_name(), it's a pointer to
     * a value in the static unit_names[] array in libparted.
     */
    name = ped_unit_get_name(unit);
    if (name != NULL) {
        return PyUnicode_FromString(name);
    } else {
        return PyUnicode_FromString("");
    }
}

PyObject *py_ped_unit_get_by_name(PyObject *s, PyObject *args) {
    int ret;
    char *name = NULL;

    if (!PyArg_ParseTuple(args, "z", &name)) {
        return NULL;
    }

    ret = ped_unit_get_by_name(name);
    if (ret < PED_UNIT_FIRST || ret > PED_UNIT_LAST) {
        PyErr_SetString(UnknownTypeException, name);
        return NULL;
    }

    return Py_BuildValue("i", ret);
}

PyObject *py_ped_unit_format_custom_byte(PyObject *s, PyObject *args) {
    PyObject *ret = NULL;
    char *pedret = NULL;
    PedSector sector;
    int unit;
    PedDevice *out_dev = NULL;

    if (!PyArg_ParseTuple(args, "Li", &sector, &unit)) {
        return NULL;
    }

    if (unit < PED_UNIT_FIRST || unit > PED_UNIT_LAST) {
        PyErr_SetString(PyExc_ValueError, "Invalid unit provided.");
        return NULL;
    }

    out_dev = _ped_Device2PedDevice(s);
    if (out_dev == NULL) {
        return NULL;
    }

    pedret = ped_unit_format_custom_byte(out_dev, sector, unit);
    if (pedret != NULL) {
        ret = PyUnicode_FromString(pedret);
        free(pedret);
    } else {
        ret = PyUnicode_FromString("");
    }

    return ret;
}

PyObject *py_ped_unit_format_byte(PyObject *s, PyObject *args) {
    PyObject *ret = NULL;
    char *pedret = NULL;
    PedSector sector;
    PedDevice *out_dev = NULL;

    if (!PyArg_ParseTuple(args, "L", &sector)) {
        return NULL;
    }

    out_dev = _ped_Device2PedDevice(s);
    if (out_dev == NULL) {
        return NULL;
    }

    pedret = ped_unit_format_byte(out_dev, sector);
    if (pedret != NULL) {
        ret = PyUnicode_FromString(pedret);
        free(pedret);
    } else {
        ret = PyUnicode_FromString("");
    }

    return ret;
}

PyObject *py_ped_unit_format_custom(PyObject *s, PyObject *args) {
    PyObject *ret = NULL;
    char *pedret = NULL;
    PedDevice *out_dev = NULL;
    PedSector sector;
    int unit;

    if (!PyArg_ParseTuple(args, "Li", &sector, &unit)) {
        return NULL;
    }

    out_dev = _ped_Device2PedDevice(s);
    if (out_dev == NULL) {
        return NULL;
    }

    pedret = ped_unit_format_custom(out_dev, sector, unit);
    if (pedret != NULL) {
        ret = PyUnicode_FromString(pedret);
        free(pedret);
    } else {
        ret = PyUnicode_FromString("");
    }

    return ret;
}

PyObject *py_ped_unit_format(PyObject *s, PyObject *args) {
    PyObject *ret = NULL;
    char *pedret = NULL;
    PedDevice *out_dev = NULL;
    PedSector sector;

    if (!PyArg_ParseTuple(args, "L", &sector)) {
        return NULL;
    }

    out_dev = _ped_Device2PedDevice(s);
    if (out_dev == NULL) {
        return NULL;
    }

    pedret = ped_unit_format(out_dev, sector);
    if (pedret != NULL) {
        ret = PyUnicode_FromString(pedret);
        free(pedret);
    } else {
        ret = PyUnicode_FromString("");
    }

    return ret;
}

PyObject *py_ped_unit_parse(PyObject *s, PyObject *args) {
    int ret;
    char *str = NULL;
    PedDevice *out_dev = NULL;
    PedSector sector;
    PyObject *in_geom = NULL;
    PedGeometry *out_geom = NULL;

    if (!PyArg_ParseTuple(args, "zLO!", &str, &sector,
                          &_ped_Geometry_Type_obj, &in_geom)) {
        return NULL;
    }

    out_dev = _ped_Device2PedDevice(s);
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
    PedDevice *out_dev = NULL;
    int unit;
    PedSector sector;
    PyObject *in_geom = NULL;
    PedGeometry *out_geom = NULL;

    if (!PyArg_ParseTuple(args, "ziLO!", &str, &unit, &sector,
                          &_ped_Geometry_Type_obj, &in_geom)) {
        return NULL;
    }

    if (unit < PED_UNIT_FIRST || unit > PED_UNIT_LAST) {
        PyErr_SetString(PyExc_ValueError, "Invalid unit provided.");
        return NULL;
    }

    out_dev = _ped_Device2PedDevice(s);
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
