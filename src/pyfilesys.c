/*
 * pyfilesys.c
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
    if (op == Py_EQ) {
        if (!(_ped_FileSystemType_Type_obj.tp_compare((PyObject *) a, b))) {
            Py_RETURN_TRUE;
        } else {
            Py_RETURN_FALSE;
        }
    } else if (op == Py_NE) {
        if (_ped_FileSystemType_Type_obj.tp_compare((PyObject *) a, b)) {
            Py_RETURN_TRUE;
        } else {
            Py_RETURN_FALSE;
        }
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
            return PyString_FromString(self->name);
        else
            return PyString_FromString("");
    } else {
        PyErr_Format(PyExc_AttributeError, "_ped.FileSystemType object has no attribute %s", member);
        return NULL;
    }
}

/* _ped.FileSystem functions */
void _ped_FileSystem_dealloc(_ped_FileSystem *self) {
    _ped_FileSystemType *fstype = (_ped_FileSystemType *) self->type;

    if (self->ped_filesystem) {
        /* XXX: do we want to set an exception string here? */
        if (!ped_file_system_close(self->ped_filesystem)) {
            PyErr_Format(FileSystemException, "Failed to close filesystem type %s", fstype->name);
        }
    }

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
    if (op == Py_EQ) {
        if (!(_ped_FileSystem_Type_obj.tp_compare((PyObject *) a, b))) {
            Py_RETURN_TRUE;
        } else {
            Py_RETURN_FALSE;
        }
    } else if (op == Py_NE) {
        if (_ped_FileSystem_Type_obj.tp_compare((PyObject *) a, b)) {
            Py_RETURN_TRUE;
        } else {
            Py_RETURN_FALSE;
        }
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

    type = PyString_AsString(_ped_FileSystem_Type_obj.tp_repr(self->type));
    if (type == NULL) {
        return NULL;
    }

    geom = PyString_AsString(_ped_Geometry_Type_obj.tp_repr(self->geom));
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
        return PyInt_FromLong(self->checked);
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

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_FileSystemType_Type_obj, &in_fstype,
                          &_ped_Geometry_Type_obj, &in_geom)) {
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
    }
    else {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_Format(FileSystemException, "Failed to probe filesystem type %s", fstype->name);

        return NULL;
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

PyObject *py_ped_file_system_clobber(PyObject *s, PyObject *args) {
    int ret = -1;
    _ped_FileSystem *self = (_ped_FileSystem *) s;
    PedGeometry *geom = NULL;

    geom = _ped_Geometry2PedGeometry(self->geom);
    if (!geom) {
        return NULL;
    }

    ret = ped_file_system_clobber(geom);

    if (!ret) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                PyErr_SetString(IOException, partedExnMessage);
        }
        else
            PyErr_SetString(FileSystemException, "Failed to clobber any filesystem in given geometry");

        return NULL;
    }

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_file_system_open(PyObject *s, PyObject *args) {
    PedFileSystem *fs = NULL;

    fs = _ped_FileSystem2PedFileSystem(s);

    if (fs) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_file_system_create(PyObject *s, PyObject *args) {
    _ped_FileSystem *self = (_ped_FileSystem *) s;
    PyObject *in_timer = NULL;
    PedGeometry *geom = NULL;
    PedFileSystemType *fstype = NULL;
    PedTimer *timer = NULL;
    PedFileSystem *fs = NULL;
    _ped_FileSystem *ret = NULL;

    if (!PyArg_ParseTuple(args, "|O!", &_ped_Timer_Type_obj, &in_timer)) {
        return NULL;
    }

    geom = _ped_Geometry2PedGeometry(self->geom);
    if (!geom) {
        return NULL;
    }

    fstype = _ped_FileSystemType2PedFileSystemType(self->type);
    if (!fstype) {
        return NULL;
    }

    if (in_timer) {
        timer = _ped_Timer2PedTimer(in_timer);
        if (!timer) {
            return NULL;
        }
    }
    else
       timer = NULL;

    fs = ped_file_system_create(geom, fstype, timer);
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
            PyErr_Format(FileSystemException, "Failed to create filesystem type %s", fstype->name);

        ped_timer_destroy(timer);
        return NULL;
    }

    ped_file_system_destroy(fs);
    ped_timer_destroy(timer);

    return (PyObject *) ret;
}

