/*
 * pyfilesys.c
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
#include "pydevice.h"
#include "pyfilesys.h"
#include "pygeom.h"

/* _ped.FileSystemType functions */
void _ped_FileSystemType_dealloc(_ped_FileSystemType *self) {
    PyObject_Del(self);
}

PyObject *_ped_FileSystemType_new(PyTypeObject *type, PyObject *args,
                                  PyObject *kwds) {
    _ped_FileSystemType *self = NULL;

    self = PyObject_New(_ped_FileSystemType, &_ped_FileSystemType_Type_obj);
    return (PyObject *) self;
}

int _ped_FileSystemType_init(_ped_FileSystemType *self, PyObject *args,
                             PyObject *kwds) {
    static char *kwlist[] = {"name", NULL};

    self->name = NULL;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|z", kwlist, &self->name))
        return -1;
    else
        return 0;
}

PyObject *_ped_FileSystemType_get(_ped_FileSystemType *self, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.FileSystemType()");
        return NULL;
    }

    if (!strcmp(member, "name")) {
        return PyString_FromString(self->name);
    } else {
        PyErr_Format(PyExc_AttributeError, "_ped.FileSystemType object has no attribute %s", member);
        return NULL;
    }
}

int _ped_FileSystemType_set(_ped_FileSystemType *self, PyObject *value,
                            void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        return NULL;
    }

    if (!strcmp(member, "name")) {
        self->name = PyString_AsString(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else {
        return -1;
    }

    return 0;
}

/* _ped.FileSystem functions */
void _ped_FileSystem_dealloc(_ped_FileSystem *self) {
    PyObject_Del(self);
}

PyObject *_ped_FileSystem_new(PyTypeObject *type, PyObject *args,
                              PyObject *kwds) {
    _ped_FileSystem *self = NULL;

    self = PyObject_New(_ped_FileSystem, &_ped_FileSystem_Type_obj);
    return (PyObject *) self;
}

int _ped_FileSystem_init(_ped_FileSystem *self, PyObject *args,
                         PyObject *kwds) {
    static char *kwlist[] = {"type", "geom", "checked", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O!O!l", kwlist,
                                     &_ped_FileSystemType_Type_obj, &self->type,
                                     &_ped_Geometry_Type_obj, &self->geom,
                                     &self->checked))
        return -1;
    else
        return 0;
}

PyObject *_ped_FileSystem_get(_ped_FileSystem *self, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.FileSystem()");
        return NULL;
    }

    if (!strcmp(member, "checked")) {
        return PyInt_FromLong(self->checked);
    } else {
        PyErr_Format(PyExc_AttributeError, "_ped.FileSystem object has no attribute %s", member);
        return NULL;
    }
}

int _ped_FileSystem_set(_ped_FileSystem *self, PyObject *value, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        return NULL;
    }

    if (!strcmp(member, "checked")) {
        self->checked = PyInt_AsLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else {
        return -1;
    }

    return 0;
}

