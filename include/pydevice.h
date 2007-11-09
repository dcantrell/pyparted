/*
 * pydevice.h
 * pyparted type definitions for pydevice.c
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

#ifndef PYDEVICE_H_INCLUDED
#define PYDEVICE_H_INCLUDED

#include <Python.h>
#include <structmember.h>

#include <parted/parted.h>

/* _ped.CHSGeometry type is the Python equiv of PedCHSGeometry in libparted */
typedef struct {
    PyObject_HEAD

    /* a PedCHSGeometry stores three ints */
    int cylinders;
    int heads;
    int sectors;
} _ped_CHSGeometry;

static PyMemberDef _ped_CHSGeometry_members[] = {
    {NULL}
};

static PyMethodDef _ped_CHSGeometry_methods[] = {
    {NULL}
};

void _ped_CHSGeometry_dealloc(_ped_CHSGeometry *self);
PyObject *_ped_CHSGeometry_new(PyTypeObject *type, PyObject *args,
                               PyObject *kwds);
int _ped_CHSGeometry_init(_ped_CHSGeometry *self, PyObject *args,
                          PyObject *kwds);
PyObject *_ped_CHSGeometry_get(_ped_CHSGeometry *self, char *member);

static PyGetSetDef _ped_CHSGeometry_getset[] = {
    {"cylinders", (getter) _ped_CHSGeometry_get, NULL,
                  "CHSGeometry cylinders", NULL},
    {"heads", (getter) _ped_CHSGeometry_get, NULL,
                  "CHSGeometry heads", NULL},
    {"sectors", (getter) _ped_CHSGeometry_get, NULL,
                  "CHSGeometry sectors", NULL},
    {NULL}  /* Sentinel */
};

static PyTypeObject _ped_CHSGeometry_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    0,                                         /* ob_size */
    "_ped.CHSGeometry",                        /* tp_name */
    sizeof(_ped_CHSGeometry),                  /* tp_basicsize */
    0,                                         /* tp_itemsize */
    (destructor) _ped_CHSGeometry_dealloc,     /* tp_dealloc */
    0,                                         /* tp_print */
    0,                                         /* tp_getattr */
    0,                                         /* tp_setattr */
    0,                                         /* tp_compare */
    0,                                         /* tp_repr */
    0,                                         /* tp_as_number */
    0,                                         /* tp_as_sequence */
    0,                                         /* tp_as_mapping */
    0,                                         /* tp_hash */
    0,                                         /* tp_call */
    0,                                         /* tp_str */
    0,                                         /* tp_getattro */
    0,                                         /* tp_setattro */
    0,                                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,  /* tp_flags */
    "PedCHSGeometry objects",                  /* tp_doc */
    0,                                         /* tp_traverse */
    0,                                         /* tp_clear */
    0,                                         /* tp_richcompare */
    0,                                         /* tp_weaklistoffset */
    0,                                         /* tp_iter */
    0,                                         /* tp_iternext */
    _ped_CHSGeometry_methods,                  /* tp_methods */
    _ped_CHSGeometry_members,                  /* tp_members */
    _ped_CHSGeometry_getset,                   /* tp_getset */
    0,                                         /* tp_base */
    0,                                         /* tp_dict */
    0,                                         /* tp_descr_get */
    0,                                         /* tp_descr_set */
    0,                                         /* tp_dictoffset */
    (initproc) _ped_CHSGeometry_init,          /* tp_init */
    0,                                         /* tp_alloc */
    _ped_CHSGeometry_new,                      /* tp_new */
};

/* _ped.Device type is the Python equivalent of PedDevice in libparted */
typedef struct {
    PyObject_HEAD

    /* a PedDevice is complex, we will store primitives when appropriate or
     * just other Python objects we've created for the typedefs in libparted */
    char *model;
    char *path;
    long long type;
    long long sector_size;
    long long phys_sector_size;
    long long length;             /* PedSector */
    int open_count;
    int read_only;
    int external_mode;
    int dirty;
    int boot_dirty;
    PyObject *hw_geom;            /* a _ped.CHSGeometry */
    PyObject *bios_geom;          /* a _ped.CHSGeometry */
    short host;
    short did;
} _ped_Device;

static PyMemberDef _ped_Device_members[] = {
    {"hw_geom", T_OBJECT, offsetof(_ped_Device, hw_geom), 0, NULL},
    {"bios_geom", T_OBJECT, offsetof(_ped_Device, bios_geom), 0, NULL},
    {NULL}
};

static PyMethodDef _ped_Device_methods[] = {
    {NULL}
};