PyObject *py_ped_file_system_close(PyObject *s, PyObject *args) {
    int ret = -1;
    PedFileSystem *fs = NULL;

    fs = _ped_FileSystem2PedFileSystem(s);
    if (!fs) {
        return NULL;
    }

    ret = ped_file_system_close(fs);

    ped_file_system_destroy(fs);

    if (!ret) {
        PyErr_Format(FileSystemException, "Failed to close filesystem type %s", fs->type->name);
        return NULL;
    }

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_file_system_check(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_timer = NULL;
    PedFileSystem *fs = NULL;
    PedTimer *out_timer = NULL;

    if (!PyArg_ParseTuple(args, "|O!", &_ped_Timer_Type_obj, &in_timer)) {
        return NULL;
    }

    fs = _ped_FileSystem2PedFileSystem(s);
    if (!fs) {
        return NULL;
    }

    if (in_timer) {
        out_timer = _ped_Timer2PedTimer(in_timer);
        if (!out_timer) {
            ped_file_system_destroy(fs);
            return NULL;
        }
    }
    else
        out_timer = NULL;

    ret = ped_file_system_check(fs, out_timer);

    ped_file_system_destroy(fs);
    ped_timer_destroy(out_timer);

    /* NotImplementedError may have been raised if it's an unsupported
     * operation for this filesystem.  Otherwise, we shouldn't get any
     * exceptions here.
     */
    if (!ret && partedExnRaised) {
        partedExnRaised = 0;
        return NULL;
    }

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_file_system_copy(PyObject *s, PyObject *args) {
    PyObject *in_geom = NULL, *in_timer = NULL;
    PedFileSystem *fs = NULL;
    PedGeometry *out_geom = NULL;
    PedTimer *out_timer = NULL;
    PedFileSystem *finalfs = NULL;
    _ped_FileSystem *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!|O!", &_ped_Geometry_Type_obj, &in_geom,
                          &_ped_Timer_Type_obj, &in_timer)) {
        return NULL;
    }

    fs = _ped_FileSystem2PedFileSystem(s);
    if (!fs) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (!out_geom) {
        ped_file_system_destroy(fs);
        return NULL;
    }

    if (in_timer) {
        out_timer = _ped_Timer2PedTimer(in_timer);
        if (!out_timer) {
            ped_file_system_destroy(fs);
            return NULL;
        }
    }
    else
        out_timer = NULL;

    finalfs = ped_file_system_copy(fs, out_geom, out_timer);

    ped_file_system_destroy(fs);
    ped_timer_destroy(out_timer);

    if (finalfs) {
        ret = PedFileSystem2_ped_FileSystem(finalfs);
    }
    else {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PyExc_NotImplementedError) &&
                !PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(FileSystemException, partedExnMessage);
        }
        else
            PyErr_Format(FileSystemException, "Failed to copy filesystem type %s", fs->type->name);

        return NULL;
    }

    ped_file_system_destroy(finalfs);

    return (PyObject *) ret;
}

