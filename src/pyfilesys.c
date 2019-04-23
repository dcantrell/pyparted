/*
 * pyfilesys.c
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
#include "pyfilesys.h"
#include "pygeom.h"
#include "docstrings/pyfilesys.h"
#include "typeobjects/pyfilesys.h"

/* _ped.FileSystemType functions */
void _ped_FileSystemType_dealloc(_ped_FileSystemType *self) {
    PyObject_GC_UnTrack(self);
    free(self->name);
    PyObject_GC_Del(self);
}

int _ped_FileSystemType_compare(_ped_FileSystemType *self, PyObject *obj) {
    _ped_FileSystemType *comp = NULL;
    int check = PyObject_IsInstance(obj, (PyObject *) &_ped_FileSystemType_Type_obj);

    if (PyErr_Occurred()) {
        return -1;
    }

    if (!check) {
        PyErr_SetString(PyExc_ValueError, "object comparing to must be a _ped.FileSystemType");
        return -1;
    }

    comp = (_ped_FileSystemType *) obj;
    if (!strcmp(self->name, comp->name)) {
        return 0;
    } else {
        return 1;
    }
}

PyObject *_ped_FileSystemType_richcompare(_ped_FileSystemType *a, PyObject *b,
                                          int op) {
    if (op == Py_EQ || op == Py_NE) {
        int rv = _ped_FileSystemType_compare(a, b);
        if (PyErr_Occurred())
            return NULL;
        return PyBool_FromLong(op == Py_EQ ? rv == 0 : rv != 0);
    } else if ((op == Py_LT) || (op == Py_LE) ||
               (op == Py_GT) || (op == Py_GE)) {
        PyErr_SetString(PyExc_TypeError, "comparison operator not supported for _ped.FileSystemType");
        return NULL;
    } else {
        PyErr_SetString(PyExc_ValueError, "unknown richcompare op");
        return NULL;
    }
}

PyObject *_ped_FileSystemType_str(_ped_FileSystemType *self) {
    char *ret = NULL;

    if (asprintf(&ret, "_ped.FileSystemType instance --\n"
                       "  name: %s",
                 self->name) == -1) {
        return PyErr_NoMemory();
    }

    return Py_BuildValue("s", ret);
}

int _ped_FileSystemType_traverse(_ped_FileSystemType *self, visitproc visit, void *arg) {
    return 0;
}

int _ped_FileSystemType_clear(_ped_FileSystemType *self) {
    return 0;
}

PyObject *_ped_FileSystemType_get(_ped_FileSystemType *self, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.FileSystemType()");
        return NULL;
    }

    if (!strcmp(member, "name")) {
        if (self->name != NULL)
            return PyUnicode_FromString(self->name);
        else
            return PyUnicode_FromString("");
    } else {
        PyErr_Format(PyExc_AttributeError, "_ped.FileSystemType object has no attribute %s", member);
        return NULL;
    }
}

/* _ped.FileSystem functions */
void _ped_FileSystem_dealloc(_ped_FileSystem *self) {
    PyObject_GC_UnTrack(self);

    Py_CLEAR(self->type);
    self->type = NULL;

    Py_CLEAR(self->geom);
    self->geom = NULL;

    PyObject_GC_Del(self);
}

int _ped_FileSystem_compare(_ped_FileSystem *self, PyObject *obj) {
    _ped_FileSystem *comp = NULL;
    int check = PyObject_IsInstance(obj, (PyObject *) &_ped_FileSystem_Type_obj);

    if (PyErr_Occurred()) {
        return -1;
    }

    if (!check) {
        PyErr_SetString(PyExc_ValueError, "object comparing to must be a _ped.FileSystem");
        return -1;
    }

    comp = (_ped_FileSystem *) obj;
    if ((_ped_FileSystemType_Type_obj.tp_richcompare(self->type, comp->type, Py_EQ)) &&
        (_ped_Geometry_Type_obj.tp_richcompare(self->geom, comp->geom, Py_EQ)) &&
        (self->checked == comp->checked)) {
        return 0;
    } else {
        return 1;
    }

}

PyObject *_ped_FileSystem_richcompare(_ped_FileSystem *a, PyObject *b, int op) {
    if (op == Py_EQ || op == Py_NE) {
        int rv = _ped_FileSystem_compare(a, b);
        if (PyErr_Occurred())
            return NULL;
        return PyBool_FromLong(op == Py_EQ ? rv == 0 : rv != 0);
    } else if ((op == Py_LT) || (op == Py_LE) ||
               (op == Py_GT) || (op == Py_GE)) {
        PyErr_SetString(PyExc_TypeError, "comparison operator not supported for _ped.FileSystem");
        return NULL;
    } else {
        PyErr_SetString(PyExc_ValueError, "unknown richcompare op");
        return NULL;
    }
}

