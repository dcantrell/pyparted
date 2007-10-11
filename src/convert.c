/*
 * convert.c
 * Convert _ped Python types to libparted typedefs.  Only typedef structs
 * need to be converted.  When a typedef in libparted is a primitive type,
 * we can just use it directly.
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
#include "pydevice.h"
#include "pygeom.h"
#include "pynatmath.h"
#include "pyunit.h"

/* Note... all of these functions malloc for memory to create the returning
 * type.  It is up to the caller to free the memory after using it.
 */

/* _ped_Alignment -> PedAlignment functions */
PedAlignment *_ped_Alignment2PedAlignment(PyObject *s) {
    PedAlignment *ret;
    _ped_Alignment *alignment = (_ped_Alignment *) s;

    if (alignment == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Alignment()");
        return NULL;
    }

    ret = malloc(sizeof(PedAlignment));
    if (ret == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Out of memory");
        return NULL;
    }

    ret->offset = _ped_Sector2PedSector(alignment->offset);
    if (ret->offset == -1) {
        return NULL;
    }

    ret->grain_size=_ped_Sector2PedSector(alignment->grain_size);
    if (ret->grain_size == -1) {
        return NULL;
    }

    return ret;
}

void _free_PedAlignment(PedAlignment *alignment) {
    if (alignment != NULL) {
        free(alignment);
        alignment = NULL;
    }

    return;
}

/* _ped_Constraint -> PedConstraint functions */
PedConstraint *_ped_Constraint2PedConstraint(PyObject *s) {
    PedConstraint *ret;
    _ped_Constraint *constraint = (_ped_Constraint *) s;

    if (constraint == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Constraint()");
        return NULL;
    }

    ret = malloc(sizeof(PedConstraint));
    if (ret == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Out of memory");
        return NULL;
    }

    ret->start_align = _ped_Alignment2PedAlignment(constraint->start_align);
    if (ret->start_align == NULL) {
        return NULL;
    }

    ret->end_align = _ped_Alignment2PedAlignment(constraint->end_align);
    if (ret->end_align == NULL) {
        return NULL;
    }

    ret->start_range = _ped_Geometry2PedGeometry(constraint->start_range);
    if (ret->start_range == NULL) {
        return NULL;
    }

    ret->end_range = _ped_Geometry2PedGeometry(constraint->end_range);
    if (ret->end_range == NULL) {
        return NULL;
    }

    ret->min_size = _ped_Sector2PedSector(constraint->min_size);
    if (ret->min_size == -1) {
        return NULL;
    }

    ret->max_size = _ped_Sector2PedSector(constraint->max_size);
    if (ret->max_size == -1) {
        return NULL;
    }

    return ret;
}

_ped_Constraint *PedConstraint2_ped_Constraint(PedConstraint *constraint) {
    /* FIXME */
    return NULL;
}

void _free_PedConstraint(PedConstraint *constraint) {
    if (constraint != NULL) {
        _free_PedAlignment(constraint->start_align);
        _free_PedAlignment(constraint->end_align);
        _free_PedGeometry(constraint->start_range);
        _free_PedGeometry(constraint->end_range);
        free(constraint);
        constraint = NULL;
    }

    return;
}

/* _ped_Device -> PedDevice functions */
PedDevice *_ped_Device2PedDevice(PyObject *s) {
    PedDevice *ret;
    _ped_Device *dev = (_ped_Device *) s;

    if (dev == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Device()");
        return NULL;
    }

    ret = malloc(sizeof(PedDevice));
    if (ret == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Out of memory");
        return NULL;
    }

    ret->next = dev->next;

    if (dev->model) {
        ret->model = strdup(dev->model);
    } else {
        ret->model = NULL;
    }

    if (dev->path) {
        ret->path = strdup(dev->path);
    } else {
        ret->path = NULL;
    }

    ret->type = dev->type;
    ret->sector_size = dev->sector_size;
    ret->phys_sector_size = dev->phys_sector_size;

    ret->length = _ped_Sector2PedSector(dev->length);
    if (ret->length == -1) {
        return NULL;
    }

    ret->open_count = dev->open_count;
    ret->read_only = dev->read_only;
    ret->external_mode = dev->external_mode;
    ret->dirty = dev->dirty;
    ret->boot_dirty = dev->boot_dirty;

    _ped_CHSGeometry2PedCHSGeometry(dev->hw_geom, &ret->hw_geom);
    _ped_CHSGeometry2PedCHSGeometry(dev->bios_geom, &ret->bios_geom);

    ret->host = dev->host;
    ret->did = dev->did;
    ret->arch_specific = dev->arch_specific;
    return ret;
}

