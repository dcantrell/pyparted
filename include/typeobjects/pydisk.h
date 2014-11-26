/*
 * pydisk.h
 * pyparted type objects for pydisk.c
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

#ifndef TYPEOBJECTS_PYDISK_H_INCLUDED
#define TYPEOBJECTS_PYDISK_H_INCLUDED

#include <Python.h>
#include <structmember.h>

/* _ped.Partition type object */
static PyMemberDef _ped_Partition_members[] = {
    {"disk", T_OBJECT, offsetof(_ped_Partition, disk), READONLY,
             "The _ped.Disk this Partition exists on."},
    {"geom", T_OBJECT, offsetof(_ped_Partition, geom), READONLY,
             "A _ped.Geometry object describing the region this Partition occupies."},
    {"fs_type", T_OBJECT, offsetof(_ped_Partition, fs_type), READONLY,
                "A _ped.FileSystemType object describing the filesystem on this Partition."},
    {NULL}
};

static PyMethodDef _ped_Partition_methods[] = {
    {"destroy", (PyCFunction) py_ped_partition_destroy, METH_VARARGS,
                partition_destroy_doc},
    {"is_active", (PyCFunction) py_ped_partition_is_active, METH_VARARGS,
                  partition_is_active_doc},
    {"set_flag", (PyCFunction) py_ped_partition_set_flag, METH_VARARGS,
                 partition_set_flag_doc},
    {"get_flag", (PyCFunction) py_ped_partition_get_flag, METH_VARARGS,
                 partition_get_flag_doc},
    {"is_flag_available", (PyCFunction) py_ped_partition_is_flag_available,
                          METH_VARARGS, partition_is_flag_available_doc},
    {"set_system", (PyCFunction) py_ped_partition_set_system,
                   METH_VARARGS, partition_set_system_doc},
    {"set_name", (PyCFunction) py_ped_partition_set_name, METH_VARARGS,
                 partition_set_name_doc},
    {"get_name", (PyCFunction) py_ped_partition_get_name, METH_VARARGS,
                 partition_get_name_doc},
    {"is_busy", (PyCFunction) py_ped_partition_is_busy, METH_VARARGS,
                partition_is_busy_doc},
    {"get_path", (PyCFunction) py_ped_partition_get_path, METH_VARARGS,
                 partition_get_path_doc},
    {"reset_num", (PyCFunction) py_ped_partition_reset_num, METH_VARARGS,
                  partition_reset_num_doc},
    {NULL}
};

static PyGetSetDef _ped_Partition_getset[] = {
    {"num", (getter) _ped_Partition_get, NULL,
            "The number of this Partition on self.disk.", "num"},
    {"type", (getter) _ped_Partition_get, (setter) _ped_Partition_set,
             "PedPartition type", "type"},
    {NULL}  /* Sentinel */
};

PyTypeObject _ped_Partition_Type_obj = {
    PyVarObject_HEAD_INIT(&PyType_Type,0)
    .tp_name = "_ped.Partition",
    .tp_basicsize = sizeof(_ped_Partition),
 /* .tp_itemsize = XXX */
    .tp_dealloc = (destructor) _ped_Partition_dealloc,
 /* .tp_getattr = XXX */
 /* .tp_setattr = XXX */
 /*   .tp_compare = (cmpfunc) _ped_Partition_compare,*/
 /* .tp_repr = XXX */
 /* .tp_as_number = XXX */
 /* .tp_as_sequence = XXX */
 /* .tp_as_mapping = XXX */
    .tp_hash = PyObject_HashNotImplemented,
    .tp_call = NULL,
    .tp_str = (reprfunc) _ped_Partition_str,
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_setattro = PyObject_GenericSetAttr,
 /* .tp_as_buffer = XXX */
    .tp_flags = TP_FLAGS,
    .tp_doc = _ped_Partition_doc,
    .tp_traverse = (traverseproc) _ped_Partition_traverse,
    .tp_clear = (inquiry) _ped_Partition_clear,
    .tp_richcompare = (richcmpfunc) _ped_Partition_richcompare,
 /* .tp_weaklistoffset = XXX */
 /* .tp_iter = XXX */
 /* .tp_iternext = XXX */
    .tp_methods = _ped_Partition_methods,
    .tp_members = _ped_Partition_members,
    .tp_getset = _ped_Partition_getset,
    .tp_base = NULL,
    .tp_dict = NULL,
 /* .tp_descr_get = XXX */
 /* .tp_descr_set = XXX */
 /* .tp_dictoffset = XXX */
    .tp_init = (initproc) _ped_Partition_init,
    .tp_alloc = PyType_GenericAlloc,
    .tp_new = PyType_GenericNew,
 /* .tp_free = XXX */
 /* .tp_is_gc = XXX */
    .tp_bases = NULL,
 /* .tp_del = XXX */
};

