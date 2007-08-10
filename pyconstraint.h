/*
 * pyconstraint.h
 *
 * Copyright (C) 2000  Red Hat, Inc.  All rights reserved.
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
 * Red Hat Author(s): Matt Wilson
 *                    David Cantrell
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
