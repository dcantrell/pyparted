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

/* Note... all of these functions malloc for memory to create the returning
 * type.  It is up to the caller to free the memory after using it.
 */

/* _ped_Alignment -> PedAlignment functions */
PedAlignment *_ped_Alignment2PedAlignment(_ped_Alignment *alignment) {
    PedAlignment *ret;

    /* FIXME */
    return NULL;
}

/* _ped_Constraint -> PedConstraint functions */
PedConstraint *_ped_Constraint2PedConstraint(_ped_Constraint *constraint) {
    PedConstraint *ret;

    /* FIXME */
    return NULL;
}

/* _ped_Device -> PedDevice functions */
PedDevice *_ped_Device2PedDevice(_ped_Device *dev) {
    PedDevice *ret;

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

    /* dev->length is a _ped_Sector, so read the val attribute */
    ret->length = dev->length->val;

    ret->open_count = dev->open_count;
    ret->read_only = dev->read_only;
    ret->external_mode = dev->external_mode;
    ret->dirty = dev->dirty;
    ret->boot_dirty = dev->boot_dirty;

    /* dev->hw_geom and dev->bios_geom are _ped_Geometry objects */
    ret->hw_geom.cylinders = dev->hw_geom->cylinders;
    ret->hw_geom.heads = dev->hw_geom->heads;
    ret->hw_geom.sectors = dev->hw_geom->sectors;
    ret->bios_geom.cylinders = dev->bios_geom.cylinders;
    ret->bios_geom.heads = dev->bios_geom.heads;
    ret->bios_geom.sectors = dev->bios_geom.sectors;

    ret->host = dev->host;
    ret->did = dev->did;
    ret->arch_specific = dev->arch_specific;
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
PedGeometry *_ped_Geometry2PedGeometry(_ped_Geometry *geometry) {
    PedDevice *dev;
    PedGeometry *ret;

    if (geometry == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Geometry()");
        return NULL;
    }

    /* FIXME */
    ret = malloc(sizeof(PedGeometry));
    if (ret == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Out of memory");
        return NULL;
    }





    return NULL;
}

/* _ped_Sector -> PedSector functions */
PedSector _ped_Sector2PedSector(_ped_Sector *sector) {
    PedSector ret;

    if (sector == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Sector()");
        return -1;
    }

    ret = sector->val;
    return ret;
}
