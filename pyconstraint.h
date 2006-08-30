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
 *
*/

#ifndef PYCONSTRAINT_H_INCLUDED
#define PYCONSTRAINT_H_INCLUDED

#include <Python.h>
#include <parted/constraint.h>
#include "pydevice.h"

typedef struct _PyPedConstraint PyPedConstraint;

struct _PyPedConstraint {
        PyObject_HEAD;
        PedConstraint *constraint;
        PyPedDevice *dev;
        int borrowed;
};

extern PyTypeObject PyPedConstraintType;

extern PyPedConstraint * py_ped_constraint_obj_new (PedConstraint *c,
                                                    PyPedDevice *dev,
                                                    int borrowed);

#endif /* PYCONSTRAINT_H_INCLUDED */
