/*
 * pydevice.c
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
#include "pyconstraint.h"
#include "pydevice.h"
#include "docstrings/pydevice.h"
#include "typeobjects/pydevice.h"

/* _ped.CHSGeometry functions */
void _ped_CHSGeometry_dealloc(_ped_CHSGeometry *self) {
    PyObject_GC_UnTrack(self);
    PyObject_GC_Del(self);
}

int _ped_CHSGeometry_compare(_ped_CHSGeometry *self, PyObject *obj) {
    _ped_CHSGeometry *comp = NULL;
    int check = PyObject_IsInstance(obj, (PyObject *) &_ped_CHSGeometry_Type_obj);

    if (PyErr_Occurred()) {
        return -1;
    }

    if (!check) {
        PyErr_SetString(PyExc_ValueError, "object comparing to must be a _ped.CHSGeometry");
        return -1;
    }

    comp = (_ped_CHSGeometry *) obj;
    if ((self->cylinders == comp->cylinders) &&
        (self->heads == comp->heads) &&
        (self->sectors == comp->sectors)) {
        return 0;
    } else {
        return 1;
    }
}

PyObject *_ped_CHSGeometry_richcompare(_ped_CHSGeometry *a, PyObject *b,
                                       int op) {
    if (op == Py_EQ || op == Py_NE) {
        int rv = _ped_CHSGeometry_compare(a, b);
        if (PyErr_Occurred())
            return NULL;
        return PyBool_FromLong(op == Py_EQ ? rv == 0 : rv != 0);
    } else if ((op == Py_LT) || (op == Py_LE) ||
               (op == Py_GT) || (op == Py_GE)) {
        PyErr_SetString(PyExc_TypeError, "comparison operator not supported for _ped.CHSGeometry");
        return NULL;
    } else {
        PyErr_SetString(PyExc_ValueError, "unknown richcompare op");
        return NULL;
    }
}

PyObject *_ped_CHSGeometry_str(_ped_CHSGeometry *self) {
    char *ret = NULL;

    if (asprintf(&ret, "_ped.CHSGeometry instance --\n"
                       "  cylinders: %d  heads: %d  sectors: %d",
                 self->cylinders, self->heads, self->sectors) == -1) {
        return PyErr_NoMemory();
    }

    return Py_BuildValue("s", ret);
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

    free(self->model);
    free(self->path);

    Py_CLEAR(self->hw_geom);
    self->hw_geom = NULL;

    Py_CLEAR(self->bios_geom);
    self->bios_geom = NULL;

    PyObject_GC_Del(self);
}

int _ped_Device_compare(_ped_Device *self, PyObject *obj) {
    _ped_Device *comp = NULL;
    int check = PyObject_IsInstance(obj, (PyObject *) &_ped_Device_Type_obj);

    if (PyErr_Occurred()) {
        return -1;
    }

    if (!check) {
        PyErr_SetString(PyExc_ValueError, "object comparing to must be a _ped.Device");
        return -1;
    }

    comp = (_ped_Device *) obj;
    if ((!strcmp(self->model, comp->model)) &&
        (!strcmp(self->path, comp->path)) &&
        (self->type == comp->type) &&
        (self->sector_size == comp->sector_size) &&
        (self->phys_sector_size == comp->phys_sector_size) &&
        (self->length == comp->length) &&
        (self->open_count == comp->open_count) &&
        (self->read_only == comp->read_only) &&
        (self->external_mode == comp->external_mode) &&
        (self->dirty == comp->dirty) &&
        (self->boot_dirty == comp->dirty) &&
        (_ped_CHSGeometry_Type_obj.tp_richcompare(self->hw_geom, comp->hw_geom, Py_EQ)) &&
        (_ped_CHSGeometry_Type_obj.tp_richcompare(self->bios_geom, comp->bios_geom, Py_EQ)) &&
        (self->host == comp->host) &&
        (self->did == comp->did)) {
        return 0;
    } else {
        return 1;
    }
}

PyObject *_ped_Device_richcompare(_ped_Device *a, PyObject *b, int op) {
    if (op == Py_EQ || op == Py_NE) {
        int rv = _ped_Device_compare(a, b);
        if (PyErr_Occurred())
            return NULL;
        return PyBool_FromLong(op == Py_EQ ? rv == 0 : rv != 0);
    } else if ((op == Py_LT) || (op == Py_LE) ||
               (op == Py_GT) || (op == Py_GE)) {
        PyErr_SetString(PyExc_TypeError, "comparison operator not supported for _ped.Device");
        return NULL;
    } else {
        PyErr_SetString(PyExc_ValueError, "unknown richcompare op");
        return NULL;
    }
}