/* _ped.Disk type object */
static PyMemberDef _ped_Disk_members[] = {
    {"dev", T_OBJECT, offsetof(_ped_Disk, dev), READONLY,
            "A _ped.Device object holding self's partition table."},
    {"type", T_OBJECT, offsetof(_ped_Disk, type), READONLY,
             "The type of the disk label as a _ped.DiskType."},
    {NULL}
};

static PyMethodDef _ped_Disk_methods[] = {
    {"duplicate", (PyCFunction) py_ped_disk_duplicate, METH_VARARGS,
                  disk_duplicate_doc},
    {"destroy", (PyCFunction) py_ped_disk_destroy, METH_VARARGS,
                disk_destroy_doc},
    {"commit", (PyCFunction) py_ped_disk_commit, METH_VARARGS,
               disk_commit_doc},
    {"commit_to_dev", (PyCFunction) py_ped_disk_commit_to_dev,
                      METH_VARARGS, disk_commit_to_dev_doc},
    {"commit_to_os", (PyCFunction) py_ped_disk_commit_to_os,
                     METH_VARARGS, disk_commit_to_os_doc},
    {"check", (PyCFunction) py_ped_disk_check, METH_VARARGS,
              disk_check_doc},
    {"print", (PyCFunction) py_ped_disk_print, METH_VARARGS,
              disk_print_doc},
    {"get_primary_partition_count", (PyCFunction)
                                    py_ped_disk_get_primary_partition_count,
                                    METH_VARARGS,
                                    disk_get_primary_partition_count_doc},
    {"get_last_partition_num", (PyCFunction)
                               py_ped_disk_get_last_partition_num,
                               METH_VARARGS,
                               disk_get_last_partition_num_doc},
    {"get_max_primary_partition_count", (PyCFunction)
                                   py_ped_disk_get_max_primary_partition_count,
                                   METH_VARARGS,
                                   disk_get_max_primary_partition_count_doc},
    {"get_max_supported_partition_count", (PyCFunction)
                                 py_ped_disk_get_max_supported_partition_count,
                                 METH_VARARGS,
                                 disk_get_max_supported_partition_count_doc},
    {"get_partition_alignment", (PyCFunction)
                                 py_ped_disk_get_partition_alignment,
                                 METH_NOARGS,
                                 disk_get_partition_alignment_doc},
    {"max_partition_length", (PyCFunction)
                             py_ped_disk_max_partition_length,
                             METH_NOARGS,
                             disk_max_partition_length_doc},
    {"max_partition_start_sector", (PyCFunction)
                             py_ped_disk_max_partition_start_sector,
                             METH_NOARGS,
                             disk_max_partition_start_sector_doc},
    {"set_flag", (PyCFunction) py_ped_disk_set_flag, METH_VARARGS,
                 disk_set_flag_doc},
    {"get_flag", (PyCFunction) py_ped_disk_get_flag, METH_VARARGS,
                 disk_get_flag_doc},
    {"is_flag_available", (PyCFunction) py_ped_disk_is_flag_available,
                          METH_VARARGS, disk_is_flag_available_doc},
    {"add_partition", (PyCFunction) py_ped_disk_add_partition,
                      METH_VARARGS, disk_add_partition_doc},
    {"remove_partition", (PyCFunction) py_ped_disk_remove_partition,
                         METH_VARARGS, disk_remove_partition_doc},
    {"delete_partition", (PyCFunction) py_ped_disk_delete_partition,
                         METH_VARARGS, disk_delete_partition_doc},
    {"delete_all", (PyCFunction) py_ped_disk_delete_all, METH_VARARGS,
                   disk_delete_all_doc},
    {"set_partition_geom", (PyCFunction) py_ped_disk_set_partition_geom,
                           METH_VARARGS, disk_set_partition_geom_doc},
    {"maximize_partition", (PyCFunction) py_ped_disk_maximize_partition,
                           METH_VARARGS, disk_maximize_partition_doc},
    {"get_max_partition_geometry", (PyCFunction)
                                   py_ped_disk_get_max_partition_geometry,
                                   METH_VARARGS,
                                   disk_get_max_partition_geometry_doc},
    {"minimize_extended_partition", (PyCFunction)
                                    py_ped_disk_minimize_extended_partition,
                                    METH_VARARGS,
                                    disk_minimize_extended_partition_doc},
    {"next_partition", (PyCFunction) py_ped_disk_next_partition,
                       METH_VARARGS, disk_next_partition_doc},
    {"get_partition", (PyCFunction) py_ped_disk_get_partition,
                      METH_VARARGS, disk_get_partition_doc},
    {"get_partition_by_sector", (PyCFunction)
                                py_ped_disk_get_partition_by_sector,
                                METH_VARARGS, disk_get_partition_by_sector_doc},
    {"extended_partition", (PyCFunction) py_ped_disk_extended_partition,
                           METH_VARARGS, disk_extended_partition_doc},
    {NULL}
};

