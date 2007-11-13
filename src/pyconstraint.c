/*
 * pyconstraint.c
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
 *                    Chris Lumens <clumens@redhat.com>
 */

#include <Python.h>

#include "convert.h"
#include "exceptions.h"
#include "pyconstraint.h"
#include "pygeom.h"
#include "pynatmath.h"

/* _ped.Constraint functions */
void _ped_Constraint_dealloc(_ped_Constraint *self) {
    PyObject_Del(self);
}

PyObject *_ped_Constraint_new(PyTypeObject *type, PyObject *args,
                              PyObject *kwds) {
    _ped_Constraint *self;

    self = PyObject_New(_ped_Constraint, &_ped_Constraint_Type_obj);
    return (PyObject *) self;
}

int _ped_Constraint_init(_ped_Constraint *self, PyObject *args,
                         PyObject *kwds) {
    static char *kwlist[] = {"start_align", "end_align", "start_range",
                             "end_range", "min_size", "max_size", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O!O!O!O!ll", kwlist,
                                     &_ped_Alignment_Type_obj, &self->start_align,
                                     &_ped_Alignment_Type_obj, &self->end_align,
                                     &_ped_Geometry_Type_obj, &self->start_range,
                                     &_ped_Geometry_Type_obj, &self->end_range,
                                     &self->min_size, &self->max_size))
        return -1;
    else
        return 0;
}

PyObject *_ped_Constraint_get(_ped_Constraint *self, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        return NULL;
    }

    if (!strcmp(member, "min_size")) {
        return PyLong_FromLongLong(self->min_size);
    } else if (!strcmp(member, "max_size")) {
        return PyLong_FromLongLong(self->max_size);
    } else {
        return NULL;
    }
}

int _ped_Constraint_set(_ped_Constraint *self, PyObject *value, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        return -1;
    }

    if (!strcmp(member, "min_size")) {
        self->min_size = PyLong_AsLongLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else if (!strcmp(member, "max_size")) {
        self->max_size = PyLong_AsLongLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else {
        return -1;
    }

    return 0;
}

/* 1:1 function mappings for constraint.h in libparted */
PyObject *py_ped_constraint_init(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_constraint, *in_start_align, *in_end_align;
    PyObject *in_start_range, *in_end_range;
    PedConstraint *out_constraint;
    PedAlignment *out_start_align, *out_end_align;
    PedGeometry *out_start_range, *out_end_range;
    PedSector min_size, max_size;

    if (!PyArg_ParseTuple(args, "O!O!O!O!O!ll", &_ped_Constraint_Type_obj,
                          &in_constraint, &_ped_Alignment_Type_obj,
                          &in_start_align, &_ped_Alignment_Type_obj,
                          &in_end_align, &_ped_Geometry_Type_obj,
                          &in_start_range, &_ped_Geometry_Type_obj,
                          &in_end_range, &min_size, &max_size)) {
        return NULL;
    }

    out_constraint = _ped_Constraint2PedConstraint(in_constraint);
    out_start_align = _ped_Alignment2PedAlignment(in_start_align);
    out_end_align = _ped_Alignment2PedAlignment(in_end_align);
    out_start_range = _ped_Geometry2PedGeometry(in_start_range);
    out_end_range = _ped_Geometry2PedGeometry(in_end_range);

    ret = ped_constraint_init(out_constraint, out_start_align, out_end_align,
                              out_start_range, out_end_range,
                              min_size, max_size);

    if (ret == 0) {
        PyErr_SetString(ConstraintException, "Could not create new constraint");
        return NULL;
    }

    ped_constraint_destroy(out_constraint);
    ped_alignment_destroy(out_start_align);
    ped_alignment_destroy(out_end_align);
    ped_geometry_destroy(out_start_range);
    ped_geometry_destroy(out_end_range);

    return Py_BuildValue("i", ret);
}

PyObject *py_ped_constraint_new(PyObject *s, PyObject *args) {
    PyObject *in_start_align, *in_end_align, *in_start_range, *in_end_range;
    PedAlignment *out_start_align, *out_end_align;
    PedGeometry *out_start_range, *out_end_range;
    PedSector min_size, max_size;
    PedConstraint *constraint;
    _ped_Constraint *ret;

    if (!PyArg_ParseTuple(args, "O!O!O!O!ll", &_ped_Alignment_Type_obj,
                          &in_start_align, &_ped_Alignment_Type_obj,
                          &in_end_align, &_ped_Geometry_Type_obj,
                          &in_start_range, &_ped_Geometry_Type_obj,
                          &in_end_range, &min_size, &max_size)) {
        return NULL;
    }

    out_start_align = _ped_Alignment2PedAlignment(in_start_align);
    out_end_align = _ped_Alignment2PedAlignment(in_end_align);
    out_start_range = _ped_Geometry2PedGeometry(in_start_range);
    out_end_range = _ped_Geometry2PedGeometry(in_end_range);

    constraint = ped_constraint_new(out_start_align, out_end_align,
                                    out_start_range, out_end_range,
                                    min_size, max_size);
    if (constraint) {
        ret = PedConstraint2_ped_Constraint(constraint);
    }
    else {
        PyErr_SetString(ConstraintException, "Could not create new constraint");
        return NULL;
    }

    ped_alignment_destroy(out_start_align);
    ped_alignment_destroy(out_end_align);
    ped_geometry_destroy(out_start_range);
    ped_geometry_destroy(out_end_range);
    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}

PyObject *py_ped_constraint_new_from_min_max(PyObject *s, PyObject *args) {
    PyObject *in_min, *in_max;
    PedGeometry *out_min, *out_max;
    PedConstraint *constraint;
    _ped_Constraint *ret;

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_Geometry_Type_obj, &in_min,
                          &_ped_Geometry_Type_obj, &in_max)) {
        return NULL;
    }

    out_min = _ped_Geometry2PedGeometry(in_min);
    out_max = _ped_Geometry2PedGeometry(in_max);

    constraint = ped_constraint_new_from_min_max(out_min, out_max);
    if (constraint) {
        ret = PedConstraint2_ped_Constraint(constraint);
    }
    else {
        PyErr_SetString(ConstraintException, "Could not create new constraint");
        return NULL;
    }

    ped_geometry_destroy(out_min);
    ped_geometry_destroy(out_max);
    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}

