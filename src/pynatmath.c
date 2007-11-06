/*
 * pynatmath.c
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
#include "pydevice.h"
#include "pynatmath.h"

/* _ped.Alignment functions */
void _ped_Alignment_dealloc(_ped_Alignment *self) {
    PyObject_Del(self);
}

PyObject *_ped_Alignment_new(PyTypeObject *type, PyObject *args,
                             PyObject *kwds) {
    _ped_Alignment *self;

    self = PyObject_New(_ped_Alignment, &_ped_Alignment_Type_obj);
    return (PyObject *) self;
}

int _ped_Alignment_init(_ped_Alignment *self, PyObject *args, PyObject *kwds) {
    /* XXX */
    return 0;
}

/* 1:1 function mappings for natmath.h in libparted */
PyObject *py_ped_round_up_to(PyObject *s, PyObject *args) {
    PyObject *in_sector, *in_grain_size;
    PedSector out_sector, out_grain_size, sector;
    _ped_Sector *ret;

    if (!PyArg_ParseTuple(args, "OO", &in_sector, &in_grain_size)) {
        return NULL;
    }

    out_sector = _ped_Sector2PedSector(in_sector);
    if (out_sector == -1) {
        return NULL;
    }

    out_grain_size = _ped_Sector2PedSector(in_grain_size);
    if (out_grain_size == -1) {
        return NULL;
    }

    sector = ped_round_up_to(out_sector, out_grain_size);
    if (sector) {
        ret = PedSector2_ped_Sector(sector);
    }

    return (PyObject *) ret;
}

PyObject *py_ped_round_down_to(PyObject *s, PyObject *args) {
    PyObject *in_sector, *in_grain_size;
    PedSector out_sector, out_grain_size, sector;
    _ped_Sector *ret;

    if (!PyArg_ParseTuple(args, "OO", &in_sector, &in_grain_size)) {
        return NULL;
    }

    out_sector = _ped_Sector2PedSector(in_sector);
    if (out_sector == -1) {
        return NULL;
    }

    out_grain_size = _ped_Sector2PedSector(in_grain_size);
    if (out_grain_size == -1) {
        return NULL;
    }

    sector = ped_round_down_to(out_sector, out_grain_size);
    if (sector) {
        ret = PedSector2_ped_Sector(sector);
    }

    return (PyObject *) ret;
}

PyObject *py_ped_round_to_nearest(PyObject *s, PyObject *args) {
    PyObject *in_sector, *in_grain_size;
    PedSector out_sector, out_grain_size, sector;
    _ped_Sector *ret;

    if (!PyArg_ParseTuple(args, "OO", &in_sector, &in_grain_size)) {
        return NULL;
    }

    out_sector = _ped_Sector2PedSector(in_sector);
    if (out_sector == -1) {
        return NULL;
    }

    out_grain_size = _ped_Sector2PedSector(in_grain_size);
    if (out_grain_size == -1) {
        return NULL;
    }

    sector = ped_round_to_nearest(out_sector, out_grain_size);
    if (sector) {
        ret = PedSector2_ped_Sector(sector);
    }

    return (PyObject *) ret;
}

PyObject *py_ped_greatest_common_divisor(PyObject *s, PyObject *args) {
    PyObject *in_a, *in_b;
    PedSector out_a, out_b, sector;
    _ped_Sector *ret;

    if (!PyArg_ParseTuple(args, "OO", &in_a, &in_b)) {
        return NULL;
    }

    out_a = _ped_Sector2PedSector(in_a);
    if (out_a == -1) {
        return NULL;
    }

    out_b = _ped_Sector2PedSector(in_b);
    if (out_b == -1) {
        return NULL;
    }

    sector = ped_greatest_common_divisor(out_a, out_b);
    if (sector) {
        ret = PedSector2_ped_Sector(sector);
    }

    return (PyObject *) ret;
}

PyObject *py_ped_alignment_init(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_alignment, *in_offset, *in_grain_size;
    PedAlignment *out_alignment;
    PedSector out_offset, out_grain_size;

    if (!PyArg_ParseTuple(args, "OOO", &in_alignment, &in_offset,
                                       &in_grain_size)) {
        return NULL;
    }

    out_alignment = _ped_Alignment2PedAlignment(in_alignment);
    if (out_alignment == NULL) {
        return NULL;
    }

    out_offset = _ped_Sector2PedSector(in_offset);
    if (out_offset == -1) {
        return NULL;
    }

    out_grain_size = _ped_Sector2PedSector(in_grain_size);
    if (out_grain_size == -1) {
        return NULL;
    }

    ret = ped_alignment_init(out_alignment, out_offset, out_grain_size);
    ped_alignment_destroy(out_alignment);
    return Py_BuildValue("i", ret);
}

