/*
 * pydevice.c
 *
 * Copyright (C) 2007  Red Hat, Inc.
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

/* _ped.CHSGeometry type object */
static PyMemberDef _ped_CHSGeometry_members[] = {
    {NULL}
};

static PyMethodDef _ped_CHSGeometry_methods[] = {
    {NULL}
};

static PyGetSetDef _ped_CHSGeometry_getset[] = {
    {"cylinders", (getter) _ped_CHSGeometry_get,
                  (setter) _ped_CHSGeometry_set,
                  "The number of cylinders.", "cylinders"},
    {"heads", (getter) _ped_CHSGeometry_get,
              (setter) _ped_CHSGeometry_set,
              "The number of heads", "heads"},
    {"sectors", (getter) _ped_CHSGeometry_get,
                (setter) _ped_CHSGeometry_set,
                "The number of sectors", "sectors"},
    {NULL}  /* Sentinel */
};

PyTypeObject _ped_CHSGeometry_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    .tp_name = "_ped.CHSGeometry",
    .tp_basicsize = sizeof(_ped_CHSGeometry),
 /* .tp_itemsize = XXX */
    .tp_dealloc = (destructor) _ped_CHSGeometry_dealloc,
 /* .tp_print = XXX */
 /* .tp_getattr = XXX */
 /* .tp_setattr = XXX */
 /* .tp_compare = XXX */
 /* .tp_repr = XXX */
 /* .tp_as_number = XXX */
 /* .tp_as_sequence = XXX */
 /* .tp_as_mapping = XXX */
 /* .tp_hash = XXX */
 /* .tp_call = XXX */
 /* .tp_str = XXX */
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_setattro = PyObject_GenericSetAttr,
 /* .tp_as_buffer = XXX */
    .tp_flags = Py_TPFLAGS_HAVE_CLASS | Py_TPFLAGS_BASETYPE |
                Py_TPFLAGS_HAVE_GC,
    .tp_doc = _ped_CHSGeometry_doc,
    .tp_traverse = (traverseproc) _ped_CHSGeometry_traverse,
    .tp_clear = (inquiry) _ped_CHSGeometry_clear,
 /* .tp_richcompare = XXX */
 /* .tp_weaklistoffset = XXX */
 /* .tp_iter = XXX */
 /* .tp_iternext = XXX */
    .tp_methods = _ped_CHSGeometry_methods,
    .tp_members = _ped_CHSGeometry_members,
    .tp_getset = _ped_CHSGeometry_getset,
    .tp_base = NULL,
 /* .tp_dict = XXX */
 /* .tp_descr_get = XXX */
 /* .tp_descr_set = XXX */
 /* .tp_dictoffset = XXX */
    .tp_init = NULL,
    .tp_alloc = PyType_GenericAlloc,
    .tp_new = NULL,
 /* .tp_free = XXX */
 /* .tp_is_gc = XXX */
    .tp_bases = NULL,
 /* .tp_mro = XXX */
 /* .tp_cache = XXX */
 /* .tp_subclasses = XXX */
 /* .tp_weaklist = XXX */
 /* .tp_del = XXX */
};

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

/* _ped.Device type object */
static PyMemberDef _ped_Device_members[] = {
    {"hw_geom", T_OBJECT, offsetof(_ped_Device, hw_geom), 0,
                "The CHSGeometry of the Device as reported by the hardware."},
    {"bios_geom", T_OBJECT, offsetof(_ped_Device, bios_geom), 0,
                  "The CHSGeometry of the Device as reported by the BIOS."},
    {NULL}
};

static PyMethodDef _ped_Device_methods[] = {
    {"get_next", (PyCFunction) py_ped_device_get_next, METH_VARARGS,
                 device_get_next_doc},
    {"is_busy", (PyCFunction) py_ped_device_is_busy, METH_VARARGS,
                device_is_busy_doc},
    {"open", (PyCFunction) py_ped_device_open, METH_VARARGS,
             device_open_doc},
    {"close", (PyCFunction) py_ped_device_close, METH_VARARGS,
              device_close_doc},
    {"destroy", (PyCFunction) py_ped_device_destroy, METH_VARARGS,
                device_destroy_doc},
    {"cache_remove", (PyCFunction) py_ped_device_cache_remove,
                     METH_VARARGS, device_cache_remove_doc},
    {"begin_external_access", (PyCFunction) py_ped_device_begin_external_access,
                              METH_VARARGS, device_begin_external_access_doc},
    {"end_external_access", (PyCFunction) py_ped_device_end_external_access,
                            METH_VARARGS, device_end_external_access_doc},
    {"read", (PyCFunction) py_ped_device_read, METH_VARARGS,
             device_read_doc},
    {"write", (PyCFunction) py_ped_device_write, METH_VARARGS,
              device_write_doc},
    {"sync", (PyCFunction) py_ped_device_sync, METH_VARARGS,
             device_sync_doc},
    {"sync_fast", (PyCFunction) py_ped_device_sync_fast, METH_VARARGS,
                  device_sync_fast_doc},
    {"check", (PyCFunction) py_ped_device_check, METH_VARARGS,
              device_check_doc},
    {"get_constraint", (PyCFunction) py_ped_device_get_constraint,
                       METH_VARARGS, device_get_constraint_doc},
    {NULL}
};