static PyGetSetDef _ped_Disk_getset[] = {
    {NULL}  /* Sentinel */
};

PyTypeObject _ped_Disk_Type_obj = {
    PyVarObject_HEAD_INIT(&PyType_Type,0)
    .tp_name = "_ped.Disk",
    .tp_basicsize = sizeof(_ped_Disk),
 /* .tp_itemsize = XXX */
    .tp_dealloc = (destructor) _ped_Disk_dealloc,
 /* .tp_getattr = XXX */
 /* .tp_setattr = XXX */
 /*   .tp_compare = (cmpfunc) _ped_Disk_compare,*/
 /* .tp_repr = XXX */
 /* .tp_as_number = XXX */
 /* .tp_as_sequence = XXX */
 /* .tp_as_mapping = XXX */
    .tp_hash = PyObject_HashNotImplemented,
    .tp_call = NULL,
    .tp_str = (reprfunc) _ped_Disk_str,
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_setattro = PyObject_GenericSetAttr,
 /* .tp_as_buffer = XXX */
    .tp_flags = TP_FLAGS,
    .tp_doc = _ped_Disk_doc,
    .tp_traverse = (traverseproc) _ped_Disk_traverse,
    .tp_clear = (inquiry) _ped_Disk_clear,
    .tp_richcompare = (richcmpfunc) _ped_Disk_richcompare,
 /* .tp_weaklistoffset = XXX */
 /* .tp_iter = XXX */
 /* .tp_iternext = XXX */
    .tp_methods = _ped_Disk_methods,
    .tp_members = _ped_Disk_members,
    .tp_getset = _ped_Disk_getset,
    .tp_base = NULL,
    .tp_dict = NULL,
 /* .tp_descr_get = XXX */
 /* .tp_descr_set = XXX */
 /* .tp_dictoffset = XXX */
    .tp_init = (initproc) _ped_Disk_init,
    .tp_alloc = PyType_GenericAlloc,
    .tp_new = PyType_GenericNew,
 /* .tp_free = XXX */
 /* .tp_is_gc = XXX */
    .tp_bases = NULL,
 /* .tp_del = XXX */
};

/* _ped.DiskType type object */
static PyMemberDef _ped_DiskType_members[] = {
    {NULL}
};

static PyMethodDef _ped_DiskType_methods[] = {
    {"check_feature", (PyCFunction) py_ped_disk_type_check_feature,
                      METH_VARARGS, disk_type_check_feature_doc},
    {NULL}
};

static PyGetSetDef _ped_DiskType_getset[] = {
    {"name", (getter) _ped_DiskType_get, NULL,
             "The name of the partition table type.", "name"},
    {"features", (getter) _ped_DiskType_get, NULL,
                 "A bitmask of features supported by this DiskType.",
                 "features"},
    {NULL}  /* Sentinel */
};

PyTypeObject _ped_DiskType_Type_obj = {
    PyVarObject_HEAD_INIT(&PyType_Type,0)
    .tp_name = "_ped.DiskType",
    .tp_basicsize = sizeof(_ped_DiskType),
 /* .tp_itemsize = XXX */
    .tp_dealloc = (destructor) _ped_DiskType_dealloc,
 /* .tp_getattr = XXX */
 /* .tp_setattr = XXX */
 /*   .tp_compare = (cmpfunc) _ped_DiskType_compare,*/
 /* .tp_repr = XXX */
 /* .tp_as_number = XXX */
 /* .tp_as_sequence = XXX */
 /* .tp_as_mapping = XXX */
    .tp_hash = PyObject_HashNotImplemented,
    .tp_call = NULL,
    .tp_str = (reprfunc) _ped_DiskType_str,
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_setattro = PyObject_GenericSetAttr,
 /* .tp_as_buffer = XXX */
    .tp_flags = TP_FLAGS,
    .tp_doc = _ped_DiskType_doc,
    .tp_traverse = (traverseproc) _ped_DiskType_traverse,
    .tp_clear = (inquiry) _ped_DiskType_clear,
    .tp_richcompare = (richcmpfunc) _ped_DiskType_richcompare,
 /* .tp_weaklistoffset = XXX */
 /* .tp_iter = XXX */
 /* .tp_iternext = XXX */
    .tp_methods = _ped_DiskType_methods,
    .tp_members = _ped_DiskType_members,
    .tp_getset = _ped_DiskType_getset,
    .tp_base = NULL,
    .tp_dict = NULL,
 /* .tp_descr_get = XXX */
 /* .tp_descr_set = XXX */
 /* .tp_dictoffset = XXX */
    .tp_init = NULL,
    .tp_alloc = PyType_GenericAlloc,
    .tp_new = NULL,
 /* .tp_free = XXX */
 /* .tp_is_gc = XXX */
    .tp_bases = NULL,
 /* .tp_del = XXX */
};

#endif /* TYPEOBJECTS_PYDISK_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4
 */
