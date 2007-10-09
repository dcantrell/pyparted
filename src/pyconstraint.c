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
    self->ob_type->tp_free((PyObject *) self);
}

PyObject *_ped_Constraint_new(PyTypeObject *type, PyObject *args,
                              PyObject *kwds) {
    _ped_Constraint *self;

    self = (_ped_Constraint *) type->tp_alloc(type, 0);
    return (PyObject *) self;
}

int _ped_Constraint_init(_ped_Constraint *self, PyObject *args,
                         PyObject *kwds) {
    /* FIXME */
    return 0;
}

/* 1:1 function mappings for constraint.h in libparted */
PyObject *py_ped_constraint_init(PyObject *s, PyObject *args) {
    _ped_Constraint *in_constraint;
    _ped_Alignment *in_start_align, *in_end_align;
    _ped_Geometry *in_start_range, *in_end_range;
    _ped_Sector in_min_size, in_max_size;
    PedConstraint *out_constraint;
    PedAlignment *out_start_align, *out_end_align;
    PedGeometry *out_start_range, *out_end_range;
    PedSector out_min_size, out_max_size;

    if (!PyArg_ParseTuple(args, "OOOOOOO", (_ped_Constraint *) &in_constraint,
                                           (_ped_Alignment *) &in_start_align,
                                           (_ped_Alignment *) &in_end_align,
                                           (_ped_Geometry *) &in_start_range,
                                           (_ped_Geometry *) &in_end_range,
                                           (_ped_Sector *) &in_min_size,
                                           (_ped_Sector *) &in_max_size)) {
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
    free(out_constraint);
    free(out_start_align);
    free(out_end_align);
    free(out_start_range);
    free(out_end_range);
    return Py_BuildValue("i", ret);
}

PyObject *py_ped_constraint_new(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_constraint_new_from_min_max(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_constraint_new_from_min(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_constraint_new_from_max(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_constraint_duplicate(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_constraint_done(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_constraint_destroy(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_constraint_intersect(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_constraint_solve_max(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_constraint_solve_nearest(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_constraint_is_solution(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_constraint_any(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_constraint_exact(PyObject *s, PyObject *args) {
    /* FIXME */
    Py_INCREF(Py_None);
    return Py_None;
}
