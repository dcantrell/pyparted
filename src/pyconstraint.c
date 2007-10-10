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
 * Red Hat Author(s): David Cantrell
 */

#include <Python.h>

#include "convert.h"
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
    /* FIXME */
    return 0;
}

/* 1:1 function mappings for constraint.h in libparted */
PyObject *py_ped_constraint_init(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_constraint, *in_start_align, *in_end_align;
    PyObject *in_start_range, *in_end_range, *in_min_size, *in_max_size;
    PedConstraint *out_constraint;
    PedAlignment *out_start_align, *out_end_align;
    PedGeometry *out_start_range, *out_end_range;
    PedSector out_min_size, out_max_size;

    if (!PyArg_ParseTuple(args, "OOOOOOO", &in_constraint, &in_start_align,
                                           &in_end_align, &in_start_range,
                                           &in_end_range, &in_min_size,
                                           &in_max_size)) {
        return NULL;
    }

    out_constraint = _ped_Constraint2PedConstraint(in_constraint);
    out_start_align = _ped_Alignment2PedAlignment(in_start_align);
    out_end_align = _ped_Alignment2PedAlignment(in_end_align);
    out_start_range = _ped_Geometry2PedGeometry(in_start_range);
    out_end_range = _ped_Geometry2PedGeometry(in_end_range);
    out_min_size = _ped_Sector2PedSector(in_min_size);
    out_max_size = _ped_Sector2PedSector(in_max_size);

    ret = ped_constraint_init(out_constraint, out_start_align, out_end_align,
                              out_start_range, out_end_range,
                              out_min_size, out_max_size);

    _free_PedConstraint(out_constraint);
    _free_PedAlignment(out_start_align);
    _free_PedAlignment(out_end_align);
    _free_PedGeometry(out_start_range);
    _free_PedGeometry(out_end_range);

    return Py_BuildValue("i", ret);
}

PyObject *py_ped_constraint_new(PyObject *s, PyObject *args) {
    PyObject *in_start_align, *in_end_align, *in_start_range, *in_end_range;
    PyObject *in_min_size, *in_max_size;
    PedAlignment *out_start_align, *out_end_align;
    PedGeometry *out_start_range, *out_end_range;
    PedSector out_min_size, out_max_size;
    PedConstraint *constraint;
    _ped_Constraint *ret;

    if (!PyArg_ParseTuple(args, "OOOOOO", &in_start_align, &in_end_align,
                                          &in_start_range, &in_end_range,
                                          &in_min_size, &in_max_size)) {
        return NULL;
    }

    out_start_align = _ped_Alignment2PedAlignment(in_start_align);
    out_end_align = _ped_Alignment2PedAlignment(in_end_align);
    out_start_range = _ped_Geometry2PedGeometry(in_start_range);
    out_end_range = _ped_Geometry2PedGeometry(in_end_range);
    out_min_size = _ped_Sector2PedSector(in_min_size);
    out_max_size = _ped_Sector2PedSector(in_max_size);

    ret = PyObject_New(_ped_Constraint, &_ped_Constraint_Type_obj);
    if (ret) {
        constraint = ped_constraint_new(out_start_align, out_end_align,
                                        out_start_range, out_end_range,
                                        out_min_size, out_max_size);
        if (constraint) {
            ret = PedConstraint2_ped_Constraint(constraint);
        }
    }

    _free_PedAlignment(out_start_align);
    _free_PedAlignment(out_end_align);
    _free_PedGeometry(out_start_range);
    _free_PedGeometry(out_end_range);
    ped_constraint_destroy(constraint);

    if (ret) {
        return (PyObject *) ret;
    } else {
        return NULL;
    }
}

PyObject *py_ped_constraint_new_from_min_max(PyObject *s, PyObject *args) {
    PyObject *in_min, *in_max;
    PedGeometry *out_min, *out_max;
    PedConstraint *constraint;
    _ped_Constraint *ret;

    if (!PyArg_ParseTuple(args, "OO", &in_min, &in_max)) {
        return NULL;
    }

    out_min = _ped_Geometry2PedGeometry(in_min);
    out_max = _ped_Geometry2PedGeometry(in_max);

    ret = PyObject_New(_ped_Constraint, &_ped_Constraint_Type_obj);
    if (ret) {
        constraint = ped_constraint_new_from_min_max(out_min, out_max);
        if (constraint) {
            ret = PedConstraint2_ped_Constraint(constraint);
        }
    }

    _free_PedGeometry(out_min);
    _free_PedGeometry(out_max);
    ped_constraint_destory(constraint);

    if (ret) {
        return (PyObject *) ret;
    } else {
        return NULL;
    }
}

