/*
 * convert.h
 * Functions for converting to/from Python _ped types and C libparted types
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
 * Red Hat Author(s): David Cantrell <dcantrell@redhat.com>
 *                    Chris Lumens <clumens@redhat.com>
 */

#ifndef CONVERT_H_INCLUDED
#define CONVERT_H_INCLUDED

#include <parted/parted.h>

#include "pyconstraint.h"
#include "pydevice.h"
#include "pyfilesys.h"
#include "pygeom.h"
#include "pynatmath.h"
#include "pytimer.h"
#include "pyunit.h"

PedAlignment *_ped_Alignment2PedAlignment(PyObject *s);
_ped_Alignment *PedAlignment2_ped_Alignment(PedAlignment *alignment);

PedConstraint *_ped_Constraint2PedConstraint(PyObject *s);
_ped_Constraint *PedConstraint2_ped_Constraint(PedConstraint *constraint);

PedDevice *_ped_Device2PedDevice(PyObject *s);
_ped_Device *PedDevice2_ped_Device(PedDevice *device);

PedFileSystem *_ped_FileSystem2PedFileSystem(PyObject *s);
_ped_FileSystem *PedFileSystem2_ped_FileSystem(PedFileSystem *fs);

PedFileSystemType *_ped_FileSystemType2PedFileSystemType(PyObject *s);
_ped_FileSystemType *PedFileSystemType2_ped_FileSystemType(PedFileSystemType *fstype);

PedGeometry *_ped_Geometry2PedGeometry(PyObject *s);
_ped_Geometry *PedGeometry2_ped_Geometry(PedGeometry *geometry);

PedCHSGeometry *_ped_CHSGeometry2PedCHSGeometry(PyObject *s);
_ped_CHSGeometry *PedCHSGeometry2_ped_CHSGeometry(PedCHSGeometry *geom);

PedSector _ped_Sector2PedSector(PyObject *s);
_ped_Sector *PedSector2_ped_Sector(PedSector s);

PedTimer *_ped_Timer2PedTimer(PyObject *s);
_ped_Timer *PedTimer2_ped_Timer(PedTimer *timer);

PedUnit _ped_Unit2PedUnit(PyObject *s);
_ped_Unit *PedUnit2_ped_Unit(PedUnit s);

#endif /* CONVERT_H_INCLUDED */
