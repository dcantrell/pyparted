/*
 * convert.h
 * Functions for converting to/from Python _ped types and C libparted types
 *
 * Copyright The pyparted Project Authors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef CONVERT_H_INCLUDED
#define CONVERT_H_INCLUDED

#include <parted/parted.h>

#include "pyconstraint.h"
#include "pydevice.h"
#include "pydisk.h"
#include "pyfilesys.h"
#include "pygeom.h"
#include "pynatmath.h"
#include "pytimer.h"

#define TP_FLAGS (Py_TPFLAGS_HAVE_GC | Py_TPFLAGS_BASETYPE)

PedAlignment *_ped_Alignment2PedAlignment(PyObject *);
_ped_Alignment *PedAlignment2_ped_Alignment(PedAlignment *);

PedConstraint *_ped_Constraint2PedConstraint(PyObject *);
_ped_Constraint *PedConstraint2_ped_Constraint(PedConstraint *);

PedDevice *_ped_Device2PedDevice(PyObject *);
_ped_Device *PedDevice2_ped_Device(PedDevice *);

PedDisk *_ped_Disk2PedDisk(PyObject *);
_ped_Disk *PedDisk2_ped_Disk(PedDisk *);

PedDiskType *_ped_DiskType2PedDiskType(PyObject *);
_ped_DiskType *PedDiskType2_ped_DiskType(const PedDiskType *);

PedFileSystem *_ped_FileSystem2PedFileSystem(PyObject *);
_ped_FileSystem *PedFileSystem2_ped_FileSystem(PedFileSystem *);

PedFileSystemType *_ped_FileSystemType2PedFileSystemType(PyObject *);
_ped_FileSystemType *PedFileSystemType2_ped_FileSystemType(const PedFileSystemType *);

PedGeometry *_ped_Geometry2PedGeometry(PyObject *);
_ped_Geometry *PedGeometry2_ped_Geometry(PedGeometry *);

PedCHSGeometry *_ped_CHSGeometry2PedCHSGeometry(PyObject *);
_ped_CHSGeometry *PedCHSGeometry2_ped_CHSGeometry(PedCHSGeometry *);

PedPartition *_ped_Partition2PedPartition(_ped_Partition *);
_ped_Partition *PedPartition2_ped_Partition(PedPartition *, _ped_Disk *);

PedTimer *_ped_Timer2PedTimer(PyObject *);
_ped_Timer *PedTimer2_ped_Timer(PedTimer *);

#endif /* CONVERT_H_INCLUDED */
