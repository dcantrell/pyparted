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
#include "pyconstraint.h"
#include "pydevice.h"

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
    static char *kwlist[] = {"cylinders", "heads", "sectors", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|iii", kwlist,
                                     &self->cylinders, &self->heads,
                                     &self->sectors))
        return -1;
    else
        return 0;
}

PyObject *_ped_CHSGeometry_get(_ped_CHSGeometry *self, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.CHSGeometry()");
        return NULL;
    }

    if (!strcmp(member, "cylinders")) {
        return Py_BuildValue("i", self->cylinders);
    } else if (!strcmp(member, "heads")) {
        return Py_BuildValue("i", self->heads);
    } else if (!strcmp(member, "sectors")) {
        return Py_BuildValue("i", self->sectors);
    } else {
        PyErr_Format(PyExc_AttributeError, "_ped.CHSGeometry object has no attribute %s", member);
        return NULL;
    }
}

int _ped_CHSGeometry_set(_ped_CHSGeometry *self, PyObject *value,
                         void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.CHSGeometry()");
        return -1;
    }

    if (!strcmp(member, "cylinders")) {
        self->cylinders = PyInt_AsLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else if (!strcmp(member, "heads")) {
        self->heads = PyInt_AsLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else if (!strcmp(member, "sectors")) {
        self->sectors = PyInt_AsLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else {
        PyErr_Format(PyExc_AttributeError, "_ped.CHSGeometry object has no attribute %s", member);
        return -1;
    }

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
    static char *kwlist[] = {"model", "path", "type", "sector_size",
                             "phys_sector_size", "length", "open_count",
                             "read_only", "external_mode", "dirty", "boot_dirty",
                             "hw_geom", "bios_geom", "host", "did", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|sslllliiiiiO!O!hh", kwlist,
                                     &self->model, &self->path, &self->type,
                                     &self->sector_size, &self->phys_sector_size,
                                     &self->length, &self->open_count,
                                     &self->read_only, &self->external_mode,
                                     &self->dirty, &self->boot_dirty,
                                     &_ped_CHSGeometry_Type_obj, &self->hw_geom,
                                     &_ped_CHSGeometry_Type_obj, &self->bios_geom,
                                     &self->host, &self->did))
        return -1;
    else
        return 0;
}

PyObject *_ped_Device_get(_ped_Device *self, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Device()");
        return NULL;
    }

    if (!strcmp(member, "model")) {
        return PyString_FromString(self->model);
    } else if (!strcmp(member, "path")) {
        return PyString_FromString(self->path);
    } else if (!strcmp(member, "type")) {
        return PyLong_FromLongLong(self->type);
    } else if (!strcmp(member, "sector_size")) {
        return PyLong_FromLongLong(self->sector_size);
    } else if (!strcmp(member, "phys_sector_size")) {
        return PyLong_FromLongLong(self->phys_sector_size);
    } else if (!strcmp(member, "length")) {
        return PyLong_FromLongLong(self->length);
    } else if (!strcmp(member, "open_count")) {
        return Py_BuildValue("i", self->open_count);
    } else if (!strcmp(member, "read_only")) {
        return Py_BuildValue("i", self->read_only);
    } else if (!strcmp(member, "external_mode")) {
        return Py_BuildValue("i", self->external_mode);
    } else if (!strcmp(member, "dirty")) {
        return Py_BuildValue("i", self->dirty);
    } else if (!strcmp(member, "boot_dirty")) {
        return Py_BuildValue("i", self->boot_dirty);
    } else if (!strcmp(member, "host")) {
        return Py_BuildValue("h", self->host);
    } else if (!strcmp(member, "did")) {
        return Py_BuildValue("h", self->did);
    } else {
        PyErr_Format(PyExc_AttributeError, "_ped.Device object has no attribute %s", member);
        return NULL;
    }
}