void _ped_Device_dealloc(_ped_Device *self);
PyObject *_ped_Device_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
int _ped_Device_init(_ped_Device *self, PyObject *args, PyObject *kwds);
PyObject *_ped_Device_get(_ped_Device *self, char *member);

static PyGetSetDef _ped_Device_getset[] = {
    {"model", (getter) _ped_Device_get, NULL,
              "PedDevice model", NULL},
    {"path", (getter) _ped_Device_get, NULL,
              "PedDevice path", NULL},
    {"type", (getter) _ped_Device_get, NULL,
              "PedDevice type", NULL},
    {"sector_size", (getter) _ped_Device_get, NULL,
              "PedDevice sector_size", NULL},
    {"phys_sector_size", (getter) _ped_Device_get, NULL,
              "PedDevice phys_sector_size", NULL},
    {"length", (getter) _ped_Device_get, NULL,
              "PedDevice length", NULL},
    {"open_count", (getter) _ped_Device_get, NULL,
              "PedDevice open_count", NULL},
    {"read_only", (getter) _ped_Device_get, NULL,
              "PedDevice read_only", NULL},
    {"external_mode", (getter) _ped_Device_get, NULL,
              "PedDevice external_mode", NULL},
    {"dirty", (getter) _ped_Device_get, NULL,
              "PedDevice dirty", NULL},
    {"boot_dirty", (getter) _ped_Device_get, NULL,
              "PedDevice boot_dirty", NULL},
    {"host", (getter) _ped_Device_get, NULL,
              "PedDevice host", NULL},
    {"did", (getter) _ped_Device_get, NULL,
              "PedDevice did", NULL},
    {NULL}  /* Sentinel */
};

static PyTypeObject _ped_Device_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    0,                                         /* ob_size */
    "_ped.Device",                             /* tp_name */
    sizeof(_ped_Device),                       /* tp_basicsize */
    0,                                         /* tp_itemsize */
    (destructor) _ped_Device_dealloc,          /* tp_dealloc */
    0,                                         /* tp_print */
    0,                                         /* tp_getattr */
    0,                                         /* tp_setattr */
    0,                                         /* tp_compare */
    0,                                         /* tp_repr */
    0,                                         /* tp_as_number */
    0,                                         /* tp_as_sequence */
    0,                                         /* tp_as_mapping */
    0,                                         /* tp_hash */
    0,                                         /* tp_call */
    0,                                         /* tp_str */
    0,                                         /* tp_getattro */
    0,                                         /* tp_setattro */
    0,                                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,  /* tp_flags */
    "PedDevice objects",                       /* tp_doc */
    0,                                         /* tp_traverse */
    0,                                         /* tp_clear */
    0,                                         /* tp_richcompare */
    0,                                         /* tp_weaklistoffset */
    0,                                         /* tp_iter */
    0,                                         /* tp_iternext */
    _ped_Device_methods,                       /* tp_methods */
    _ped_Device_members,                       /* tp_members */
    _ped_Device_getset,                        /* tp_getset */
    0,                                         /* tp_base */
    0,                                         /* tp_dict */
    0,                                         /* tp_descr_get */
    0,                                         /* tp_descr_set */
    0,                                         /* tp_dictoffset */
    (initproc) _ped_Device_init,               /* tp_init */
    0,                                         /* tp_alloc */
    _ped_Device_new,                           /* tp_new */
};

/* 1:1 function mappings for device.h in libparted */
PyObject *py_ped_device_probe_all(PyObject *s, PyObject *args);
PyObject *py_ped_device_free_all(PyObject *s, PyObject *args);
PyObject *py_ped_device_get(PyObject *s, PyObject *args);
PyObject *py_ped_device_get_next(PyObject *s, PyObject *args);
PyObject *py_ped_device_is_busy(PyObject *s, PyObject *args);
PyObject *py_ped_device_open(PyObject *s, PyObject *args);
PyObject *py_ped_device_close(PyObject *s, PyObject *args);
PyObject *py_ped_device_destroy(PyObject *s, PyObject *args);
PyObject *py_ped_device_cache_remove(PyObject *s, PyObject *args);
PyObject *py_ped_device_begin_external_access(PyObject *s, PyObject *args);
PyObject *py_ped_device_end_external_access(PyObject *s, PyObject *args);
PyObject *py_ped_device_read(PyObject *s, PyObject *args);
PyObject *py_ped_device_write(PyObject *s, PyObject *args);
PyObject *py_ped_device_sync(PyObject *s, PyObject *args);
PyObject *py_ped_device_sync_fast(PyObject *s, PyObject *args);
PyObject *py_ped_device_check(PyObject *s, PyObject *args);
PyObject *py_ped_device_get_constraint(PyObject *s, PyObject *args);

#endif /* PYDEVICE_H_INCLUDED */
