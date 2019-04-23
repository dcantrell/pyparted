/*
 * pyconstraint.c
 *
 * Copyright (C) 2007-2013  Red Hat, Inc.
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
#include "pygeom.h"
#include "pynatmath.h"
#include "docstrings/pyconstraint.h"
#include "typeobjects/pyconstraint.h"

/* _ped.Constraint functions */
void _ped_Constraint_dealloc(_ped_Constraint *self) {
    PyObject_GC_UnTrack(self);

    Py_CLEAR(self->start_align);
    self->start_align = NULL;

    Py_CLEAR(self->end_align);
    self->end_align = NULL;

    Py_CLEAR(self->start_range);
    self->start_range = NULL;

    Py_CLEAR(self->end_range);
    self->end_range = NULL;

    PyObject_GC_Del(self);
}

int _ped_Constraint_compare(_ped_Constraint *self, PyObject *obj) {
    _ped_Constraint *comp = NULL;
    int check = PyObject_IsInstance(obj, (PyObject *) &_ped_Constraint_Type_obj);

    if (PyErr_Occurred()) {
        return -1;
    }

    if (!check) {
        PyErr_SetString(PyExc_ValueError, "object comparing to must be a _ped.Constraint");
        return -1;
    }

    comp = (_ped_Constraint *) obj;
    if ((_ped_Alignment_Type_obj.tp_richcompare(self->start_align, comp->start_align, Py_EQ)) &&
        (_ped_Alignment_Type_obj.tp_richcompare(self->end_align, comp->end_align, Py_EQ)) &&
        (_ped_Geometry_Type_obj.tp_richcompare(self->start_range, comp->start_range, Py_EQ)) &&
        (_ped_Geometry_Type_obj.tp_richcompare(self->end_range, comp->end_range, Py_EQ)) &&
        (self->min_size == comp->min_size) &&
        (self->max_size == comp->max_size)) {
        return 0;
    } else {
        return 1;
    }
}

PyObject *_ped_Constraint_richcompare(_ped_Constraint *a, PyObject *b, int op) {
    if (op == Py_EQ || op == Py_NE) {
        int rv = _ped_Constraint_compare(a, b);
        if (PyErr_Occurred())
            return NULL;
        return PyBool_FromLong(op == Py_EQ ? rv == 0 : rv != 0);
    } else if ((op == Py_LT) || (op == Py_LE) ||
               (op == Py_GT) || (op == Py_GE)) {
        PyErr_SetString(PyExc_TypeError, "comparison operator not supported for _ped.Constraint");
        return NULL;
    } else {
        PyErr_SetString(PyExc_ValueError, "unknown richcompare op");
        return NULL;
    }
}

PyObject *_ped_Constraint_str(_ped_Constraint *self) {
    char *ret = NULL;
    char *start_align = NULL, *end_align = NULL;
    char *start_range = NULL, *end_range = NULL;

    start_align = (char *) PyUnicode_AsUTF8(_ped_Alignment_Type_obj.tp_repr(self->start_align));
    if (start_align == NULL) {
        return NULL;
    }

    end_align = (char *) PyUnicode_AsUTF8(_ped_Alignment_Type_obj.tp_repr(self->end_align));
    if (end_align == NULL) {
        return NULL;
    }

    start_range = (char *) PyUnicode_AsUTF8(_ped_Geometry_Type_obj.tp_repr(self->start_range));
    if (start_range == NULL) {
        return NULL;
    }

    end_range = (char *) PyUnicode_AsUTF8(_ped_Geometry_Type_obj.tp_repr(self->end_range));
    if (end_range == NULL) {
        return NULL;
    }

    if (asprintf(&ret, "_ped.Constraint instance --\n"
                       "  start_align: %s  end_align: %s\n"
                       "  start_range: %s  end_range: %s\n"
                       "  min_size: %lld  max_size: %lld",
                 start_align, end_align,
                 start_range, end_range,
                 self->min_size, self->max_size) == -1) {
        return PyErr_NoMemory();
    }

    return Py_BuildValue("s", ret);
}

int _ped_Constraint_traverse(_ped_Constraint *self, visitproc visit, void *arg) {
    int err;

    if (self->start_align) {
        if ((err = visit(self->start_align, arg))) {
            return err;
        }
    }

    if (self->end_align) {
        if ((err = visit(self->end_align, arg))) {
            return err;
        }
    }

    if (self->start_range) {
        if ((err = visit(self->start_range, arg))) {
            return err;
        }
    }

    if (self->end_range) {
        if ((err = visit(self->end_range, arg))) {
            return err;
        }
    }

    return 0;
}