PyObject *_ped_Device_str(_ped_Device *self) {
    char *ret = NULL;
    char *hw_geom = NULL, *bios_geom = NULL;

    hw_geom = (char *) PyUnicode_AsUTF8(_ped_CHSGeometry_Type_obj.tp_repr(self->hw_geom));
    if (hw_geom == NULL) {
        return NULL;
    }

    bios_geom = (char *) PyUnicode_AsUTF8(_ped_CHSGeometry_Type_obj.tp_repr(self->bios_geom));
    if (bios_geom == NULL) {
        return NULL;
    }

    if (asprintf(&ret, "_ped.Device instance --\n"
                       "  model: %s  path: %s  type: %lld\n"
                       "  sector_size: %lld  phys_sector_size: %lld\n"
                       "  length: %lld  open_count: %d  read_only: %d\n"
                       "  external_mode: %d  dirty: %d  boot_dirty: %d\n"
                       "  host: %hd  did: %hd\n"
                       "  hw_geom: %s  bios_geom: %s",
                 self->model, self->path, self->type,
                 self->sector_size, self->phys_sector_size,
                 self->length, self->open_count, self->read_only,
                 self->external_mode, self->dirty, self->boot_dirty,
                 self->host, self->did,
                 hw_geom, bios_geom) == -1) {
        return PyErr_NoMemory();
    }

    return Py_BuildValue("s", ret);
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
        if (self->model != NULL)
            return PyUnicode_FromString(self->model);
        else
            return PyUnicode_FromString("");
    } else if (!strcmp(member, "path")) {
        if (self->path != NULL)
            return PyUnicode_FromString(self->path);
        else
            return PyUnicode_FromString("");
    } else if (!strcmp(member, "type")) {
        return PyLong_FromLong(self->type);
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
        PyErr_Format(DeviceException, "Could not find device for empty path");
        return NULL;
    }

    device = ped_device_get(path);
    if (device) {
        ret = PedDevice2_ped_Device(device);
    }
    else {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_Format(DeviceException, "Could not find device for path %s", path);

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

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
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

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
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

PyObject *py_ped_device_destroy(PyObject *s, PyObject *args) {
    _ped_Device *dev = (_ped_Device *) s;
    PedDevice *device = NULL;

    device = _ped_Device2PedDevice(s);
    if (device == NULL) {
        return NULL;
    }

    ped_device_destroy(device);

    Py_CLEAR(dev->hw_geom);
    dev->hw_geom = NULL;

    Py_CLEAR(dev->bios_geom);
    dev->bios_geom = NULL;

    Py_CLEAR(dev);

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

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
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

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
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
    PyObject *ret = NULL;
    PedSector start, count;
    PedDevice *device = NULL;
    char *out_buf = NULL;

    if (!PyArg_ParseTuple(args, "LL", &start, &count)) {
        return NULL;
    }

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

    if ((out_buf = malloc(device->sector_size * count)) == NULL) {
        return PyErr_NoMemory();
    }

    if (ped_device_read(device, out_buf, start, count) == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_Format(IOException, "Could not read from device %s", device->path);

        free(out_buf);
        return NULL;
    }

    ret = PyUnicode_FromString(out_buf);
    free(out_buf);

    return ret;
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

    out_buf = PyCapsule_GetPointer(in_buf, 0);
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

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_Format(IOException, "Could not write to device %s", device->path);

        return NULL;
    }

    return PyLong_FromLong(ret);
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

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
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

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
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
    PedSector start, count, ret;
    PedDevice *device = NULL;
    char *out_buf = NULL;

    if (!PyArg_ParseTuple(args, "LL", &start, &count)) {
        return NULL;
    }

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

    if ((out_buf = malloc(device->sector_size * 32)) == NULL) {
        return PyErr_NoMemory();
    }

    ret = ped_device_check(device, out_buf, start, count);
    free(out_buf);

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
    }
    else {
        PyErr_SetString(CreateException, "Could not create constraint");
        return NULL;
    }

    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}

PyObject *py_ped_device_get_minimal_aligned_constraint(PyObject *s, PyObject *args) {
    PedDevice *device = NULL;
    PedConstraint *constraint = NULL;
    _ped_Constraint *ret = NULL;

    device = _ped_Device2PedDevice(s);
    if (device == NULL) {
        return NULL;
    }

    constraint = ped_device_get_minimal_aligned_constraint(device);
    if (!constraint) {
        PyErr_SetString(CreateException, "Could not create constraint");
        return NULL;
    }

    ret = PedConstraint2_ped_Constraint(constraint);
    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}

PyObject *py_ped_device_get_optimal_aligned_constraint(PyObject *s, PyObject *args) {
    PedDevice *device = NULL;
    PedConstraint *constraint = NULL;
    _ped_Constraint *ret = NULL;

    device = _ped_Device2PedDevice(s);
    if (device == NULL) {
        return NULL;
    }

    constraint = ped_device_get_optimal_aligned_constraint(device);
    if (!constraint) {
        PyErr_SetString(CreateException, "Could not create constraint");
        return NULL;
    }

    ret = PedConstraint2_ped_Constraint(constraint);
    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}

PyObject *py_ped_device_get_minimum_alignment(PyObject *s, PyObject *args) {
    PedDevice *device = NULL;
    PedAlignment *alignment = NULL;
    _ped_Alignment *ret = NULL;

    device = _ped_Device2PedDevice(s);
    if (device == NULL) {
        return NULL;
    }

    alignment = ped_device_get_minimum_alignment(device);
    if (!alignment) {
        PyErr_SetString(CreateException, "Could not get alignment for device");
        return NULL;
    }

    ret = PedAlignment2_ped_Alignment(alignment);
    ped_alignment_destroy(alignment);

    return (PyObject *) ret;
}

PyObject *py_ped_device_get_optimum_alignment(PyObject *s, PyObject *args) {
    PedDevice *device = NULL;
    PedAlignment *alignment = NULL;
    _ped_Alignment *ret = NULL;

    device = _ped_Device2PedDevice(s);
    if (device == NULL) {
        return NULL;
    }

    alignment = ped_device_get_optimum_alignment(device);
    if (!alignment) {
        PyErr_SetString(CreateException, "Could not get alignment for device");
        return NULL;
    }

    ret = PedAlignment2_ped_Alignment(alignment);
    ped_alignment_destroy(alignment);

    return (PyObject *) ret;
}

/* vim:tw=78:ts=4:et:sw=4
 */
