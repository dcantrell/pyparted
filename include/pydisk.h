/*
 * pydisk.h
 * pyparted type definitions for pydisk.c
 *
 * Copyright (C) 2007-2013 Red Hat, Inc.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions of
 * the GNU General Public License v.2, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY expressed or implied, including the implied warranties of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.  You should have received a copy of the
 * GNU General Public License along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.  Any Red Hat trademarks that are incorporated in the
 * source code or documentation are not subject to the GNU General Public
 * License and may only be used or replicated with the express permission of
 * Red Hat, Inc.
 *
 * Author(s): David Cantrell <dcantrell@redhat.com>
 *            Chris Lumens <clumens@redhat.com>
 *            Alex Skinner <alex@lx.lc>
 */

#ifndef PYDISK_H_INCLUDED
#define PYDISK_H_INCLUDED

#include <Python.h>

#include <parted/parted.h>

/* _ped.Partition type is the Python equivalent of PedPartition
 * in libparted */
typedef struct {
    PyObject_HEAD

    /* PedPartition members */
    PyObject *disk;                /* _ped.Disk */
    PyObject *geom;                /* _ped.Geometry */
    int type;                      /* PedPartitionType */
    PyObject *fs_type;             /* _ped.FileSystemType */

    /* store the PedPartition from libparted */
    PedPartition *ped_partition;

    int _owned;                    /* Belongs to a Disk or not */
} _ped_Partition;

void _ped_Partition_dealloc(_ped_Partition *);
int _ped_Partition_compare(_ped_Partition *, PyObject *);
PyObject *_ped_Partition_richcompare(_ped_Partition *, PyObject *, int);
PyObject *_ped_Partition_str(_ped_Partition *);
int _ped_Partition_traverse(_ped_Partition *, visitproc, void *);
int _ped_Partition_clear(_ped_Partition *);
int _ped_Partition_init(_ped_Partition *, PyObject *, PyObject *);
PyObject *_ped_Partition_get(_ped_Partition *, void *);
int _ped_Partition_set(_ped_Partition *, PyObject *, void *);

extern PyTypeObject _ped_Partition_Type_obj;

/* _ped.Disk type is the Python equivalent of PedDisk in libparted */
typedef struct {
    PyObject_HEAD

    /* PedDisk members */
    PyObject *dev;             /* _ped.Device */
    PyObject *type;            /* _ped.DiskType */

    /* store the PedDisk from libparted */
    PedDisk *ped_disk;
} _ped_Disk;

void _ped_Disk_dealloc(_ped_Disk *);
int _ped_Disk_compare(_ped_Disk *, PyObject *);
PyObject *_ped_Disk_richcompare(_ped_Disk *, PyObject *, int);
PyObject *_ped_Disk_str(_ped_Disk *);
int _ped_Disk_traverse(_ped_Disk *, visitproc, void *);
int _ped_Disk_clear(_ped_Disk *);
int _ped_Disk_init(_ped_Disk *, PyObject *, PyObject *);

extern PyTypeObject _ped_Disk_Type_obj;

/* _ped.DiskType type is the Python equivalent of PedDiskType in libparted */
typedef struct {
    PyObject_HEAD

    /* PedDiskType members */
    char *name;
    long long features;        /* PedDiskTypeFeature */
} _ped_DiskType;

void _ped_DiskType_dealloc(_ped_DiskType *);
int _ped_DiskType_compare(_ped_DiskType *, PyObject *);
PyObject *_ped_DiskType_richcompare(_ped_DiskType *, PyObject *, int);
PyObject *_ped_DiskType_str(_ped_DiskType *);
int _ped_DiskType_traverse(_ped_DiskType *, visitproc, void *);
int _ped_DiskType_clear(_ped_DiskType *);
PyObject *_ped_DiskType_get(_ped_DiskType *, void *);

extern PyTypeObject _ped_DiskType_Type_obj;

