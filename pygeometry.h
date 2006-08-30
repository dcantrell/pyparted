/* -*- Mode: c; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*-
 * Matt Wilson <msw@redhat.com>
 *
 * Copyright 2000 Red Hat, Inc.
 *
 * This software may be freely redistributed under the terms of the GNU
 * library public license.
 *
 * You should have received a copy of the GNU Library Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef PYGEOMETRY_H_INCLUDED
#define PYGEOMETRY_H_INCLUDED

#include <Python.h>

#include <parted/constraint.h>
#include <parted/geom.h>
#include "pydevice.h"

typedef struct _PyPedGeometry PyPedGeometry;

struct _PyPedGeometry {
        PyObject_HEAD;
        PedGeometry* geom;
        PyPedDevice *dev;
        int borrowed;
};

extern PyTypeObject PyPedGeometryType;

PyPedGeometry *
py_ped_geometry_obj_new (PedGeometry* geom, PyPedDevice *disk, int borrowed);

#endif /* PYGEOMETRY_H_INCLUDED */

