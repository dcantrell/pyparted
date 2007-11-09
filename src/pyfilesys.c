/*
 * pyfilesys.c
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
#include "pydevice.h"
#include "pyfilesys.h"
#include "pygeom.h"

/* _ped.FileSystemType functions */
void _ped_FileSystemType_dealloc(_ped_FileSystemType *self) {
    PyObject_Del(self);
}

PyObject *_ped_FileSystemType_new(PyTypeObject *type, PyObject *args,
                                  PyObject *kwds) {
    _ped_FileSystemType *self;

    self = PyObject_New(_ped_FileSystemType, &_ped_FileSystemType_Type_obj);
    return (PyObject *) self;
}

int _ped_FileSystemType_init(_ped_FileSystemType *self, PyObject *args,
                             PyObject *kwds) {
    /* XXX */
    return 0;
}

/* _ped.FileSystem functions */
void _ped_FileSystem_dealloc(_ped_FileSystem *self) {
    PyObject_Del(self);
}

PyObject *_ped_FileSystem_new(PyTypeObject *type, PyObject *args,
                              PyObject *kwds) {
    _ped_FileSystem *self;

    self = PyObject_New(_ped_FileSystem, &_ped_FileSystem_Type_obj);
    return (PyObject *) self;
}

int _ped_FileSystem_init(_ped_FileSystem *self, PyObject *args,
                         PyObject *kwds) {
    /* XXX */
    return 0;
}

/* 1:1 function mappings for filesys.h in libparted */
PyObject *py_ped_file_system_type_register(PyObject *s, PyObject *args) {
    PyObject *in_fstype;
    PedFileSystemType *out_fstype;

    if (!PyArg_ParseTuple(args, "O!", &_ped_FileSystemType_Type_obj,
                          &in_fstype)) {
        return NULL;
    }

    out_fstype = _ped_FileSystemType2PedFileSystemType(in_fstype);
    ped_file_system_type_register(out_fstype);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_file_system_type_unregister(PyObject *s, PyObject *args) {
    PyObject *in_fstype;
    PedFileSystemType *out_fstype;

    if (!PyArg_ParseTuple(args, "O!", &_ped_FileSystemType_Type_obj,
                          &in_fstype)) {
        return NULL;
    }

    out_fstype = _ped_FileSystemType2PedFileSystemType(in_fstype);
    ped_file_system_type_register(out_fstype);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_file_system_type_get(PyObject *s, PyObject *args) {
    PedFileSystemType *fstype;
    _ped_FileSystemType *ret;
    char *name;

    if (!PyArg_ParseTuple(args, "s", &name)) {
        return NULL;
    }

    fstype = ped_file_system_type_get(name);
    if (fstype) {
        ret = PedFileSystemType2_ped_FileSystemType(fstype);
        return (PyObject *) ret;
    }
    else {
        PyErr_SetString(UnknownTypeException, name);
        return NULL;
    }
}

PyObject *py_ped_file_system_type_get_next(PyObject *s, PyObject *args) {
    PyObject *in_fstype = NULL;
    PedFileSystemType *out_fstype, *fstype;
    _ped_FileSystemType *ret;

    if (!PyArg_ParseTuple(args, "|O!", &_ped_FileSystemType_Type_obj,
                          &in_fstype)) {
        return NULL;
    }

    if (in_fstype == Py_None)
        out_fstype = NULL;
    else
        out_fstype = _ped_FileSystemType2PedFileSystemType(in_fstype);

    fstype = ped_file_system_type_get_next(out_fstype);
    if (fstype) {
        ret = PedFileSystemType2_ped_FileSystemType(fstype);
        return (PyObject *) ret;
    }
    else {
        PyErr_SetString(PyExc_IndexError, NULL);
        return NULL;
    }

}

PyObject *py_ped_file_system_probe_specific(PyObject *s, PyObject *args) {
    PyObject *in_fstype, *in_geom;
    PedFileSystemType *out_fstype;
    PedGeometry *out_geom, *geom;
    _ped_Geometry *ret;

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_FileSystemType_Type_obj,
                          &in_fstype, &_ped_Geometry_Type_obj, &in_geom)) {
        return NULL;
    }

    out_fstype = _ped_FileSystemType2PedFileSystemType(in_fstype);
    out_geom = _ped_Geometry2PedGeometry(in_geom);

    geom = ped_file_system_probe_specific(out_fstype, out_geom);
    if (geom) {
        ret = PedGeometry2_ped_Geometry(geom);
    }

    ped_geometry_destroy(out_geom);
    ped_geometry_destroy(geom);

    return (PyObject *) ret;
}

