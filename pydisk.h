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