/* 1:1 function mappings for filesys.h in libparted */
PyObject *py_ped_file_system_type_register(PyObject *s, PyObject *args) {
    PyObject *in_fstype = NULL;
    PedFileSystemType *out_fstype = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_FileSystemType_Type_obj,
                          &in_fstype)) {
        return NULL;
    }

    out_fstype = _ped_FileSystemType2PedFileSystemType(in_fstype);
    if (!out_fstype) {
        return NULL;
    }

    ped_file_system_type_register(out_fstype);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_file_system_type_unregister(PyObject *s, PyObject *args) {
    PyObject *in_fstype = NULL;
    PedFileSystemType *out_fstype = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_FileSystemType_Type_obj,
                          &in_fstype)) {
        return NULL;
    }

    out_fstype = _ped_FileSystemType2PedFileSystemType(in_fstype);
    if (!out_fstype) {
        return NULL;
    }

    ped_file_system_type_unregister(out_fstype);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_file_system_type_get(PyObject *s, PyObject *args) {
    PedFileSystemType *fstype = NULL;
    _ped_FileSystemType *ret = NULL;
    char *name = NULL;

    if (!PyArg_ParseTuple(args, "z", &name)) {
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
    PedFileSystemType *out_fstype = NULL, *fstype = NULL;
    _ped_FileSystemType *ret = NULL;

    if (!PyArg_ParseTuple(args, "|O!", &_ped_FileSystemType_Type_obj,
                          &in_fstype)) {
        return NULL;
    }

    if (!in_fstype)
        out_fstype = NULL;
    else {
        out_fstype = _ped_FileSystemType2PedFileSystemType(in_fstype);
        if (!out_fstype) {
            return NULL;
        }
    }

    fstype = ped_file_system_type_get_next(out_fstype);
    if (fstype) {
        ret = PedFileSystemType2_ped_FileSystemType(fstype);
        return (PyObject *) ret;
    }
    else {
        PyErr_SetNone(PyExc_IndexError);
        return NULL;
    }

}

PyObject *py_ped_file_system_probe_specific(PyObject *s, PyObject *args) {
    PyObject *in_fstype = NULL, *in_geom = NULL;
    PedFileSystemType *out_fstype = NULL;
    PedGeometry *out_geom = NULL, *geom = NULL;
    _ped_Geometry *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_FileSystemType_Type_obj,
                          &in_fstype, &_ped_Geometry_Type_obj, &in_geom)) {
        return NULL;
    }

    out_fstype = _ped_FileSystemType2PedFileSystemType(in_fstype);
    if (!out_fstype) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (!out_geom) {
        return NULL;
    }

    geom = ped_file_system_probe_specific(out_fstype, out_geom);
    if (geom) {
        ret = PedGeometry2_ped_Geometry(geom);
    }
    else {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_Format(FileSystemException, "Failed to probe filesystem type %s", out_fstype->name);

        return NULL;
    }

    ped_geometry_destroy(out_geom);
    ped_geometry_destroy(geom);

    return (PyObject *) ret;
}

PyObject *py_ped_file_system_probe(PyObject *s, PyObject *args) {
    PyObject *in_geom = NULL;
    PedGeometry *out_geom = NULL;
    PedFileSystemType *fstype = NULL;
    _ped_FileSystemType *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Geometry_Type_obj, &in_geom)) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (!out_geom) {
        return NULL;
    }

    fstype = ped_file_system_probe(out_geom);
    if (fstype) {
        ret = PedFileSystemType2_ped_FileSystemType(fstype);
    }
    else {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_SetString(FileSystemException, "Failed to find any filesystem in given geometry");

        return NULL;
    }

    ped_geometry_destroy(out_geom);

    return (PyObject *) ret;
}

PyObject *py_ped_file_system_clobber(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_geom = NULL;
    PedGeometry *out_geom = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Geometry_Type_obj, &in_geom)) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (!out_geom) {
        return NULL;
    }

    ret = ped_file_system_clobber(out_geom);

    if (!ret) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_SetString(FileSystemException, "Failed to clobber any filesystem in given geometry");

        return NULL;
    }

    ped_geometry_destroy(out_geom);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_file_system_open(PyObject *s, PyObject *args) {
    PyObject *in_geom = NULL;
    PedGeometry *out_geom = NULL;
    PedFileSystem *fs = NULL;
    _ped_FileSystem *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Geometry_Type_obj, &in_geom)) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (!out_geom) {
        return NULL;
    }

    fs = ped_file_system_open(out_geom);
    if (fs) {
        ret = PedFileSystem2_ped_FileSystem(fs);
    }
    else {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PyExc_NotImplementedError) &&
                !PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(FileSystemException, partedExnMessage);
        }
        else
            PyErr_SetString(FileSystemException, "Failed to open any filesystem in given geometry");

        return NULL;
    }

    ped_file_system_destroy(fs);
    ped_geometry_destroy(out_geom);

    return (PyObject *) ret;
}