PyObject *py_ped_constraint_new_from_min(PyObject *s, PyObject *args) {
    PyObject *in_min;
    PedGeometry *out_min;
    PedConstraint *constraint;
    _ped_Constraint *ret;

    if (!PyArg_ParseTuple(args, "O", &in_min)) {
        return NULL;
    }

    out_min = _ped_Geometry2PedGeometry(in_min);

    ret = PyObject_New(_ped_Constraint, &_ped_Constraint_Type_obj);
    if (ret) {
        constraint = ped_constraint_new_from_min(out_min);
        if (constraint) {
            ret = PedConstraint2_ped_Constraint(constraint);
        }
    }

    _free_PedGeometry(out_min);
    ped_constraint_destroy(constraint);

    if (ret) {
        return (PyObject *) ret;
    } else {
        return NULL;
    }
}

PyObject *py_ped_constraint_new_from_max(PyObject *s, PyObject *args) {
    PyObject *in_max;
    PedGeometry *out_max;
    PedConstraint *constraint;
    _ped_Constraint *ret;

    if (!PyArg_ParseTuple(args, "O", &in_max)) {
        return NULL;
    }

    out_max = _ped_Geometry2PedGeometry(in_max);

    ret = PyObject_New(_ped_Constraint, &_ped_Constraint_Type_obj);
    if (ret) {
        constraint = ped_constraint_new_from_max(out_max);
        if (constraint) {
            ret = PedConstraint2_ped_Constraint(constraint);
        }
    }

    _free_PedGeometry(out_max);
    ped_constraint_destroy(constraint);

    if (ret) {
        return (PyObject *) ret;
    } else {
        return NULL;
    }
}

PyObject *py_ped_constraint_duplicate(PyObject *s, PyObject *args) {
    PyObject *in_constraint;
    PedConstraint *out_constraint, *constraint;
    _ped_Constraint *ret;

    if (!PyArg_ParseTuple(args, "O", &in_constraint)) {
        return NULL;
    }

    out_constraint = _ped_Constraint2PedConstraint(in_constraint);

    ret = PyObject_New(_ped_Constraint, &_ped_Constraint_Type_obj);
    if (ret) {
        constraint = ped_constraint_duplicate(out_constraint);
        if (constraint) {
            ret = PedConstraint2_ped_Constraint(constraint);
        }
    }

    _free_PedConstraint(out_constraint);
    ped_constraint_destroy(constraint);

    if (ret) {
        return (PyObject *) ret;
    } else {
        return NULL;
    }
}