/* 1:1 function mappings for disk.h in libparted */
PyObject *py_ped_disk_type_get_next(PyObject *, PyObject *);
PyObject *py_ped_disk_type_get(PyObject *, PyObject *);
PyObject *py_ped_disk_type_check_feature(PyObject *, PyObject *);
PyObject *py_ped_disk_clobber(PyObject *, PyObject *);
PyObject *py_ped_disk_duplicate(PyObject *, PyObject *);
PyObject *py_ped_disk_destroy(PyObject *, PyObject *);
PyObject *py_ped_disk_commit(PyObject *, PyObject *);
PyObject *py_ped_disk_commit_to_dev(PyObject *, PyObject *);
PyObject *py_ped_disk_commit_to_os(PyObject *, PyObject *);
PyObject *py_ped_disk_check(PyObject *, PyObject *);
PyObject *py_ped_disk_print(PyObject *, PyObject *);
PyObject *py_ped_disk_get_primary_partition_count(PyObject *, PyObject *);
PyObject *py_ped_disk_get_last_partition_num(PyObject *, PyObject *);
PyObject *py_ped_disk_get_max_primary_partition_count(PyObject *, PyObject *);
PyObject *py_ped_disk_get_max_supported_partition_count(PyObject *, PyObject *);
PyObject *py_ped_disk_get_partition_alignment(PyObject *, PyObject *);
PyObject *py_ped_disk_max_partition_length(PyObject *, PyObject *);
PyObject *py_ped_disk_max_partition_start_sector(PyObject *, PyObject *);
PyObject *py_ped_disk_set_flag(PyObject *, PyObject *);
PyObject *py_ped_disk_get_flag(PyObject *, PyObject *);
PyObject *py_ped_disk_is_flag_available(PyObject *, PyObject *);
PyObject *py_ped_disk_flag_get_name(PyObject *, PyObject *);
PyObject *py_ped_disk_flag_get_by_name(PyObject *, PyObject *);
PyObject *py_ped_disk_flag_next(PyObject *, PyObject *);
PyObject *py_ped_partition_destroy(_ped_Partition *, PyObject *);
PyObject *py_ped_partition_is_active(_ped_Partition *, PyObject *);
PyObject *py_ped_partition_set_flag(_ped_Partition *, PyObject *);
PyObject *py_ped_partition_get_flag(_ped_Partition *, PyObject *);
PyObject *py_ped_partition_is_flag_available(_ped_Partition *, PyObject *);
PyObject *py_ped_partition_set_system(_ped_Partition *, PyObject *);
PyObject *py_ped_partition_set_name(_ped_Partition *, PyObject *);
PyObject *py_ped_partition_get_name(_ped_Partition *, PyObject *);
PyObject *py_ped_partition_is_busy(_ped_Partition *, PyObject *);
PyObject *py_ped_partition_get_path(_ped_Partition *, PyObject *);
PyObject *py_ped_partition_reset_num(_ped_Partition *, PyObject *);
PyObject *py_ped_partition_type_get_name(PyObject *, PyObject *);
PyObject *py_ped_partition_flag_get_name(PyObject *, PyObject *);
PyObject *py_ped_partition_flag_get_by_name(PyObject *, PyObject *);
PyObject *py_ped_partition_flag_next(PyObject *, PyObject *);
PyObject *py_ped_disk_add_partition(PyObject *, PyObject *);
PyObject *py_ped_disk_remove_partition(PyObject *, PyObject *);
PyObject *py_ped_disk_delete_partition(PyObject *, PyObject *);
PyObject *py_ped_disk_delete_all(PyObject *, PyObject *);
PyObject *py_ped_disk_set_partition_geom(PyObject *, PyObject *);
PyObject *py_ped_disk_maximize_partition(PyObject *, PyObject *);
PyObject *py_ped_disk_get_max_partition_geometry(PyObject *, PyObject *);
PyObject *py_ped_disk_minimize_extended_partition(PyObject *, PyObject *);
PyObject *py_ped_disk_next_partition(PyObject *, PyObject *);
PyObject *py_ped_disk_get_partition(PyObject *, PyObject *);
PyObject *py_ped_disk_get_partition_by_sector(PyObject *, PyObject *);
PyObject *py_ped_disk_extended_partition(PyObject *, PyObject *);
PyObject *py_ped_disk_new_fresh(PyObject *, PyObject *);
PyObject *py_ped_disk_new(PyObject *, PyObject *);

#endif /* PYDISK_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4
 */