PyObject *py_ped_file_system_create(PyObject *s, PyObject *args) {
    PyObject *in_geom = NULL, *in_fstype = NULL, *in_timer = NULL;
    PedGeometry *out_geom = NULL;
    PedFileSystemType *out_fstype = NULL;
    PedTimer *out_timer = NULL;
    PedFileSystem *fs = NULL;
    _ped_FileSystem *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!O!|O!", &_ped_Geometry_Type_obj, &in_geom,
                          &_ped_FileSystemType_Type_obj, &in_fstype,
                          &_ped_Timer_Type_obj, &in_timer)) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (!out_geom) {
        return NULL;
    }

    out_fstype = _ped_FileSystemType2PedFileSystemType(in_fstype);
    if (!out_fstype) {
        return NULL;
    }

    if (in_timer) {
        out_timer = _ped_Timer2PedTimer(in_timer);
        if (!out_timer) {
            return NULL;
        }
    }
    else
       out_timer = NULL;

    fs = ped_file_system_create(out_geom, out_fstype, out_timer);
    if (fs) {
        ret = PedFileSystem2_ped_FileSystem(fs);
    }
    else {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PyExc_NotImplementedError) &&
                !PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(FileSystemException, partedExnMessage);
        }
        else
            PyErr_Format(FileSystemException, "Failed to create filesystem type %s", out_fstype->name);

        return NULL;
    }

    ped_file_system_destroy(fs);
    ped_geometry_destroy(out_geom);
    ped_timer_destroy(out_timer);

    return (PyObject *) ret;
}

PyObject *py_ped_file_system_close(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_fs = NULL;
    PedFileSystem *out_fs = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_FileSystem_Type_obj, &in_fs)) {
        return NULL;
    }

    out_fs = _ped_FileSystem2PedFileSystem(in_fs);
    if (!out_fs) {
        return NULL;
    }

    ret = ped_file_system_close(out_fs);

    if (!ret) {
        PyErr_Format(FileSystemException, "Failed to close filesystem type %s", out_fs->type->name);
        return NULL;
    }

    ped_file_system_destroy(out_fs);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_file_system_check(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_fs = NULL, *in_timer = NULL;
    PedFileSystem *out_fs = NULL;
    PedTimer *out_timer = NULL;

    if (!PyArg_ParseTuple(args, "O!|O!", &_ped_FileSystem_Type_obj, &in_fs,
                          &_ped_Timer_Type_obj, &in_timer)) {
        return NULL;
    }

    out_fs = _ped_FileSystem2PedFileSystem(in_fs);
    if (!out_fs) {
        return NULL;
    }

    if (in_timer) {
        out_timer = _ped_Timer2PedTimer(in_timer);
        if (!out_timer) {
            return NULL;
        }
    }
    else
        out_timer = NULL;

    ret = ped_file_system_check(out_fs, out_timer);

    /* NotImplementedError may have been raised if it's an unsupported
     * operation for this filesystem.  Otherwise, we shouldn't get any
     * exceptions here.
     */
    if (!ret && partedExnRaised) {
        partedExnRaised = 0;
        return NULL;
    }

    ped_file_system_destroy(out_fs);
    ped_timer_destroy(out_timer);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_file_system_copy(PyObject *s, PyObject *args) {
    PyObject *in_fs = NULL, *in_geom = NULL, *in_timer = NULL;
    PedFileSystem *out_fs = NULL;
    PedGeometry *out_geom = NULL;
    PedTimer *out_timer = NULL;
    PedFileSystem *fs = NULL;
    _ped_FileSystem *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!O!|O!", &_ped_FileSystem_Type_obj, &in_fs,
                          &_ped_Geometry_Type_obj, &in_geom,
                          &_ped_Timer_Type_obj, &in_timer)) {
        return NULL;
    }

    out_fs = _ped_FileSystem2PedFileSystem(in_fs);
    if (!out_fs) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (!out_geom) {
        return NULL;
    }

    if (in_timer) {
        out_timer = _ped_Timer2PedTimer(in_timer);
        if (!out_timer) {
            return NULL;
        }
    }
    else
        out_timer = NULL;

    fs = ped_file_system_copy(out_fs, out_geom, out_timer);
    if (fs) {
        ret = PedFileSystem2_ped_FileSystem(fs);
    }
    else {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PyExc_NotImplementedError) &&
                !PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(FileSystemException, partedExnMessage);
        }
        else
            PyErr_Format(FileSystemException, "Failed to copy filesystem type %s", out_fs->type->name);

        return NULL;
    }

    ped_file_system_destroy(fs);
    ped_file_system_destroy(out_fs);
    ped_geometry_destroy(out_geom);
    ped_timer_destroy(out_timer);

    return (PyObject *) ret;
}

