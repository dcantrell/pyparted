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
 * Red Hat Author(s): David Cantrell
 */

#ifndef PYDEVICE_H_INCLUDED
#define PYDEVICE_H_INCLUDED

#include <Python.h>
#include <structmember.h>

#include <parted/parted.h>

/* _ped.Sector type is the Python equivalent of PedSector in libparted */
typedef struct {
    PyObject_HEAD

    /* a PedSector is a long long, so we'll just store a long long here */
    long long val;
} _ped_Sector;

static PyMemberDef _ped_Sector_members[] = {
    {"val", T_LONGLONG, offsetof(_ped_Sector, val), 0, "PedSector value"},
    {NULL}
};

static PyMethodDef _ped_Sector_methods[] = {
    {NULL}
};

void _ped_Sector_dealloc(_ped_Sector *self);
PyObject *_ped_Sector_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
int _ped_Sector_init(_ped_Sector *self, PyObject *args, PyObject *kwds);
PyObject *_ped_Sector_getval(_ped_Sector *self, void *closure);
int _ped_Sector_setval(_ped_Sector *self, PyObject *value, void *closure);

static PyGetSetDef _ped_Sector_getset[] = {
    {"val", (getter) _ped_Sector_getval, (setter) _ped_Sector_setval,
     "PedSector value", NULL},
    {NULL}  /* Sentinel */
};

static PyTypeObject _ped_SectorType = {
    PyObject_HEAD_INIT(NULL)
    0,                                         /* ob_size */
    "_ped.Sector",                             /* tp_name */
    sizeof(_ped_Sector),                       /* tp_basicsize */
    0,                                         /* tp_itemsize */
    (destructor) _ped_Sector_dealloc,          /* tp_dealloc */
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
    "PedSector objects",                       /* tp_doc */
    0,                                         /* tp_traverse */
    0,                                         /* tp_clear */
    0,                                         /* tp_richcompare */
    0,                                         /* tp_weaklistoffset */
    0,                                         /* tp_iter */
    0,                                         /* tp_iternext */
    _ped_Sector_methods,                       /* tp_methods */
    _ped_Sector_members,                       /* tp_members */
    _ped_Sector_getset,                        /* tp_getset */
    0,                                         /* tp_base */
    0,                                         /* tp_dict */
    0,                                         /* tp_descr_get */
    0,                                         /* tp_descr_set */
    0,                                         /* tp_dictoffset */
    (initproc) _ped_Sector_init,               /* tp_init */
    0,                                         /* tp_alloc */
    _ped_Sector_new,                           /* tp_new */
};

/* _ped.CHSGeometry type is the Python equiv of PedCHSGeometry in libparted */
typedef struct {
    PyObject_HEAD

    /* a PedCHSGeometry stores three ints */
    int cylinders;
    int heads;
    int sectors;
} _ped_CHSGeometry;

static PyMemberDef _ped_CHSGeometry_members[] = {
    {"cylinders", T_INT, offsetof(_ped_CHSGeometry, cylinders), 0,
     "PedCHSGeometry cylinders value"},
    {"heads", T_INT, offsetof(_ped_CHSGeometry, heads), 0,
     "PedCHSGeometry heads value"},
    {"sectors", T_INT, offsetof(_ped_CHSGeometry, sectors), 0,
     "PedCHSGeometry sectors value"},
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
PyObject *_ped_CHSGeometry_getcylinders(_ped_CHSGeometry *self, void *closure);
int _ped_CHSGeometry_setcylinders(_ped_CHSGeometry *self, PyObject *value,
                                  void *closure);
PyObject *_ped_CHSGeometry_getheads(_ped_CHSGeometry *self, void *closure);
int _ped_CHSGeometry_setheads(_ped_CHSGeometry *self, PyObject *value,
                              void *closure);
PyObject *_ped_CHSGeometry_getsectors(_ped_CHSGeometry *self, void *closure);
int _ped_CHSGeometry_setsectors(_ped_CHSGeometry *self, PyObject *value,
                                void *closure);

static PyGetSetDef _ped_CHSGeometry_getset[] = {
    {"cylinders", (getter) _ped_CHSGeometry_getcylinders,
                  (setter) _ped_CHSGeometry_setcylinders,
                  "PedCHSGeometry cylinders value", NULL},
    {"heads", (getter) _ped_CHSGeometry_getheads,
              (setter) _ped_CHSGeometry_setheads,
              "PedCHSGeometry heads value", NULL},
    {"sectors", (getter) _ped_CHSGeometry_getsectors,
                (setter) _ped_CHSGeometry_setsectors,
                "PedCHSGeometry sectors value", NULL},
    {NULL}  /* Sentinel */
};

static PyTypeObject _ped_CHSGeometryType = {
    PyObject_HEAD_INIT(NULL)
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

/* 1:1 function mappings for device.h in libparted */

#endif /* PYDEVICE_H_INCLUDED */