PyObject *py_ped_constraint_done(PyObject *s, PyObject *args) {
    PyObject *in_constraint;
    PedConstraint *out_constraint;

    if (!PyArg_ParseTuple(args, "O", &in_constraint)) {
        return NULL;
    }

    out_constraint = _ped_Constraint2PedConstraint(in_constraint);
    ped_constraint_done(out_constraint);
    _free_PedConstraint(out_constraint);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_constraint_destroy(PyObject *s, PyObject *args) {
    PyObject *in_constraint;
    PedConstraint *out_constraint;

    if (!PyArg_ParseTuple(args, "O", &in_constraint)) {
        return NULL;
    }

    out_constraint = _ped_Constraint2PedConstraint(in_constraint);
    ped_constraint_destroy(out_constraint);
    _free_PedConstraint(out_constraint);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_constraint_intersect(PyObject *s, PyObject *args) {
    PyObject *in_constraintA, *in_constraintB;
    PedConstraint *out_constraintA, *out_constraintB, *constraint;
    _ped_Constraint *ret;

    if (!PyArg_ParseTuple(args, "OO", &in_constraintA, &in_constraintB)) {
        return NULL;
    }

    out_constraintA = _ped_Constraint2PedConstraint(in_constraintA);
    out_constraintB = _ped_Constraint2PedConstraint(in_constraintB);

    ret = PyObject_New(_ped_Constraint, &_ped_Constraint_Type_obj);
    if (ret) {
        constraint = ped_constraint_intersect(out_constraintA, out_constraintB);
        if (constraint) {
            ret = PedConstraint2_ped_Constraint(constraint);
        }
    }

    _free_PedConstraint(out_constraintA);
    _free_PedConstraint(out_constraintB);
    ped_constraint_destroy(constraint);

    if (ret) {
        return (PyObject *) ret;
    } else {
        return NULL;
    }
}

PyObject *py_ped_constraint_solve_max(PyObject *s, PyObject *args) {
    PyObject *in_constraint;
    PedConstraint *out_constraint;
    PedGeometry *geometry;
    _ped_Geometry *ret;

    if (!PyArg_ParseTuple(args, "O", &in_constraint)) {
        return NULL;
    }

    out_constraint = _ped_Constraint2PedConstraint(in_constraint);

    ret = PyObject_New(_ped_Geometry, &_ped_Geometry_Type_obj);
    if (ret) {
        geometry = ped_constraint_solve_max(out_constraint);
        if (geometry) {
            ret = PedGeometry2_ped_Geometry(geometry);
        }
    }

    _free_PedConstraint(out_constraint);
    ped_geometry_destroy(geometry);

    if (ret) {
        return (PyObject *) ret;
    } else {
        return NULL;
    }
}

PyObject *py_ped_constraint_solve_nearest(PyObject *s, PyObject *args) {
    PyObject *in_constraint, *in_geometry;
    PedConstraint *out_constraint;
    PedGeometry *out_geometry;
    PedGeometry *geometry;
    _ped_Geometry *ret;

    if (!PyArg_ParseTuple(args, "OO", &in_constraint, &in_geometry)) {
        return NULL;
    }

    out_constraint = _ped_Constraint2PedConstraint(in_constraint);
    out_geometry = _ped_Geometry2PedGeometry(in_geometry);

    ret = PyObject_New(_ped_Geometry, &_ped_Geometry_Type_obj);
    if (ret) {
        geometry = ped_geometry(out_constraint, out_geometry);
        if (geometry) {
            ret = PedGeometry2_ped_Geometry(geometry);
        }
    }

    _free_PedConstraint(out_constraint);
    _free_PedGeometry(out_geometry);
    ped_geometry_destroy(geometry);

    if (ret) {
        return (PyObject *) ret;
    } else {
        return NULL;
    }
}

PyObject *py_ped_constraint_is_solution(PyObject *s, PyObject *args) {
    PyObject *in_constraint, *in_geometry;
    PedConstraint *out_constraint;
    PedGeometry *out_geometry;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "OO", &in_constraint, &in_geometry)) {
        return NULL;
    }

    out_constraint = _ped_Constraint2PedConstraint(in_constraint);
    out_geometry = _ped_Geometry2PedGeometry(in_geometry);

    ret = ped_constraint_is_solution(out_constraint, out_geometry);
    if (!ret) {
        return NULL;
    }

    _free_PedConstraint(out_constraint);
    _free_PedGeometry(out_geometry);

    return Py_BuildValue("i", ret);
}

PyObject *py_ped_constraint_any(PyObject *s, PyObject *args) {
    PyObject *in_device;
    PedDevice *out_device;
    PedConstraint *constraint;
    _ped_Constraint *ret;

    if (!PyArg_ParseTuple(args, "O", &in_device)) {
        return NULL;
    }

    out_device = _ped_Device2PedDevice(in_device);

    ret = PyObject_New(_ped_Constraint, &_ped_Constraint_Type_obj);
    if (ret) {
        constraint = ped_constraint_any(out_device);
        if (constraint) {
            ret = PedConstraint2_ped_Constraint(constraint);
        }
    }

    _free_PedDevice(out_device);
    ped_constraint_destroy(constraint);

    if (ret) {
        return (PyObject *) ret;
    } else {
        return NULL;
    }
}

PyObject *py_ped_constraint_exact(PyObject *s, PyObject *args) {
    PyObject *in_geometry;
    PedGeometry *out_geometry;
    PedConstraint *constraint;
    _ped_Constraint *ret;

    if (!PyArg_ParseTuple(args, "O", &in_geometry)) {
        return NULL;
    }

    out_geometry = _ped_Geometry2PedGeometry(in_geometry);

    ret = PyObject_New(_ped_Constraint, &_ped_Constraint_Type_obj);
    if (ret) {
        constraint = ped_constraint_exact(out_geometry);
        if (constraint) {
            ret = PedConstraint2_ped_Constraint(constraint);
        }
    }

    _free_PedGeometry(out_geometry);
    ped_constraint_destroy(constraint);

    if (ret) {
        return (PyObject *) ret;
    } else {
        return NULL;
    }
}