PyObject *py_ped_file_system_resize(PyObject *s, PyObject *args) {
    PyObject *in_fs = NULL, *in_geom = NULL, *in_timer = NULL;
    PedFileSystem *out_fs = NULL;
    PedGeometry *out_geom = NULL;
    PedTimer *out_timer = NULL;
    int ret = -1;

    if (!PyArg_ParseTuple(args, "O!O!|O!", &_ped_FileSystem_Type_obj, &in_fs,
                          &_ped_Geometry_Type_obj, &in_geom,
                          &_ped_Timer_Type_obj, &in_timer)) {
        return NULL;
    }

    out_fs = _ped_FileSystem2PedFileSystem(in_fs);
    if (!out_fs) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (!out_geom) {
        return NULL;
    }

    if (in_timer) {
        out_timer = _ped_Timer2PedTimer(in_timer);
        if (!out_timer) {
            return NULL;
        }
    }
    else
        out_timer = NULL;

    ret = ped_file_system_resize(out_fs, out_geom, out_timer);

    if (!ret) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PyExc_NotImplementedError) &&
                !PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(FileSystemException, partedExnMessage);
        }
        else
            PyErr_Format(FileSystemException, "Failed to resize filesystem type %s", out_fs->type->name);

        return NULL;
    }

    ped_file_system_destroy(out_fs);
    ped_geometry_destroy(out_geom);
    ped_timer_destroy(out_timer);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_file_system_get_create_constraint(PyObject *s,
                                                   PyObject *args) {
    PyObject *in_fstype = NULL, *in_device = NULL;
    PedFileSystemType *out_fstype = NULL;
    PedDevice *out_device = NULL;
    PedConstraint *constraint = NULL;
    _ped_Constraint *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_FileSystemType_Type_obj,
                          &in_fstype, &_ped_Device_Type_obj, &in_device)) {
        return NULL;
    }

    out_fstype = _ped_FileSystemType2PedFileSystemType(in_fstype);
    if (!out_fstype) {
        return NULL;
    }

    out_device = _ped_Device2PedDevice(in_device);
    if (!out_device) {
        return NULL;
    }

    constraint = ped_file_system_get_create_constraint(out_fstype, out_device);
    if (constraint) {
        ret = PedConstraint2_ped_Constraint(constraint);
    }
    else {
        PyErr_Format(CreateException, "Failed to create constraint for filesystem type %s", out_fstype->name);
        return NULL;
    }

    ped_device_destroy(out_device);
    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}

PyObject *py_ped_file_system_get_resize_constraint(PyObject *s,
                                                   PyObject *args) {
    PyObject *in_fs = NULL;
    PedFileSystem *out_fs = NULL;
    PedConstraint *constraint = NULL;
    _ped_Constraint *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_FileSystem_Type_obj, &in_fs)) {
        return NULL;
    }

    out_fs = _ped_FileSystem2PedFileSystem(in_fs);
    if (!out_fs) {
        return NULL;
    }

    constraint = ped_file_system_get_resize_constraint(out_fs);
    if (constraint) {
        ret = PedConstraint2_ped_Constraint(constraint);
    }
    else {
        PyErr_Format(CreateException, "Failed to create resize constraint for filesystem type %s", out_fs->type->name);
        return NULL;
    }

    ped_file_system_destroy(out_fs);
    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}

PyObject *py_ped_file_system_get_copy_constraint(PyObject *s, PyObject *args) {
    PyObject *in_fs = NULL, *in_device = NULL;
    PedFileSystem *out_fs = NULL;
    PedDevice *out_device = NULL;
    PedConstraint *constraint = NULL;
    _ped_Constraint *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_FileSystem_Type_obj, &in_fs,
                          &_ped_Device_Type_obj, &in_device)) {
        return NULL;
    }

    out_fs = _ped_FileSystem2PedFileSystem(in_fs);
    if (!out_fs) {
        return NULL;
    }

    out_device = _ped_Device2PedDevice(in_device);
    if (!out_device) {
        return NULL;
    }

    constraint = ped_file_system_get_copy_constraint(out_fs, out_device);
    if (constraint) {
        ret = PedConstraint2_ped_Constraint(constraint);
    }
    else {
        PyErr_Format(CreateException, "Failed to create copy constraint for filesystem type %s", out_fs->type->name);
        return NULL;
    }

    ped_file_system_destroy(out_fs);
    ped_device_destroy(out_device);
    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}