void _free_PedDevice(PedDevice *dev) {
    if (dev != NULL) {
        if (dev->model) {
            free(dev->model);
            dev->model = NULL;
        }

        if (dev->path) {
            free(dev->path);
            dev->path = NULL;
        }

        free(dev);
        dev = NULL;
    }

    return;
}

/* _ped_Geometry -> PedGeometry functions */
PedGeometry *_ped_Geometry2PedGeometry(PyObject *s) {
    PedGeometry *ret;
    _ped_Geometry *geometry = (_ped_Geometry *) s;

    if (geometry == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Geometry()");
        return NULL;
    }

    ret = malloc(sizeof(PedGeometry));
    if (ret == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Out of memory");
        return NULL;
    }

    ret->dev = _ped_Device2PedDevice(geometry->dev);
    if (ret->dev == NULL) {
        return NULL;
    }

    ret->start = _ped_Sector2PedSector(geometry->start);
    if (ret->start == -1) {
        return NULL;
    }

    ret->length = _ped_Sector2PedSector(geometry->length);
    if (ret->start == -1) {
        return NULL;
    }

    ret->end = _ped_Sector2PedSector(geometry->end);
    if (ret->end == -1) {
        return NULL;
    }

    return ret;
}

_ped_Geometry *PedGeometry2_ped_Geometry(PedGeometry *geometry) {
    /* FIXME */
   return NULL;
}

void _free_PedGeometry(PedGeometry *geometry) {
    if (geometry != NULL) {
        _free_PedDevice(geometry->dev);
        free(geometry);
        geometry = NULL;
    }

    return;
}

/* _ped_CHSGeometry -> PedCHSGeometry functions */
void _ped_CHSGeometry2PedCHSGeometry(PyObject *s, PedCHSGeometry *destgeom) {
    _ped_CHSGeometry *srcgeom = (_ped_CHSGeometry *) s;

    if (srcgeom == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.CHSGeometry()");
        return;
    }

    destgeom->cylinders = srcgeom->cylinders;
    destgeom->heads = srcgeom->heads;
    destgeom->sectors = srcgeom->sectors;
    return;
}

/* _ped_Sector -> PedSector functions */
PedSector _ped_Sector2PedSector(PyObject *s) {
    PedSector ret;
    _ped_Sector *sector = (_ped_Sector *) s;

    if (sector == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Sector()");
        return -1;
    }

    ret = sector->val;
    return ret;
}

/* PedSector -> _ped_Sector functions */
_ped_Sector *PedSector2_ped_Sector(PedSector s) {
    _ped_Sector *ret;

    ret = PyObject_New(_ped_Sector, &_ped_Sector_Type_obj);
    ret->val = s;
    return ret;
}

/* _ped_Unit -> PedUnit functions */
PedUnit _ped_Unit2PedUnit(PyObject *s) {
    PedUnit ret;
    _ped_Unit *unit = (_ped_Unit *) s;

    if (unit == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped_Unit()");
        return -1;
    }

    ret = unit->val;
    return ret;
}

/* PedUnit -> _ped_Unit function s*/
_ped_Unit *PedUnit2_ped_Unit(PedUnit s) {
    _ped_Unit *ret;

    ret = PyObject_New(_ped_Unit, &_ped_Unit_Type_obj);
    ret->val = s;
    return ret;
}