int _ped_Device_set(_ped_Device *self, PyObject *value, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Device()");
        return -1;
    }

    if (!strcmp(member, "model")) {
        self->model = PyString_AsString(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else if (!strcmp(member, "path")) {
        self->path = PyString_AsString(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else if (!strcmp(member, "type")) {
        self->type = PyLong_AsLongLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else if (!strcmp(member, "sector_size")) {
        self->sector_size = PyLong_AsLongLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else if (!strcmp(member, "phys_sector_size")) {
        self->phys_sector_size = PyLong_AsLongLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else if (!strcmp(member, "length")) {
        self->length = PyLong_AsLongLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else if (!strcmp(member, "open_count")) {
        self->open_count = PyInt_AsLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else if (!strcmp(member, "read_only")) {
        self->read_only = PyInt_AsLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else if (!strcmp(member, "external_mode")) {
        self->external_mode = PyInt_AsLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else if (!strcmp(member, "dirty")) {
        self->dirty = PyInt_AsLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else if (!strcmp(member, "boot_dirty")) {
        self->boot_dirty = PyInt_AsLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else if (!strcmp(member, "host")) {
        if (!PyArg_ParseTuple(value, "h", &self->host)) {
            return -1;
        }
    } else if (!strcmp(member, "did")) {
        if (!PyArg_ParseTuple(value, "h", &self->did)) {
            return -1;
        }
    } else {
        PyErr_Format(PyExc_AttributeError, "_ped.Device object has no attribute %s", member);
        return -1;
    }

    return 0;
}

/* 1:1 function mappings for device.h in libparted */
PyObject *py_ped_device_probe_all(PyObject *s, PyObject *args)  {
    char *path;

    if (!PyArg_ParseTuple(args, "s", &path)) {
        return NULL;
    }

    ped_device_probe_all();

    free(path);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_device_free_all(PyObject *s, PyObject *args) {
    if (!PyArg_ParseTuple(args, "")) {
        return NULL;
    }

    ped_device_free_all();

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_device_get(PyObject *s, PyObject *args) {
    PedDevice *device;
    _ped_Device *ret;
    char *path;

    if (!PyArg_ParseTuple(args, "s", &path)) {
        return NULL;
    }

    device = ped_device_get(path);
    if (device) {
        ret = PedDevice2_ped_Device(device);
    }
    else {
        PyErr_Format(DiskException, "Could not find device for path %s", path);
        return NULL;
    }

    ped_device_destroy(device);

    return (PyObject *) ret;
}

PyObject *py_ped_device_get_next(PyObject *s, PyObject *args) {
    PyObject *in_device;
    PedDevice *out_device, *device;
    _ped_Device *ret;

    if (!PyArg_ParseTuple(args, "|O!", &_ped_Device_Type_obj, &in_device)) {
        return NULL;
    }

    if (!in_device)
       out_device = NULL;
    else
       out_device = _ped_Device2PedDevice(in_device);

    device = ped_device_get_next(out_device);
    if (device) {
        ret = PedDevice2_ped_Device(device);
    }
    else {
        PyErr_SetNone(PyExc_IndexError);
        return NULL;
    }

    ped_device_destroy(out_device);
    ped_device_destroy(device);

    return (PyObject *) ret;
}

PyObject *py_ped_device_is_busy(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_device;
    PedDevice *out_device;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Device_Type_obj, &in_device)) {
        return NULL;
    }

    out_device = _ped_Device2PedDevice(in_device);
    if (out_device == NULL) {
        return NULL;
    }

    ret = ped_device_is_busy(out_device);
    ped_device_destroy(out_device);
    return PyBool_FromLong(ret);
}

PyObject *py_ped_device_open(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_device;
    PedDevice *out_device;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Device_Type_obj, &in_device)) {
        return NULL;
    }

    out_device = _ped_Device2PedDevice(in_device);
    if (out_device == NULL) {
        return NULL;
    }

    ret = ped_device_open(out_device);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_Format(IOException, "Could not open device %s", out_device->path);

        return NULL;
    }

    ped_device_destroy(out_device);
    return PyBool_FromLong(ret);
}

PyObject *py_ped_device_close(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_device;
    PedDevice *out_device;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Device_Type_obj, &in_device)) {
        return NULL;
    }

    out_device = _ped_Device2PedDevice(in_device);
    if (out_device == NULL) {
        return NULL;
    }

    ret = ped_device_close(out_device);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_Format(IOException, "Could not close device %s", out_device->path);

        return NULL;
    }

    ped_device_destroy(out_device);
    return PyBool_FromLong(ret);
}

PyObject *py_ped_device_destroy(PyObject *s, PyObject *args) {
    PyObject *in_device;
    PedDevice *out_device;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Device_Type_obj, &in_device)) {
        return NULL;
    }

    out_device = _ped_Device2PedDevice(in_device);
    if (out_device == NULL) {
        return NULL;
    }

    ped_device_destroy(out_device);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_device_cache_remove(PyObject *s, PyObject *args) {
    PyObject *in_device;
    PedDevice *out_device;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Device_Type_obj, &in_device)) {
        return NULL;
    }

    out_device = _ped_Device2PedDevice(in_device);
    if (out_device == NULL) {
        return NULL;
    }

    ped_device_cache_remove(out_device);

    ped_device_destroy(out_device);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_device_begin_external_access(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_device;
    PedDevice *out_device;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Device_Type_obj, &in_device)) {
        return NULL;
    }

    out_device = _ped_Device2PedDevice(in_device);
    if (out_device == NULL) {
        return NULL;
    }

    ret = ped_device_begin_external_access(out_device);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_Format(IOException, "Could not begin external access mode on device %s", out_device->path);

        return NULL;
    }

    ped_device_destroy(out_device);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_device_end_external_access(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_device;
    PedDevice *out_device;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Device_Type_obj, &in_device)) {
        return NULL;
    }

    out_device = _ped_Device2PedDevice(in_device);
    if (out_device == NULL) {
        return NULL;
    }

    ret = ped_device_end_external_access(out_device);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_Format(IOException, "Could not end external access mode on device %s", out_device->path);

        return NULL;
    }

    ped_device_destroy(out_device);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_device_read(PyObject *s, PyObject *args) {
    PyObject *in_dev, *in_buf;
    PedSector start, count, ret;
    PedDevice *out_dev;
    void *out_buf;

    if (!PyArg_ParseTuple(args, "O!Oll", &_ped_Device_Type_obj, &in_dev,
                          &in_buf, &start, &count)) {
        return NULL;
    }

    out_dev = _ped_Device2PedDevice(in_dev);
    if (out_dev == NULL) {
        return NULL;
    }

    out_buf = PyCObject_AsVoidPtr(in_buf);
    if (out_buf == NULL) {
        return NULL;
    }

    ret = ped_device_read(out_dev, out_buf, start, count);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_Format(IOException, "Could not read from device %s", out_dev->path);

        return NULL;
    }

    ped_device_destroy(out_dev);

    return PyLong_FromLongLong(ret);
}