int _ped_Constraint_clear(_ped_Constraint *self) {
    Py_CLEAR(self->start_align);
    self->start_align = NULL;

    Py_CLEAR(self->end_align);
    self->end_align = NULL;

    Py_CLEAR(self->start_range);
    self->start_range = NULL;

    Py_CLEAR(self->end_range);
    self->end_range = NULL;

    return 0;
}

int _ped_Constraint_init(_ped_Constraint *self, PyObject *args,
                         PyObject *kwds) {
    static char *kwlist[] = {"start_align", "end_align", "start_range",
                             "end_range", "min_size", "max_size", NULL};
    PedConstraint *constraint = NULL;
    PedAlignment *start_align = NULL, *end_align = NULL;
    PedGeometry *start_range = NULL, *end_range = NULL;

    if (kwds == NULL) {
        if (!PyArg_ParseTuple(args, "O!O!O!O!LL",
                              &_ped_Alignment_Type_obj, &self->start_align,
                              &_ped_Alignment_Type_obj, &self->end_align,
                              &_ped_Geometry_Type_obj, &self->start_range,
                              &_ped_Geometry_Type_obj, &self->end_range,
                              &self->min_size, &self->max_size)) {
            self->start_align = self->end_align = NULL;
            self->start_range = self->end_range = NULL;
            return -1;
        }
    } else {
        if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!O!O!O!LL", kwlist,
                                         &_ped_Alignment_Type_obj,
                                         &self->start_align,
                                         &_ped_Alignment_Type_obj,
                                         &self->end_align,
                                         &_ped_Geometry_Type_obj,
                                         &self->start_range,
                                         &_ped_Geometry_Type_obj,
                                         &self->end_range,
                                         &self->min_size,
                                         &self->max_size)) {
            self->start_align = self->end_align = NULL;
            self->start_range = self->end_range = NULL;
            return -2;
        }
    }

    /*
     * try to call libparted with provided information,
     * on failure, raise an exception
     */
    start_align = _ped_Alignment2PedAlignment(self->start_align);
    end_align = _ped_Alignment2PedAlignment(self->end_align);
    start_range = _ped_Geometry2PedGeometry(self->start_range);
    end_range = _ped_Geometry2PedGeometry(self->end_range);

    constraint = ped_constraint_new(start_align, end_align,
                                    start_range, end_range,
                                    self->min_size, self->max_size);
    if (constraint == NULL) {
        PyErr_SetString(CreateException, "Could not create new constraint");

        ped_alignment_destroy(start_align);
        ped_alignment_destroy(end_align);
        self->start_align = NULL;
        self->end_align = NULL;
        self->start_range = NULL;
        self->end_range = NULL;
        return -3;
    }

    /* increment reference count for PyObjects read by PyArg_ParseTuple */
    Py_INCREF(self->start_align);
    Py_INCREF(self->end_align);
    Py_INCREF(self->start_range);
    Py_INCREF(self->end_range);

    /* clean up libparted objects we created */
    ped_alignment_destroy(start_align);
    ped_alignment_destroy(end_align);
    ped_constraint_destroy(constraint);
    return 0;
}

PyObject *_ped_Constraint_get(_ped_Constraint *self, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Constraint()");
        return NULL;
    }

    if (!strcmp(member, "min_size")) {
        return PyLong_FromLongLong(self->min_size);
    } else if (!strcmp(member, "max_size")) {
        return PyLong_FromLongLong(self->max_size);
    } else {
        PyErr_Format(PyExc_AttributeError, "_ped.Constraint object has no attribute %s", member);
        return NULL;
    }
}

int _ped_Constraint_set(_ped_Constraint *self, PyObject *value, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Constraint()");
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
        PyErr_Format(PyExc_AttributeError, "_ped.Constraint object has no attribute %s", member);
        return -1;
    }

    return 0;
}