PyObject *py_ped_constraint_new_from_min(PyObject *s, PyObject *args) {
    PyObject *in_min;
    PedGeometry *out_min;
    PedConstraint *constraint;
    _ped_Constraint *ret;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Geometry_Type_obj, &in_min)) {
        return NULL;
    }

    out_min = _ped_Geometry2PedGeometry(in_min);

    constraint = ped_constraint_new_from_min(out_min);
    if (constraint) {
        ret = PedConstraint2_ped_Constraint(constraint);
    }
    else {
        PyErr_SetString(ConstraintException, "Could not create new constraint");
        return NULL;
    }

    ped_geometry_destroy(out_min);
    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}

PyObject *py_ped_constraint_new_from_max(PyObject *s, PyObject *args) {
    PyObject *in_max;
    PedGeometry *out_max;
    PedConstraint *constraint;
    _ped_Constraint *ret;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Geometry_Type_obj, &in_max)) {
        return NULL;
    }

    out_max = _ped_Geometry2PedGeometry(in_max);

    constraint = ped_constraint_new_from_max(out_max);
    if (constraint) {
        ret = PedConstraint2_ped_Constraint(constraint);
    }
    else {
        PyErr_SetString(ConstraintException, "Could not create new constraint");
        return NULL;
    }

    ped_geometry_destroy(out_max);
    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}

PyObject *py_ped_constraint_duplicate(PyObject *s, PyObject *args) {
    PyObject *in_constraint;
    PedConstraint *out_constraint, *constraint;
    _ped_Constraint *ret;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Constraint_Type_obj,
                          &in_constraint)) {
        return NULL;
    }

    out_constraint = _ped_Constraint2PedConstraint(in_constraint);

    constraint = ped_constraint_duplicate(out_constraint);
    if (constraint) {
        ret = PedConstraint2_ped_Constraint(constraint);
    }
    else {
        PyErr_SetString(ConstraintException, "Could not duplicate constraint");
        return NULL;
    }

    ped_constraint_destroy(out_constraint);
    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}

PyObject *py_ped_constraint_done(PyObject *s, PyObject *args) {
    PyObject *in_constraint;
    PedConstraint *out_constraint;

    if (!PyArg_ParseTuple(args, "O", &_ped_Constraint_Type_obj,
                          &in_constraint)) {
        return NULL;
    }

    out_constraint = _ped_Constraint2PedConstraint(in_constraint);
    ped_constraint_done(out_constraint);
    ped_constraint_destroy(out_constraint);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_constraint_destroy(PyObject *s, PyObject *args) {
    PyObject *in_constraint;
    PedConstraint *out_constraint;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Constraint_Type_obj,
                          &in_constraint)) {
        return NULL;
    }

    out_constraint = _ped_Constraint2PedConstraint(in_constraint);
    ped_constraint_destroy(out_constraint);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_constraint_intersect(PyObject *s, PyObject *args) {
    PyObject *in_constraintA, *in_constraintB;
    PedConstraint *out_constraintA, *out_constraintB, *constraint;
    _ped_Constraint *ret;

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_Constraint_Type_obj,
                          &in_constraintA, &_ped_Constraint_Type_obj,
                          &in_constraintB)) {
        return NULL;
    }

    out_constraintA = _ped_Constraint2PedConstraint(in_constraintA);
    out_constraintB = _ped_Constraint2PedConstraint(in_constraintB);

    constraint = ped_constraint_intersect(out_constraintA, out_constraintB);
    if (constraint) {
        ret = PedConstraint2_ped_Constraint(constraint);
    }
    else {
        PyErr_SetString(ConstraintException, "Could not find constraint intersection");
        return NULL;
    }

    ped_constraint_destroy(out_constraintA);
    ped_constraint_destroy(out_constraintB);
    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}