PyObject *py_ped_file_system_probe(PyObject *s, PyObject *args) {
    PyObject *in_geom;
    PedGeometry *out_geom;
    PedFileSystemType *fstype;
    _ped_FileSystemType *ret;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Geometry_Type_obj, &in_geom)) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);

    fstype = ped_file_system_probe(out_geom);
    if (fstype) {
        ret = PedFileSystemType2_ped_FileSystemType(fstype);
    }

    ped_geometry_destroy(out_geom);

    return (PyObject *) ret;
}

PyObject *py_ped_file_system_clobber(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_geom;
    PedGeometry *out_geom;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Geometry_Type_obj, &in_geom)) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    ret = ped_file_system_clobber(out_geom);
    ped_geometry_destroy(out_geom);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_file_system_open(PyObject *s, PyObject *args) {
    PyObject *in_geom;
    PedGeometry *out_geom;
    PedFileSystem *fs;
    _ped_FileSystem *ret;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Geometry_Type_obj, &in_geom)) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);

    fs = ped_file_system_open(out_geom);
    if (fs) {
        ret = PedFileSystem2_ped_FileSystem(fs);
    }

    ped_file_system_destroy(fs);
    ped_geometry_destroy(out_geom);

    return (PyObject *) ret;
}

PyObject *py_ped_file_system_create(PyObject *s, PyObject *args) {
    PyObject *in_geom, *in_fstype, *in_timer;
    PedGeometry *out_geom;
    PedFileSystemType *out_fstype;
    PedTimer *out_timer;
    PedFileSystem *fs;
    _ped_FileSystem *ret;

    if (!PyArg_ParseTuple(args, "O!O!O!", &_ped_Geometry_Type_obj, &in_geom,
                          &_ped_FileSystemType_Type_obj, &in_fstype,
                          &_ped_Timer_Type_obj, &in_timer)) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    out_fstype = _ped_FileSystemType2PedFileSystemType(in_fstype);
    out_timer = _ped_Timer2PedTimer(in_timer);

    fs = ped_file_system_create(out_geom, out_fstype, out_timer);
    if (fs) {
        ret = PedFileSystem2_ped_FileSystem(fs);
    }

    ped_file_system_destroy(fs);
    ped_geometry_destroy(out_geom);
    ped_timer_destroy(out_timer);

    return (PyObject *) ret;
}

PyObject *py_ped_file_system_close(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_fs;
    PedFileSystem *out_fs;

    if (!PyArg_ParseTuple(args, "O!", &_ped_FileSystem_Type_obj, &in_fs)) {
        return NULL;
    }

    out_fs = _ped_FileSystem2PedFileSystem(in_fs);
    ret = ped_file_system_close(out_fs);
    ped_file_system_destroy(out_fs);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_file_system_check(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_fs, *in_timer;
    PedFileSystem *out_fs;
    PedTimer *out_timer;

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_FileSystem_Type_obj, &in_fs,
                          &_ped_Timer_Type_obj, &in_timer)) {
        return NULL;
    }

    out_fs = _ped_FileSystem2PedFileSystem(in_fs);
    out_timer = _ped_Timer2PedTimer(in_timer);
    ret = ped_file_system_check(out_fs, out_timer);

    ped_file_system_destroy(out_fs);
    ped_timer_destroy(out_timer);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_file_system_copy(PyObject *s, PyObject *args) {
    PyObject *in_fs, *in_geom, *in_timer;
    PedFileSystem *out_fs;
    PedGeometry *out_geom;
    PedTimer *out_timer;
    PedFileSystem *fs;
    _ped_FileSystem *ret;

    if (!PyArg_ParseTuple(args, "O!O!O!", &_ped_FileSystem_Type_obj, &in_fs,
                          &_ped_Geometry_Type_obj, &in_geom,
                          &_ped_Timer_Type_obj, &in_timer)) {
        return NULL;
    }

    out_fs = _ped_FileSystem2PedFileSystem(in_fs);
    out_geom = _ped_Geometry2PedGeometry(in_geom);
    out_timer = _ped_Timer2PedTimer(in_timer);

    fs = ped_file_system_copy(out_fs, out_geom, out_timer);
    if (fs) {
        ret = PedFileSystem2_ped_FileSystem(fs);
    }

    ped_file_system_destroy(fs);
    ped_file_system_destroy(out_fs);
    ped_geometry_destroy(out_geom);
    ped_timer_destroy(out_timer);

    return (PyObject *) ret;
}