PyObject *py_ped_file_system_resize(PyObject *s, PyObject *args) {
    _ped_FileSystem *self = (_ped_FileSystem *) s;
    PyObject *in_geom = NULL, *in_timer = NULL;
    PedFileSystem *fs = NULL;
    PedGeometry *out_geom = NULL;
    PedTimer *out_timer = NULL;
    int ret = -1;

    if (!PyArg_ParseTuple(args, "O!|O!", &_ped_Geometry_Type_obj, &in_geom,
                          &_ped_Timer_Type_obj, &in_timer)) {
        return NULL;
    }

    fs = _ped_FileSystem2PedFileSystem(s);
    if (!fs) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (!out_geom) {
        ped_file_system_destroy(fs);
        return NULL;
    }

    if (in_timer) {
        out_timer = _ped_Timer2PedTimer(in_timer);
        if (!out_timer) {
            ped_file_system_destroy(fs);
            return NULL;
        }
    }
    else
        out_timer = NULL;

    ret = ped_file_system_resize(fs, out_geom, out_timer);

    if (ret)
        *((_ped_Geometry *)self->geom)->ped_geometry = *(fs->geom);

    ped_file_system_destroy(fs);
    ped_timer_destroy(out_timer);

    if (!ret) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PyExc_NotImplementedError) &&
                !PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(FileSystemException, partedExnMessage);
        }
        else
            PyErr_Format(FileSystemException, "Failed to resize filesystem type %s", fs->type->name);

        return NULL;
    }

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

/*
 * This is a function that exists in filesys.c in libparted, but the
 * way it works it is more appropriate to make it a method on a Device
 * object in the _ped module.
 *
 * The self object for this method is a Device, so the input parameter
 * will be a FileSystemType and the return value will be a Constraint.
 */
PyObject *py_ped_file_system_get_create_constraint(PyObject *s,
                                                   PyObject *args) {
    PyObject *in_fstype = NULL;
    PedFileSystemType *fstype = NULL;
    PedDevice *device = NULL;
    PedConstraint *constraint = NULL;
    _ped_Constraint *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_FileSystemType_Type_obj, &in_fstype)) {
        return NULL;
    }

    device = _ped_Device2PedDevice(s);
    if (!device) {
        return NULL;
    }

    fstype = _ped_FileSystemType2PedFileSystemType(in_fstype);
    if (!fstype) {
        return NULL;
    }

    constraint = ped_file_system_get_create_constraint(fstype, device);
    if (constraint) {
        ret = PedConstraint2_ped_Constraint(constraint);
    }
    else {
        PyErr_Format(CreateException, "Failed to create constraint for filesystem type %s", fstype->name);
        return NULL;
    }

    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}

PyObject *py_ped_file_system_get_resize_constraint(PyObject *s,
                                                   PyObject *args) {
    PedFileSystem *fs = NULL;
    PedConstraint *constraint = NULL;
    _ped_Constraint *ret = NULL;

    fs = _ped_FileSystem2PedFileSystem(s);
    if (!fs) {
        return NULL;
    }

    constraint = ped_file_system_get_resize_constraint(fs);

    ped_file_system_destroy(fs);

    if (constraint) {
        ret = PedConstraint2_ped_Constraint(constraint);
    }
    else {
        PyErr_Format(CreateException, "Failed to create resize constraint for filesystem type %s", fs->type->name);
        return NULL;
    }

    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}

/*
 * This is a function that exists in filesys.c in libparted, but the
 * way it works it is more appropriate to make it a method on a Device
 * object in the _ped module.
 *
 * The self object for this method is a Device, so the input parameter
 * will be a FileSystem and the return value will be a Constraint.
 */
PyObject *py_ped_file_system_get_copy_constraint(PyObject *s, PyObject *args) {
    PedDevice *device = NULL;
    PyObject *in_fs = NULL;
    PedFileSystem *fs = NULL;
    PedConstraint *constraint = NULL;
    _ped_Constraint *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_FileSystem_Type_obj, &in_fs)) {
        return NULL;
    }

    device = _ped_Device2PedDevice(s);
    if (!device) {
        return NULL;
    }

    fs = _ped_FileSystem2PedFileSystem(s);
    if (!fs) {
        return NULL;
    }

    constraint = ped_file_system_get_copy_constraint(fs, device);

    ped_file_system_destroy(fs);

    if (constraint) {
        ret = PedConstraint2_ped_Constraint(constraint);
    }
    else {
        PyErr_Format(CreateException, "Failed to create copy constraint for filesystem type %s", fs->type->name);
        return NULL;
    }

    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}