PyObject *py_ped_constraint_solve_max(PyObject *s, PyObject *args) {
    PyObject *in_constraint;
    PedConstraint *out_constraint;
    PedGeometry *geometry;
    _ped_Geometry *ret;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Constraint_Type_obj,
                          &in_constraint)) {
        return NULL;
    }

    out_constraint = _ped_Constraint2PedConstraint(in_constraint);

    geometry = ped_constraint_solve_max(out_constraint);
    if (geometry) {
        ret = PedGeometry2_ped_Geometry(geometry);
    }
    else {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(ConstraintException, partedExnMessage);
        }
        else
            PyErr_SetString(ConstraintException, "Could not solve constraint");

        return NULL;
    }

    ped_constraint_destroy(out_constraint);
    ped_geometry_destroy(geometry);

    return (PyObject *) ret;
}

PyObject *py_ped_constraint_solve_nearest(PyObject *s, PyObject *args) {
    PyObject *in_constraint, *in_geometry;
    PedConstraint *out_constraint;
    PedGeometry *out_geometry;
    PedGeometry *geometry;
    _ped_Geometry *ret;

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_Constraint_Type_obj,
                          &in_constraint, &_ped_Geometry_Type_obj,
                          &in_geometry)) {
        return NULL;
    }

    out_constraint = _ped_Constraint2PedConstraint(in_constraint);
    out_geometry = _ped_Geometry2PedGeometry(in_geometry);

    geometry = ped_constraint_solve_nearest(out_constraint, out_geometry);
    if (geometry) {
        ret = PedGeometry2_ped_Geometry(geometry);
    }
    else {
        PyErr_SetString(ConstraintException, "Could not solve constraint");
        return NULL;
    }

    ped_constraint_destroy(out_constraint);
    ped_geometry_destroy(out_geometry);
    ped_geometry_destroy(geometry);

    return (PyObject *) ret;
}

PyObject *py_ped_constraint_is_solution(PyObject *s, PyObject *args) {
    PyObject *in_constraint, *in_geometry;
    PedConstraint *out_constraint;
    PedGeometry *out_geometry;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_Constraint_Type_obj,
                          &in_constraint, &_ped_Geometry_Type_obj,
                          &in_geometry)) {
        return NULL;
    }

    out_constraint = _ped_Constraint2PedConstraint(in_constraint);
    out_geometry = _ped_Geometry2PedGeometry(in_geometry);

    ret = ped_constraint_is_solution(out_constraint, out_geometry);

    ped_constraint_destroy(out_constraint);
    ped_geometry_destroy(out_geometry);

    return Py_BuildValue("i", ret);
}

PyObject *py_ped_constraint_any(PyObject *s, PyObject *args) {
    PyObject *in_device;
    PedDevice *out_device;
    PedConstraint *constraint;
    _ped_Constraint *ret;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Device_Type_obj, &in_device)) {
        return NULL;
    }

    out_device = _ped_Device2PedDevice(in_device);

    constraint = ped_constraint_any(out_device);
    if (constraint) {
        ret = PedConstraint2_ped_Constraint(constraint);
    }
    else {
        PyErr_SetString(ConstraintException, "Could not create constraint");
        return NULL;
    }

    ped_device_destroy(out_device);
    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}

PyObject *py_ped_constraint_exact(PyObject *s, PyObject *args) {
    PyObject *in_geometry;
    PedGeometry *out_geometry;
    PedConstraint *constraint;
    _ped_Constraint *ret;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Geometry_Type_obj, &in_geometry)) {
        return NULL;
    }

    out_geometry = _ped_Geometry2PedGeometry(in_geometry);

    constraint = ped_constraint_exact(out_geometry);
    if (constraint) {
        ret = PedConstraint2_ped_Constraint(constraint);
    }
    else {
        PyErr_SetString(ConstraintException, "Could not create constraint");
        return NULL;
    }

    ped_geometry_destroy(out_geometry);
    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}
