/*
 * pydevice.c
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
#include "pyconstraint.h"
#include "pydevice.h"
#include "docstrings/pydevice.h"
#include "typeobjects/pydevice.h"

/* _ped.CHSGeometry functions */
void _ped_CHSGeometry_dealloc(_ped_CHSGeometry *self) {
    PyObject_GC_UnTrack(self);
    PyObject_GC_Del(self);
}

int _ped_CHSGeometry_traverse(_ped_CHSGeometry *self, visitproc visit, void *arg) {
    return 0;
}

int _ped_CHSGeometry_clear(_ped_CHSGeometry *self) {
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

/* _ped.Device functions */
void _ped_Device_dealloc(_ped_Device *self) {
    PyObject_GC_UnTrack(self);

    Py_CLEAR(self->hw_geom);
    self->hw_geom = NULL;

    Py_CLEAR(self->bios_geom);
    self->bios_geom = NULL;

    PyObject_GC_Del(self);
}

int _ped_Device_traverse(_ped_Device *self, visitproc visit, void *arg) {
    int err;

    if (self->hw_geom) {
        if ((err = visit(self->hw_geom, arg))) {
            return err;
        }
    }

    if (self->bios_geom) {
        if ((err = visit(self->bios_geom, arg))) {
            return err;
        }
    }

    return 0;
}

int _ped_Device_clear(_ped_Device *self) {
    Py_CLEAR(self->hw_geom);
    self->hw_geom = NULL;

    Py_CLEAR(self->bios_geom);
    self->bios_geom = NULL;

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

/*
 * Returns the _ped.DiskType for the specified _ped.Device.
 * Even though this function is part of pydisk.c, it's a method
 * on _ped.Device since it operates on _ped.Device objects and
 * not on _ped.Disk objects.
 */
PyObject *py_ped_disk_probe(PyObject *s, PyObject *args) {
    PedDevice *device = NULL;
    PedDiskType *type = NULL;
    _ped_DiskType *ret = NULL;

    device = _ped_Device2PedDevice(s);
    if (device) {
        type = ped_disk_probe(device);
        if (type == NULL) {
            PyErr_Format(IOException, "Could not probe device %s", device->path);
            return NULL;
        }

        ret = PedDiskType2_ped_DiskType(type);
        if (ret == NULL) {
            return NULL;
        }

        free(type);
    }

    return (PyObject *) ret;
}

/* 1:1 function mappings for device.h in libparted */
PyObject *py_ped_device_probe_all(PyObject *s, PyObject *args)  {
    ped_device_probe_all();

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_device_free_all(PyObject *s, PyObject *args) {
    ped_device_free_all();

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_device_get(PyObject *s, PyObject *args) {
    PedDevice *device = NULL;
    _ped_Device *ret = NULL;
    char *path = NULL;

    if (!PyArg_ParseTuple(args, "z", &path)) {
        return NULL;
    }

    if (path == NULL) {
        PyErr_Format(DiskException, "Could not find device for empty path");
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

    return (PyObject *) ret;
}

PyObject *py_ped_device_get_next(PyObject *s, PyObject *args) {
    PyObject *in_device = NULL;
    PedDevice *cur = NULL, *next = NULL;
    _ped_Device *ret = NULL;

    if (!PyArg_ParseTuple(args, "|O!", &_ped_Device_Type_obj, &in_device)) {
        return NULL;
    }

    if (in_device) {
        cur = _ped_Device2PedDevice(in_device);
        if (!cur) {
            return NULL;
        }
    }

    next = ped_device_get_next(cur);
    if (next) {
        ret = PedDevice2_ped_Device(next);
        return (PyObject *) ret;
    }
    else {
        PyErr_SetNone(PyExc_IndexError);
        return NULL;
    }
}

PyObject *py_ped_device_is_busy(PyObject *s, PyObject *args) {
    int ret = -1;
    PedDevice *device = NULL;

    device = _ped_Device2PedDevice(s);
    if (device == NULL) {
        return NULL;
    }

    ret = ped_device_is_busy(device);

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_device_open(PyObject *s, PyObject *args) {
    int ret = -1;
    PedDevice *device = NULL;

    device = _ped_Device2PedDevice(s);
    if (device == NULL) {
        return NULL;
    }

    if (device->external_mode) {
        PyErr_Format(IOException, "Device %s is already open for external access.", device->path);
        return NULL;
    }

    ret = ped_device_open(device);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_Format(IOException, "Could not open device %s", device->path);

        return NULL;
    }

    ((_ped_Device *) s)->open_count = device->open_count;

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_device_close(PyObject *s, PyObject *args) {
    int ret = -1;
    PedDevice *device = NULL;

    device = _ped_Device2PedDevice(s);
    if (device == NULL) {
        return NULL;
    }

    if (!device->open_count) {
        PyErr_Format(IOException, "Device %s is not open.", device->path);
        return NULL;
    }

    if (device->external_mode) {
        PyErr_Format(IOException, "Device %s is already open for external access.", device->path);
        return NULL;
    }

    ret = ped_device_close(device);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_Format(IOException, "Could not close device %s", device->path);

        return NULL;
    }

    ((_ped_Device *) s)->open_count = device->open_count;

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

/*
 * XXX: the reason we want to call ped_device_destroy() here is
 * to unregister the device.  We need to figure out how to also
 * get a 'del' operation run on the _ped.Device itself.
 *
 * Possible solution could be to put a call to ped_device_destroy()
 * in the tp_clear function to make sure it gets unregistered.
 */
PyObject *py_ped_device_destroy(PyObject *s, PyObject *args) {
    PedDevice *device = NULL;

    device = _ped_Device2PedDevice(s);
    if (device == NULL) {
        return NULL;
    }

    ped_device_destroy(device);
    Py_CLEAR(s);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_device_cache_remove(PyObject *s, PyObject *args) {
    PedDevice *device = NULL;

    device = _ped_Device2PedDevice(s);
    if (device == NULL) {
        return NULL;
    }

    ped_device_cache_remove(device);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_device_begin_external_access(PyObject *s, PyObject *args) {
    int ret = -1;
    PedDevice *device = NULL;

    device = _ped_Device2PedDevice(s);
    if (device == NULL) {
        return NULL;
    }

    if (device->external_mode) {
        PyErr_Format(IOException, "Device %s is already open for external access.", device->path);
        return NULL;
    }

    ret = ped_device_begin_external_access(device);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_Format(IOException, "Could not begin external access mode on device %s", device->path);

        return NULL;
    }

    ((_ped_Device *) s)->external_mode = device->external_mode;

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_device_end_external_access(PyObject *s, PyObject *args) {
    int ret = -1;
    PedDevice *device = NULL;

    device = _ped_Device2PedDevice(s);
    if (device == NULL) {
        return NULL;
    }

    if (!device->external_mode) {
        PyErr_Format(IOException, "Device %s is not open for external access.", device->path);
        return NULL;
    }

    ret = ped_device_end_external_access(device);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_Format(IOException, "Could not end external access mode on device %s", device->path);

        return NULL;
    }

    ((_ped_Device *) s)->external_mode = device->external_mode;

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_device_read(PyObject *s, PyObject *args) {
    PyObject *in_buf = NULL;
    PedSector start, count, ret;
    PedDevice *device = NULL;
    void *out_buf = NULL;

    if (!PyArg_ParseTuple(args, "OLL", &in_buf, &start, &count)) {
        return NULL;
    }

    device = _ped_Device2PedDevice(s);
    if (device == NULL) {
        return NULL;
    }

    out_buf = PyCObject_AsVoidPtr(in_buf);
    if (out_buf == NULL) {
        return NULL;
    }

    if (!device->open_count) {
        PyErr_Format(IOException, "Device %s is not open.", device->path);
        return NULL;
    }

    if (device->external_mode) {
        PyErr_Format(IOException, "Device %s is already open for external access.", device->path);
        return NULL;
    }

    ret = ped_device_read(device, out_buf, start, count);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_Format(IOException, "Could not read from device %s", device->path);

        return NULL;
    }

    return PyLong_FromLongLong(ret);
}

PyObject *py_ped_device_write(PyObject *s, PyObject *args) {
    PyObject *in_buf = NULL;
    PedSector start, count, ret;
    PedDevice *device = NULL;
    void *out_buf = NULL;

    if (!PyArg_ParseTuple(args, "OLL", &in_buf, &start, &count)) {
        return NULL;
    }

    device = _ped_Device2PedDevice(s);
    if (device == NULL) {
        return NULL;
    }

    out_buf = PyCObject_AsVoidPtr(in_buf);
    if (out_buf == NULL) {
        return NULL;
    }

    if (!device->open_count) {
        PyErr_Format(IOException, "Device %s is not open.", device->path);
        return NULL;
    }

    if (device->external_mode) {
        PyErr_Format(IOException, "Device %s is already open for external access.", device->path);
        return NULL;
    }

    ret = ped_device_write(device, out_buf, start, count);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_Format(IOException, "Could not write to device %s", device->path);

        return NULL;
    }

    return PyLong_FromLongLong(ret);
}

PyObject *py_ped_device_sync(PyObject *s, PyObject *args) {
    int ret = -1;
    PedDevice *device = NULL;

    device = _ped_Device2PedDevice(s);
    if (device == NULL) {
        return NULL;
    }

    if (!device->open_count) {
        PyErr_Format(IOException, "Device %s is not open.", device->path);
        return NULL;
    }

    if (device->external_mode) {
        PyErr_Format(IOException, "Device %s is already open for external access.", device->path);
        return NULL;
    }

    ret = ped_device_sync(device);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_Format(IOException, "Could not sync device %s", device->path);

        return NULL;
    }

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_device_sync_fast(PyObject *s, PyObject *args) {
    int ret = -1;
    PedDevice *device = NULL;

    device = _ped_Device2PedDevice(s);
    if (device == NULL) {
        return NULL;
    }

    if (!device->open_count) {
        PyErr_Format(IOException, "Device %s is not open.", device->path);
        return NULL;
    }

    if (device->external_mode) {
        PyErr_Format(IOException, "Device %s is already open for external access.", device->path);
        return NULL;
    }

    ret = ped_device_sync_fast(device);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_Format(IOException, "Could not sync device %s", device->path);

        return NULL;
    }

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_device_check(PyObject *s, PyObject *args) {
    PyObject *in_buf = NULL;
    PedSector start, count, ret;
    PedDevice *device = NULL;
    void *out_buf = NULL;

    if (!PyArg_ParseTuple(args, "OLL", &in_buf, &start, &count)) {
        return NULL;
    }

    device = _ped_Device2PedDevice(s);
    if (device == NULL) {
        return NULL;
    }

    out_buf = PyCObject_AsVoidPtr(in_buf);
    if (out_buf == NULL) {
        return NULL;
    }

    if (!device->open_count) {
        PyErr_Format(IOException, "Device %s is not open.", device->path);
        return NULL;
    }

    if (device->external_mode) {
        PyErr_Format(IOException, "Device %s is already open for external access.", device->path);
        return NULL;
    }

    ret = ped_device_check(device, out_buf, start, count);

    return PyLong_FromLongLong(ret);
}

PyObject *py_ped_device_get_constraint(PyObject *s, PyObject *args) {
    PedDevice *device = NULL;
    PedConstraint *constraint = NULL;
    _ped_Constraint *ret = NULL;

    device = _ped_Device2PedDevice(s);
    if (device == NULL) {
        return NULL;
    }

    constraint = ped_device_get_constraint(device);
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

    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}

/* vim:tw=78:ts=4:et:sw=4
 */