/* 1:1 function mappings for constraint.h in libparted */
PyObject *py_ped_constraint_new_from_min_max(PyObject *s, PyObject *args) {
    PyObject *in_min = NULL, *in_max = NULL;
    PedGeometry *out_min = NULL, *out_max = NULL;
    PedConstraint *constraint = NULL;
    _ped_Constraint *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_Geometry_Type_obj, &in_min,
                          &_ped_Geometry_Type_obj, &in_max)) {
        return NULL;
    }

    out_min = _ped_Geometry2PedGeometry(in_min);
    if (out_min == NULL) {
        return NULL;
    }

    out_max = _ped_Geometry2PedGeometry(in_max);
    if (out_max == NULL) {
        return NULL;
    }

    /* ped_constraint_new_from_min_max will ASSERT if this isn't enforced. */
    if (!ped_geometry_test_inside(out_max, out_min)) {
        PyErr_SetString(CreateException, "min geometry must be contained within max geometry");
        return NULL;
    }

    constraint = ped_constraint_new_from_min_max(out_min, out_max);
    if (constraint) {
        ret = PedConstraint2_ped_Constraint(constraint);
    }
    else {
        PyErr_SetString(CreateException, "Could not create new constraint from min/max");
        return NULL;
    }

    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}

PyObject *py_ped_constraint_new_from_min(PyObject *s, PyObject *args) {
    PyObject *in_min = NULL;
    PedGeometry *out_min = NULL;
    PedConstraint *constraint = NULL;
    _ped_Constraint *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Geometry_Type_obj, &in_min)) {
        return NULL;
    }

    out_min = _ped_Geometry2PedGeometry(in_min);
    if (out_min == NULL) {
        return NULL;
    }

    constraint = ped_constraint_new_from_min(out_min);
    if (constraint) {
        ret = PedConstraint2_ped_Constraint(constraint);
    }
    else {
        PyErr_SetString(CreateException, "Could not create new constraint from min");
        return NULL;
    }

    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}

PyObject *py_ped_constraint_new_from_max(PyObject *s, PyObject *args) {
    PyObject *in_max = NULL;
    PedGeometry *out_max = NULL;
    PedConstraint *constraint = NULL;
    _ped_Constraint *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Geometry_Type_obj, &in_max)) {
        return NULL;
    }

    out_max = _ped_Geometry2PedGeometry(in_max);
    if (out_max == NULL) {
        return NULL;
    }

    constraint = ped_constraint_new_from_max(out_max);
    if (constraint) {
        ret = PedConstraint2_ped_Constraint(constraint);
    }
    else {
        PyErr_SetString(CreateException, "Could not create new constraint from max");
        return NULL;
    }

    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}

/* XXX: Remove this function at some point in the future.  The deprecation
 * warning tells people what they should be doing.
 */
PyObject *py_ped_constraint_duplicate(PyObject *s, PyObject *args) {
    PedConstraint *constraint = NULL, *dup_constraint = NULL;
    _ped_Constraint *ret = NULL;

    constraint = _ped_Constraint2PedConstraint(s);
    if (constraint == NULL) {
        return NULL;
    }

    if (PyErr_WarnEx(PyExc_DeprecationWarning,
                     "use copy.deepcopy() to duplicate a _ped.Constraint",
                     1) == -1) {
        return NULL;
    }

    dup_constraint = ped_constraint_duplicate(constraint);
    ped_constraint_destroy(constraint);

    if (dup_constraint) {
        ret = PedConstraint2_ped_Constraint(dup_constraint);
    } else {
        PyErr_SetString(CreateException, "Could not duplicate constraint");
        return NULL;
    }

    ped_constraint_destroy(dup_constraint);

    return (PyObject *) ret;
}

PyObject *py_ped_constraint_intersect(PyObject *s, PyObject *args) {
    PyObject *in_constraintB = NULL;
    PedConstraint *constraintA = NULL, *constraintB = NULL;
    PedConstraint *constraint = NULL;
    _ped_Constraint *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Constraint_Type_obj,
                          &in_constraintB)) {
        return NULL;
    }

    constraintA = _ped_Constraint2PedConstraint(s);
    if (constraintA == NULL) {
        return NULL;
    }

    constraintB = _ped_Constraint2PedConstraint(in_constraintB);
    if (constraintB == NULL) {
        ped_constraint_destroy(constraintA);
        return NULL;
    }

    constraint = ped_constraint_intersect(constraintA, constraintB);

    ped_constraint_destroy(constraintA);
    ped_constraint_destroy(constraintB);

    if (constraint) {
        ret = PedConstraint2_ped_Constraint(constraint);
    }
    else {
        PyErr_SetString(PyExc_ArithmeticError, "Could not find constraint intersection");
        return NULL;
    }

    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}

