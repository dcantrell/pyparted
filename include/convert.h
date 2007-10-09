/*
 * convert.h
 * Functions to convert from Python _ped types to C libparted types
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

#ifndef CONVERT_H_INCLUDED
#define CONVERT_H_INCLUDED

#include <parted/parted.h>

#include "pyconstraint.h"
#include "pydevice.h"
#include "pygeom.h"
#include "pynatmath.h"

PedAlignment *_ped_Alignment2PedAlignment(PyObject *s);
void _free_PedAlignment(PedAlignment *alignment);

PedConstraint *_ped_Constraint2PedConstraint(PyObject *s);
void _free_PedConstraint(PedConstraint *constraint);

PedDevice *_ped_Device2PedDevice(PyObject *s);
void _free_PedDevice(PedDevice *dev);

PedGeometry *_ped_Geometry2PedGeometry(PyObject *s);
void _free_PedGeometry(PedGeometry *geometry);

void _ped_CHSGeometry2PedCHSGeometry(PyObject *s, PedCHSGeometry *destgeom);

PedSector _ped_Sector2PedSector(PyObject *s);

PedUnit _ped_Unit2PedUnit(PyObject *s);

#endif /* CONVERT_H_INCLUDED */