PyObject *py_ped_alignment_new(PyObject *s, PyObject *args) {
    PyObject *in_offset, *in_grain_size;
    PedSector out_offset, out_grain_size;
    PedAlignment *align;
    _ped_Alignment *ret;

    if (!PyArg_ParseTuple(args, "OO", &in_offset, &in_grain_size)) {
        return NULL;
    }

    out_offset = _ped_Sector2PedSector(in_offset);
    if (out_offset == -1) {
        return NULL;
    }

    out_grain_size = _ped_Sector2PedSector(in_grain_size);
    if (out_grain_size == -1) {
        return NULL;
    }

    align = ped_alignment_new(out_offset, out_grain_size);
    if (align) {
        ret = PedAlignment2_ped_Alignment(align);
    }

    ped_alignment_destroy(align);
    return (PyObject *) ret;
}

PyObject *py_ped_alignment_destroy(PyObject *s, PyObject *args) {
    PyObject *in_alignment;
    PedAlignment *out_alignment;

    if (!PyArg_ParseTuple(args, "O", &in_alignment)) {
        return NULL;
    }

    out_alignment = _ped_Alignment2PedAlignment(in_alignment);
    if (out_alignment == NULL) {
        return NULL;
    }

    ped_alignment_destroy(out_alignment);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_alignment_duplicate(PyObject *s, PyObject *args) {
    PyObject *in_alignment;
    PedAlignment *out_alignment, *align;
    _ped_Alignment *ret;

    if (!PyArg_ParseTuple(args, "O", &in_alignment)) {
        return NULL;
    }

    out_alignment = _ped_Alignment2PedAlignment(in_alignment);
    if (out_alignment == NULL) {
        return NULL;
    }

    align = ped_alignment_duplicate(out_alignment);
    if (align) {
        ret = PedAlignment2_ped_Alignment(align);
    }

    ped_alignment_destroy(out_alignment);
    ped_alignment_destroy(align);

    return (PyObject *) ret;
}

PyObject *py_ped_alignment_intersect(PyObject *s, PyObject *args) {
    PyObject *in_a, *in_b;
    PedAlignment *out_a, *out_b, *align;
    _ped_Alignment *ret;

    if (!PyArg_ParseTuple(args, "OO", &in_a, &in_b)) {
        return NULL;
    }

    out_a = _ped_Alignment2PedAlignment(in_a);
    if (out_a == NULL) {
        return NULL;
    }

    out_b = _ped_Alignment2PedAlignment(in_b);
    if (out_b == NULL) {
        return NULL;
    }

    align = ped_alignment_intersect(out_a, out_b);
    if (align) {
        ret = PedAlignment2_ped_Alignment(align);
    }

    ped_alignment_destroy(out_a);
    ped_alignment_destroy(out_b);
    ped_alignment_destroy(align);

    return (PyObject *) ret;
}

PyObject *py_ped_alignment_align_up(PyObject *s, PyObject *args) {
    PyObject *in_align, *in_geom, *in_sector;
    PedAlignment *out_align;
    PedGeometry *out_geom;
    PedSector out_sector, sector;
    _ped_Sector *ret;

    if (!PyArg_ParseTuple(args, "OOO", &in_align, &in_geom, &in_sector)) {
        return NULL;
    }

    out_align = _ped_Alignment2PedAlignment(in_align);
    if (out_align == NULL) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    out_sector = _ped_Sector2PedSector(in_sector);
    if (out_sector == -1) {
        return NULL;
    }

    sector = ped_alignment_align_up(out_align, out_geom, out_sector);
    if (sector) {
        ret = PedSector2_ped_Sector(sector);
    }

    ped_alignment_destroy(out_align);
    ped_geometry_destroy(out_geom);

    return (PyObject *) ret;
}

PyObject *py_ped_alignment_align_down(PyObject *s, PyObject *args) {
    PyObject *in_align, *in_geom, *in_sector;
    PedAlignment *out_align;
    PedGeometry *out_geom;
    PedSector out_sector, sector;
    _ped_Sector *ret;

    if (!PyArg_ParseTuple(args, "OOO", &in_align, &in_geom, &in_sector)) {
        return NULL;
    }

    out_align = _ped_Alignment2PedAlignment(in_align);
    if (out_align == NULL) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    out_sector = _ped_Sector2PedSector(in_sector);
    if (out_sector == -1) {
        return NULL;
    }

    sector = ped_alignment_align_down(out_align, out_geom, out_sector);
    if (sector) {
        ret = PedSector2_ped_Sector(sector);
    }

    ped_alignment_destroy(out_align);
    ped_geometry_destroy(out_geom);

    return (PyObject *) ret;
}

PyObject *py_ped_alignment_align_nearest(PyObject *s, PyObject *args) {
    PyObject *in_align, *in_geom, *in_sector;
    PedAlignment *out_align;
    PedGeometry *out_geom;
    PedSector out_sector, sector;
    _ped_Sector *ret;

    if (!PyArg_ParseTuple(args, "OOO", &in_align, &in_geom, &in_sector)) {
        return NULL;
    }

    out_align = _ped_Alignment2PedAlignment(in_align);
    if (out_align == NULL) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    out_sector = _ped_Sector2PedSector(in_sector);
    if (out_sector == -1) {
        return NULL;
    }

    sector = ped_alignment_align_nearest(out_align, out_geom, out_sector);
    if (sector) {
        ret = PedSector2_ped_Sector(sector);
    }

    ped_alignment_destroy(out_align);
    ped_geometry_destroy(out_geom);

    return (PyObject *) ret;
}

PyObject *py_ped_alignment_is_aligned(PyObject *s, PyObject *args) {
    int ret = -1;
    PyObject *in_align, *in_geom, *in_sector;
    PedAlignment *out_align;
    PedGeometry *out_geom;
    PedSector out_sector;

    if (!PyArg_ParseTuple(args, "OOO", &in_align, &in_geom, &in_sector)) {
        return NULL;
    }

    out_align = _ped_Alignment2PedAlignment(in_align);
    if (out_align == NULL) {
        return NULL;
    }

    out_geom = _ped_Geometry2PedGeometry(in_geom);
    if (out_geom == NULL) {
        return NULL;
    }

    out_sector = _ped_Sector2PedSector(in_sector);
    if (out_sector == -1) {
        return NULL;
    }

    ret = ped_alignment_is_aligned(out_align, out_geom, out_sector);
    ped_alignment_destroy(out_align);
    ped_geometry_destroy(out_geom);
    return PyBool_FromLong(ret);
}

PyObject *py_ped_div_round_up(PyObject *s, PyObject *args) {
    PyObject *in_numerator, *in_divisor;
    PedSector out_numerator, out_divisor, sector;
    _ped_Sector *ret;

    if (!PyArg_ParseTuple(args, "OO", &in_numerator, &in_divisor)) {
        return NULL;
    }

    out_numerator = _ped_Sector2PedSector(in_numerator);
    if (out_numerator == -1) {
        return NULL;
    }

    out_divisor = _ped_Sector2PedSector(in_divisor);
    if (out_divisor == -1) {
        return NULL;
    }

    sector = ped_div_round_up(out_numerator, out_divisor);
    if (sector) {
        ret = PedSector2_ped_Sector(sector);
    }

    return (PyObject *) ret;
}

PyObject *py_ped_div_round_to_nearest(PyObject *s, PyObject *args) {
    PyObject *in_numerator, *in_divisor;
    PedSector out_numerator, out_divisor, sector;
    _ped_Sector *ret;

    if (!PyArg_ParseTuple(args, "OO", &in_numerator, &in_divisor)) {
        return NULL;
    }

    out_numerator = _ped_Sector2PedSector(in_numerator);
    if (out_numerator == -1) {
        return NULL;
    }

    out_divisor = _ped_Sector2PedSector(in_divisor);
    if (out_divisor == -1) {
        return NULL;
    }

    sector = ped_div_round_to_nearest(out_numerator, out_divisor);
    if (sector) {
        ret = PedSector2_ped_Sector(sector);
    }

    return (PyObject *) ret;
}