PyObject *py_ped_constraint_solve_max(PyObject *s, PyObject *args) {
    PedConstraint *constraint = NULL;
    PedGeometry *geometry = NULL;
    _ped_Geometry *ret = NULL;

    constraint = _ped_Constraint2PedConstraint(s);
    if (constraint == NULL) {
        return NULL;
    }

    geometry = ped_constraint_solve_max(constraint);

    ped_constraint_destroy(constraint);

    if (geometry) {
        ret = PedGeometry2_ped_Geometry(geometry);
    }
    else {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException) &&
                !PyErr_ExceptionMatches(PyExc_NotImplementedError))
                PyErr_SetString(ConstraintException, partedExnMessage);
        }
        else
            PyErr_SetString(PyExc_ArithmeticError, "Could not find largest region satisfying constraint");

        return NULL;
    }

    return (PyObject *) ret;
}

PyObject *py_ped_constraint_solve_nearest(PyObject *s, PyObject *args) {
    PyObject *in_geometry = NULL;
    PedConstraint *constraint = NULL;
    PedGeometry *out_geometry = NULL;
    PedGeometry *geometry = NULL;
    _ped_Geometry *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Geometry_Type_obj,
                          &in_geometry)) {
        return NULL;
    }

    constraint = _ped_Constraint2PedConstraint(s);
    if (constraint == NULL) {
        return NULL;
    }

    out_geometry = _ped_Geometry2PedGeometry(in_geometry);
    if (out_geometry == NULL) {
        ped_constraint_destroy(constraint);
        return NULL;
    }

    geometry = ped_constraint_solve_nearest(constraint, out_geometry);

    ped_constraint_destroy(constraint);

    if (geometry) {
        ret = PedGeometry2_ped_Geometry(geometry);
    }
    else {
        PyErr_SetString(PyExc_ArithmeticError, "Could not find region nearest to constraint for given geometry");
        return NULL;
    }

    return (PyObject *) ret;
}

PyObject *py_ped_constraint_is_solution(PyObject *s, PyObject *args) {
    PyObject *in_geometry = NULL;
    PedConstraint *constraint = NULL;
    PedGeometry *out_geometry = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Geometry_Type_obj,
                          &in_geometry)) {
        return NULL;
    }

    constraint = _ped_Constraint2PedConstraint(s);
    if (constraint == NULL) {
        return NULL;
    }

    out_geometry = _ped_Geometry2PedGeometry(in_geometry);
    if (out_geometry == NULL) {
        ped_constraint_destroy(constraint);
        return NULL;
    }

    ret = ped_constraint_is_solution(constraint, out_geometry);
    ped_constraint_destroy(constraint);

    if (ret) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyObject *py_ped_constraint_any(PyObject *s, PyObject *args) {
    PyObject *in_device = NULL;
    PedDevice *out_device = NULL;
    PedConstraint *constraint = NULL;
    _ped_Constraint *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Device_Type_obj, &in_device)) {
        return NULL;
    }

    out_device = _ped_Device2PedDevice(in_device);
    if (out_device == NULL) {
        return NULL;
    }

    constraint = ped_constraint_any(out_device);
    if (constraint) {
        ret = PedConstraint2_ped_Constraint(constraint);
    }
    else {
        PyErr_SetString(CreateException, "Could not create new constraint");
        return NULL;
    }

    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}

PyObject *py_ped_constraint_exact(PyObject *s, PyObject *args) {
    PyObject *in_geometry = NULL;
    PedGeometry *out_geometry = NULL;
    PedConstraint *constraint = NULL;
    _ped_Constraint *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Geometry_Type_obj, &in_geometry)) {
        return NULL;
    }

    out_geometry = _ped_Geometry2PedGeometry(in_geometry);
    if (out_geometry == NULL) {
        return NULL;
    }

    constraint = ped_constraint_exact(out_geometry);
    if (constraint) {
        ret = PedConstraint2_ped_Constraint(constraint);
    }
    else {
        PyErr_SetString(CreateException, "Could not create exact constraint");
        return NULL;
    }

    ped_constraint_destroy(constraint);

    return (PyObject *) ret;
}

/* vim:tw=78:ts=4:et:sw=4
 */