static PyGetSetDef _ped_Device_getset[] = {
    {"model", (getter) _ped_Device_get, (setter) _ped_Device_set,
              "A brief description of the hardware, usually mfr and model.",
              "model"},
    {"path", (getter) _ped_Device_get, (setter) _ped_Device_set,
             "The operating system level path to the device node.", "path"},
    {"type", (getter) _ped_Device_get, (setter) _ped_Device_set,
             "The type of device, deprecated in favor of PedDeviceType", "type"},
    {"sector_size", (getter) _ped_Device_get, (setter) _ped_Device_set,
                    "Logical sector size.", "sector_size"},
    {"phys_sector_size", (getter) _ped_Device_get, (setter) _ped_Device_set,
                         "Physical sector size.", "phys_sector_size"},
    {"length", (getter) _ped_Device_get, (setter) _ped_Device_set,
               "Device length, in sectors (LBA).", "length"},
    {"open_count", (getter) _ped_Device_get, (setter) _ped_Device_set,
                   "How many times self.open() has been called.", "open_count"},
    {"read_only", (getter) _ped_Device_get, (setter) _ped_Device_set,
                  "Is the device opened in read-only mode?", "read_only"},
    {"external_mode", (getter) _ped_Device_get, (setter) _ped_Device_set,
                      "PedDevice external_mode", "external_mode"},
    {"dirty", (getter) _ped_Device_get, (setter) _ped_Device_set,
              "Have any unflushed changes been made to self?", "dirty"},
    {"boot_dirty", (getter) _ped_Device_get, (setter) _ped_Device_set,
                   "Have any unflushed changes been made to the bootloader?",
                   "boot_dirty"},
    {"host", (getter) _ped_Device_get, (setter) _ped_Device_set,
             "Any SCSI host ID associated with self.", "host"},
    {"did", (getter) _ped_Device_get, (setter) _ped_Device_set,
            "Any SCSI device ID associated with self.", "did"},
    {NULL}  /* Sentinel */
};

PyTypeObject _ped_Device_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    .tp_name = "_ped.Device",
    .tp_basicsize = PyGC_HEAD_SIZE + sizeof(_ped_Device),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor) _ped_Device_dealloc,
 /* .tp_print = XXX */
 /* .tp_getattr = XXX */
 /* .tp_setattr = XXX */
 /* .tp_compare = XXX */
 /* .tp_repr = XXX */
 /* .tp_as_number = XXX */
 /* .tp_as_sequence = XXX */
 /* .tp_as_mapping = XXX */
 /* .tp_hash = XXX */
 /* .tp_call = XXX */
 /* .tp_str = XXX */
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_setattro = PyObject_GenericSetAttr,
 /* .tp_as_buffer = XXX */
    .tp_flags = Py_TPFLAGS_HAVE_CLASS | Py_TPFLAGS_BASETYPE |
                Py_TPFLAGS_HAVE_GC,
    .tp_doc = _ped_Device_doc,
    .tp_traverse = (traverseproc) _ped_Device_traverse,
    .tp_clear = (inquiry) _ped_Device_clear,
 /* .tp_richcompare = XXX */
 /* .tp_weaklistoffset = XXX */
 /* .tp_iter = XXX */
 /* .tp_iternext = XXX */
    .tp_methods = _ped_Device_methods,
    .tp_members = _ped_Device_members,
    .tp_getset = _ped_Device_getset,
    .tp_base = NULL,
    .tp_dict = NULL,
 /* .tp_descr_get = XXX */
 /* .tp_descr_set = XXX */
 /* .tp_dictoffset = XXX */
    .tp_init = NULL,
    .tp_alloc = PyType_GenericAlloc,
    .tp_new = NULL,
 /* .tp_free = XXX */
 /* .tp_is_gc = XXX */
    .tp_bases = NULL,
 /* .tp_mro = XXX */
 /* .tp_cache = XXX */
 /* .tp_subclasses = XXX */
 /* .tp_weaklist = XXX */
 /* .tp_del = XXX */
};