PyObject *py_ped_device_write(PyObject *s, PyObject *args) {
    PyObject *in_dev, *in_buf;
    PedSector start, count, ret;
    PedDevice *out_dev;
    void *out_buf;

    if (!PyArg_ParseTuple(args, "O!Oll", &_ped_Device_Type_obj, &in_dev,
                          &in_buf, &start, &count)) {
        return NULL;
    }

    out_dev = _ped_Device2PedDevice(in_dev);
    if (out_dev == NULL) {
        return NULL;
    }

    out_buf = PyCObject_AsVoidPtr(in_buf);
    if (out_buf == NULL) {
        return NULL;
    }

    ret = ped_device_write(out_dev, out_buf, start, count);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_Format(IOException, "Could not write to device %s", out_dev->path);

        return NULL;
    }

    ped_device_destroy(out_dev);

    return PyLong_FromLongLong(ret);
}

PyObject *py_ped_device_sync(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_device;
    PedDevice *out_device;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Device_Type_obj, &in_device)) {
        return NULL;
    }

    out_device = _ped_Device2PedDevice(in_device);
    if (out_device == NULL) {
        return NULL;
    }

    ret = ped_device_sync(out_device);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_Format(IOException, "Could not sync device %s", out_device->path);

        return NULL;
    }

    ped_device_destroy(out_device);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_device_sync_fast(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_device;
    PedDevice *out_device;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Device_Type_obj, &in_device)) {
        return NULL;
    }

    out_device = _ped_Device2PedDevice(in_device);
    if (out_device == NULL) {
        return NULL;
    }

    ret = ped_device_sync_fast(out_device);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_Format(IOException, "Could not sync device %s", out_device->path);

        return NULL;
    }

    ped_device_destroy(out_device);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_device_check(PyObject *s, PyObject *args) {
    PyObject *in_dev, *in_buf;
    PedSector start, count, ret;
    PedDevice *out_dev;
    void *out_buf;

    if (!PyArg_ParseTuple(args, "O!Oll", &_ped_Device_Type_obj, &in_dev,
                          &in_buf, &start, &count)) {
        return NULL;
    }

    out_dev = _ped_Device2PedDevice(in_dev);
    if (out_dev == NULL) {
        return NULL;
    }

    out_buf = PyCObject_AsVoidPtr(in_buf);
    if (out_buf == NULL) {
        return NULL;
    }

    ret = ped_device_check(out_dev, out_buf, start, count);
    ped_device_destroy(out_dev);

    return PyLong_FromLongLong(ret);
}

PyObject *py_ped_device_get_constraint(PyObject *s, PyObject *args) {
    PyObject *in_device;
    PedDevice *out_device;
    PedConstraint *constraint;
    _ped_Constraint *ret;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Device_Type_obj, &in_device)) {
        return NULL;
    }

    out_device = _ped_Device2PedDevice(in_device);
    if (out_device == NULL) {
        return NULL;
    }

    constraint = ped_device_get_constraint(out_device);
    if (constraint) {
        ret = PedConstraint2_ped_Constraint(constraint);
        if (ret == NULL) {
            return NULL;
        }
    }
    else {
        PyErr_SetString(CreateException, "Could not create constraint");
        return NULL;
    }

    ped_device_destroy(out_device);
    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}
