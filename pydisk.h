/*
 * pydisk.h
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

#ifndef PYDISK_H_INCLUDED
#define PYDISK_H_INCLUDED

#include <Python.h>

#include <parted/constraint.h>
#include <parted/disk.h>

/* disk object */

typedef struct _PyPedDisk PyPedDisk;

struct _PyPedDisk {
        PyObject_HEAD;
        PedDisk *disk;
        int borrowed;
        int closed;
};

extern PyTypeObject PyPedDiskType;

extern PyPedDisk * py_ped_disk_obj_new (PedDisk *disk,
                                        int borrowed);

/* disk type (partitioning method) object */

typedef struct _PyPedDiskTypeObj PyPedDiskTypeObj;

struct _PyPedDiskTypeObj {
        PyObject_HEAD;
        PedDiskType *type;
};

extern PyTypeObject PyPedDiskTypeType;

extern PyPedDiskTypeObj * py_ped_disk_type_obj_new (const PedDiskType *type);

/* partition object */

typedef struct _PyPedPartition PyPedPartition;

struct _PyPedPartition {
        PyObject_HEAD;
        PedPartition *part;
        PyPedDisk *disk;
        int borrowed;
};

extern PyTypeObject PyPedPartitionType;

extern PyPedPartition * py_ped_partition_obj_new (PedPartition *part,
                                                  PyPedDisk *disk,
                                                  int borrowed);

/* constructor type for parted.PedDisk.open(dev) */

extern PyTypeObject PyPedDiskConstructorType;

#endif /* PYDISK_H_INCLUDED */