PyObject *_ped_FileSystem_str(_ped_FileSystem *self) {
    char *ret = NULL;
    char *type = NULL, *geom = NULL;

    type = (char *) PyUnicode_AsUTF8(_ped_FileSystem_Type_obj.tp_repr(self->type));
    if (type == NULL) {
        return NULL;
    }

    geom = (char *) PyUnicode_AsUTF8(_ped_Geometry_Type_obj.tp_repr(self->geom));
    if (geom == NULL) {
        return NULL;
    }

    if (asprintf(&ret, "_ped.FileSystem instance --\n"
                       "  type: %s  geom: %s\n"
                       "  checked: %d",
                 type, geom, self->checked) == -1) {
        return PyErr_NoMemory();
    }

    return Py_BuildValue("s", ret);
}

int _ped_FileSystem_traverse(_ped_FileSystem *self, visitproc visit, void *arg) {
    int err;

    if (self->type) {
        if ((err = visit(self->type, arg))) {
            return err;
        }
    }

    if (self->geom) {
        if ((err = visit(self->geom, arg))) {
            return err;
        }
    }

    return 0;
}

int _ped_FileSystem_clear(_ped_FileSystem *self) {
    Py_CLEAR(self->type);
    self->type = NULL;

    Py_CLEAR(self->geom);
    self->geom = NULL;

    return 0;
}

int _ped_FileSystem_init(_ped_FileSystem *self, PyObject *args,
                         PyObject *kwds) {
    static char *kwlist[] = {"type", "geom", "checked", NULL};

    self->checked = 0;

    if (kwds == NULL) {
        if (!PyArg_ParseTuple(args, "O!O!|i",
                              &_ped_FileSystemType_Type_obj, &self->type,
                              &_ped_Geometry_Type_obj, &self->geom,
                              &self->checked)) {
            self->type = self->geom = NULL;
            return -1;
        }
    } else {
        if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!O!|i", kwlist,
                                         &_ped_FileSystemType_Type_obj, &self->type,
                                         &_ped_Geometry_Type_obj, &self->geom,
                                         &self->checked)) {
            self->type = self->geom = NULL;
            return -2;
        }
    }

    Py_INCREF(self->type);
    Py_INCREF(self->geom);
    self->ped_filesystem = NULL;
    return 0;
}

PyObject *_ped_FileSystem_get(_ped_FileSystem *self, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.FileSystem()");
        return NULL;
    }

    if (!strcmp(member, "checked")) {
        return PyLong_FromLong(self->checked);
    } else {
        PyErr_Format(PyExc_AttributeError, "_ped.FileSystem object has no attribute %s", member);
        return NULL;
    }
}

/* 1:1 function mappings for filesys.h in libparted */
PyObject *py_ped_file_system_type_get(PyObject *s, PyObject *args) {
    PedFileSystemType *fstype = NULL;
    _ped_FileSystemType *ret = NULL;
    char *name = NULL;

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
    PedFileSystemType *cur = NULL, *next = NULL;
    _ped_FileSystemType *ret = NULL;

    if (!PyArg_ParseTuple(args, "|O!", &_ped_FileSystemType_Type_obj, &in_fstype)) {
        return NULL;
    }

    if (in_fstype) {
        cur = _ped_FileSystemType2PedFileSystemType(in_fstype);
        if (!cur) {
            return NULL;
        }
    }

    next = ped_file_system_type_get_next(cur);
    if (next) {
        ret = PedFileSystemType2_ped_FileSystemType(next);
        return (PyObject *) ret;
    } else {
        PyErr_SetNone(PyExc_IndexError);
        return NULL;
    }
}

PyObject *py_ped_file_system_probe_specific(PyObject *s, PyObject *args) {
    PyObject *in_geom = NULL, *in_fstype = NULL;
    PedFileSystemType *fstype = NULL;
    PedGeometry *out_geom = NULL, *geom = NULL;
    _ped_Geometry *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_FileSystemType_Type_obj,
                          &in_fstype, &_ped_Geometry_Type_obj, &in_geom)) {
        return NULL;
    }

    fstype = _ped_FileSystemType2PedFileSystemType(in_fstype);
    if (!fstype) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (!out_geom) {
        return NULL;
    }

    geom = ped_file_system_probe_specific(fstype, out_geom);
    if (geom) {
        ret = PedGeometry2_ped_Geometry(geom);
    } else {
        /* libparted swallows exceptions here (I think) and just returns
         * NULL if the match is not made.  Reset exception flag and return
         * None.
         */
        if (partedExnRaised) {
            partedExnRaised = 0;
        }

        Py_INCREF(Py_None);
        return Py_None;
    }

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

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_SetString(FileSystemException, "Failed to find any filesystem in given geometry");

        return NULL;
    }

    return (PyObject *) ret;
}