PyObject *py_ped_file_system_resize(PyObject *s, PyObject *args) {
    PyObject *in_fs, *in_geom, *in_timer;
    PedFileSystem *out_fs;
    PedGeometry *out_geom;
    PedTimer *out_timer;
    int ret = -1;

    if (!PyArg_ParseTuple(args, "O!O!O!", &_ped_FileSystem_Type_obj, &in_fs,
                          &_ped_Geometry_Type_obj, &in_geom,
                          &_ped_Timer_Type_obj, &in_timer)) {
        return NULL;
    }

    out_fs = _ped_FileSystem2PedFileSystem(in_fs);
    out_geom = _ped_Geometry2PedGeometry(in_geom);
    out_timer = _ped_Timer2PedTimer(in_timer);

    ret = ped_file_system_resize(out_fs, out_geom, out_timer);

    ped_file_system_destroy(out_fs);
    ped_geometry_destroy(out_geom);
    ped_timer_destroy(out_timer);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_file_system_get_create_constraint(PyObject *s,
                                                   PyObject *args) {
    PyObject *in_fstype, *in_device;
    PedFileSystemType *out_fstype;
    PedDevice *out_device;
    PedConstraint *constraint;
    _ped_Constraint *ret;

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_FileSystemType_Type_obj,
                          &in_fstype, &_ped_Device_Type_obj, &in_device)) {
        return NULL;
    }

    out_fstype = _ped_FileSystemType2PedFileSystemType(in_fstype);
    out_device = _ped_Device2PedDevice(in_device);

    constraint = ped_file_system_get_create_constraint(out_fstype, out_device);
    if (constraint) {
        ret = PedConstraint2_ped_Constraint(constraint);
    }

    ped_device_destroy(out_device);
    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}

PyObject *py_ped_file_system_get_resize_constraint(PyObject *s,
                                                   PyObject *args) {
    PyObject *in_fs;
    PedFileSystem *out_fs;
    PedConstraint *constraint;
    _ped_Constraint *ret;

    if (!PyArg_ParseTuple(args, "O!", &_ped_FileSystem_Type_obj, &in_fs)) {
        return NULL;
    }

    out_fs = _ped_FileSystem2PedFileSystem(in_fs);

    constraint = ped_file_system_get_resize_constraint(out_fs);
    if (constraint) {
        ret = PedConstraint2_ped_Constraint(constraint);
    }

    ped_file_system_destroy(out_fs);
    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}

PyObject *py_ped_file_system_get_copy_constraint(PyObject *s, PyObject *args) {
    PyObject *in_fs, *in_device;
    PedFileSystem *out_fs;
    PedDevice *out_device;
    PedConstraint *constraint;
    _ped_Constraint *ret;

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_FileSystem_Type_obj, &in_fs,
                          &_ped_Device_Type_obj, &in_device)) {
        return NULL;
    }

    out_fs = _ped_FileSystem2PedFileSystem(in_fs);
    out_device = _ped_Device2PedDevice(in_device);

    constraint = ped_file_system_get_copy_constraint(out_fs, out_device);
    if (constraint) {
        ret = PedConstraint2_ped_Constraint(constraint);
    }

    ped_file_system_destroy(out_fs);
    ped_device_destroy(out_device);
    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}