/* _ped.Device functions */
void _ped_Device_dealloc(_ped_Device *self) {
    PyObject_GC_UnTrack(self);
    Py_XDECREF(self->hw_geom);
    Py_XDECREF(self->bios_geom);
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
    Py_XDECREF(self->hw_geom);
    self->hw_geom = NULL;

    Py_XDECREF(self->bios_geom);
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
    PedDevice *self = NULL, *device = NULL;
    _ped_Device *ret = NULL;

    self = _ped_Device2PedDevice(s);
    device = ped_device_get_next(self);

    if (device) {
        ret = PedDevice2_ped_Device(device);
    } else {
        PyErr_SetNone(PyExc_IndexError);
        return NULL;
    }

    ped_device_destroy(self);
    ped_device_destroy(device);

    return (PyObject *) ret;
}

PyObject *py_ped_device_is_busy(PyObject *s, PyObject *args) {
    int ret = -1;
    PedDevice *device = NULL;

    device = _ped_Device2PedDevice(s);
    if (device == NULL) {
        return NULL;
    }

    ret = ped_device_is_busy(device);
    ped_device_destroy(device);
    return PyBool_FromLong(ret);
}

PyObject *py_ped_device_open(PyObject *s, PyObject *args) {
    int ret = -1;
    PedDevice *device = NULL;

    device = _ped_Device2PedDevice(s);
    if (device == NULL) {
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

    ped_device_destroy(device);
    return PyBool_FromLong(ret);
}

PyObject *py_ped_device_close(PyObject *s, PyObject *args) {
    int ret = -1;
    PedDevice *device = NULL;

    device = _ped_Device2PedDevice(s);
    if (device == NULL) {
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

    ped_device_destroy(device);
    return PyBool_FromLong(ret);
}

PyObject *py_ped_device_destroy(PyObject *s, PyObject *args) {
    PedDevice *device = NULL;

    device = _ped_Device2PedDevice(s);
    if (device == NULL) {
        return NULL;
    }

    ped_device_destroy(device);

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
    ped_device_destroy(device);

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

    ped_device_destroy(device);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_device_end_external_access(PyObject *s, PyObject *args) {
    int ret = -1;
    PedDevice *device = NULL;

    device = _ped_Device2PedDevice(s);
    if (device == NULL) {
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

    ped_device_destroy(device);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_device_read(PyObject *s, PyObject *args) {
    PyObject *in_buf = NULL;
    PedSector start, count, ret;
    PedDevice *device = NULL;
    void *out_buf = NULL;

    if (!PyArg_ParseTuple(args, "Oll", &in_buf, &start, &count)) {
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

    ped_device_destroy(device);

    return PyLong_FromLongLong(ret);
}

PyObject *py_ped_device_write(PyObject *s, PyObject *args) {
    PyObject *in_buf = NULL;
    PedSector start, count, ret;
    PedDevice *device = NULL;
    void *out_buf = NULL;

    if (!PyArg_ParseTuple(args, "Oll", &in_buf, &start, &count)) {
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

    ped_device_destroy(device);

    return PyLong_FromLongLong(ret);
}

PyObject *py_ped_device_sync(PyObject *s, PyObject *args) {
    int ret = -1;
    PedDevice *device = NULL;

    device = _ped_Device2PedDevice(s);
    if (device == NULL) {
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

    ped_device_destroy(device);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_device_sync_fast(PyObject *s, PyObject *args) {
    int ret = -1;
    PedDevice *device = NULL;

    device = _ped_Device2PedDevice(s);
    if (device == NULL) {
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

    ped_device_destroy(device);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_device_check(PyObject *s, PyObject *args) {
    PyObject *in_buf = NULL;
    PedSector start, count, ret;
    PedDevice *device = NULL;
    void *out_buf = NULL;

    if (!PyArg_ParseTuple(args, "Oll", &in_buf, &start, &count)) {
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

    ret = ped_device_check(device, out_buf, start, count);
    ped_device_destroy(device);

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

    ped_device_destroy(device);
    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}

/* vim:tw=78:ts=4:et:sw=4
 */
