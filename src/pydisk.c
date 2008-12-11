/*
 * pydisk.c
 *
 * Copyright (C) 2007  Red Hat, Inc.
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
 * Red Hat Author(s): David Cantrell <dcantrell@redhat.com>
 *                    Chris Lumens <clumens@redhat.com>
 */

#include <Python.h>

#include <stdlib.h>

#include "convert.h"
#include "exceptions.h"
#include "pydisk.h"

/* _ped.Partition type object */
static PyMemberDef _ped_Partition_members[] = {
    {"disk", T_OBJECT, offsetof(_ped_Partition, disk), 0,
             "The _ped.Disk this Partition exists on."},
    {"geom", T_OBJECT, offsetof(_ped_Partition, geom), 0,
             "A _ped.Geometry object describing the region this Partition occupies."},
    {"fs_type", T_OBJECT, offsetof(_ped_Partition, fs_type), 0,
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
    {NULL}
};

static PyGetSetDef _ped_Partition_getset[] = {
    {"num", (getter) _ped_Partition_get, (setter) _ped_Partition_set,
            "The number of this Partition on self.disk.", "num"},
    {"type", (getter) _ped_Partition_get, (setter) _ped_Partition_set,
             "PedPartition type", "type"},
    {NULL}  /* Sentinel */
};

PyTypeObject _ped_Partition_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    .tp_name = "_ped.Partition",
    .tp_basicsize = sizeof(_ped_Partition),
 /* .tp_itemsize = XXX */
    .tp_dealloc = (destructor) _ped_Partition_dealloc,
 /* .tp_print = XXX */
 /* .tp_getattr = XXX */
 /* .tp_setattr = XXX */
 /* .tp_compare = XXX */
 /* .tp_repr = XXX */
 /* .tp_as_number = XXX */
 /* .tp_as_sequence = XXX */
 /* .tp_as_mapping = XXX */
 /* .tp_hash = XXX */
 /* .tp_call = XXX */
 /* .tp_str = XXX */
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_setattro = PyObject_GenericSetAttr,
 /* .tp_as_buffer = XXX */
    .tp_flags = Py_TPFLAGS_HAVE_CLASS | Py_TPFLAGS_CHECKTYPES |
                Py_TPFLAGS_HAVE_GC | Py_TPFLAGS_BASETYPE,
    .tp_doc = _ped_Partition_doc,
    .tp_traverse = (traverseproc) _ped_Partition_traverse,
    .tp_clear = (inquiry) _ped_Partition_clear,
 /* .tp_richcompare = XXX */
 /* .tp_weaklistoffset = XXX */
 /* .tp_iter = XXX */
 /* .tp_iternext = XXX */
    .tp_methods = _ped_Partition_methods,
    .tp_members = _ped_Partition_members,
    .tp_getset = _ped_Partition_getset,
    .tp_base = NULL,
 /* .tp_dict = XXX */
 /* .tp_descr_get = XXX */
 /* .tp_descr_set = XXX */
 /* .tp_dictoffset = XXX */
    .tp_init = (initproc) _ped_Partition_init,
    .tp_alloc = PyType_GenericAlloc,
    .tp_new = PyType_GenericNew,
 /* .tp_free = XXX */
 /* .tp_is_gc = XXX */
    .tp_bases = NULL,
 /* .tp_mro = XXX */
 /* .tp_cache = XXX */
 /* .tp_subclasses = XXX */
 /* .tp_weaklist = XXX */
 /* .tp_del = XXX */
};

/* _ped.Partition functions */
void _ped_Partition_dealloc(_ped_Partition *self) {
    PyObject_GC_UnTrack(self);
    Py_XDECREF(self->disk);
    Py_XDECREF(self->geom);
    Py_XDECREF(self->fs_type);
    PyObject_GC_Del(self);
}

int _ped_Partition_traverse(_ped_Partition *self, visitproc visit, void *arg) {
    int err;

    if (self->disk) {
        if ((err = visit(self->disk, arg))) {
            return err;
        }
    }

    if (self->geom) {
        if ((err = visit(self->geom, arg))) {
            return err;
        }
    }

    if (self->fs_type) {
        if ((err = visit(self->fs_type, arg))) {
            return err;
        }
    }

    return 0;
}

int _ped_Partition_clear(_ped_Partition *self) {
    Py_XDECREF(self->disk);
    self->disk = NULL;

    Py_XDECREF(self->geom);
    self->geom = NULL;

    Py_XDECREF(self->fs_type);
    self->fs_type = NULL;

    return 0;
}

int _ped_Partition_init(_ped_Partition *self, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"disk", "type", "fs_type", "start", "end", NULL};
    PedSector start, end;
    PedDisk *disk = NULL;
    PedFileSystemType *fstype = NULL;
    PedPartition *part = NULL;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!lO!ll", kwlist,
                                     &_ped_Disk_Type_obj, &self->disk,
                                     &self->type, &_ped_FileSystemType_Type_obj,
                                     &self->fs_type, &start, &end)) {
        return -1;
    } else {
        disk = _ped_Disk2PedDisk(self->disk);
        fstype = _ped_FileSystemType2PedFileSystemType(self->fs_type);

        part = ped_partition_new(disk, self->type, fstype, start, end);
        if (part == NULL) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException)) {
                    PyErr_SetString(PartitionException, partedExnMessage);
                }
            } else {
                PyErr_Format(PartitionException, "Could not create new partition on device %s", disk->dev->path);
            }

            ped_disk_destroy(disk);
            return -1;
        }

        Py_XDECREF(self);
        self = PedPartition2_ped_Partition(part);

        ped_disk_destroy(disk);
        return 0;
    }
}

PyObject *_ped_Partition_get(_ped_Partition *self, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Partition()");
        return NULL;
    }

    if (!strcmp(member, "num")) {
        return Py_BuildValue("i", self->num);
    } else if (!strcmp(member, "type")) {
        return PyLong_FromLongLong(self->type);
    } else {
        PyErr_Format(PyExc_AttributeError, "_ped.Partition object has no attribute %s", member);
        return NULL;
    }
}

int _ped_Partition_set(_ped_Partition *self, PyObject *value, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Partition()");
        return -1;
    }

    if (!strcmp(member, "num")) {
        self->num = PyInt_AsLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else if (!strcmp(member, "type")) {
        self->type = PyInt_AsLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else {
        PyErr_Format(PyExc_AttributeError, "_ped.Partition object has no attribute %s", member);
        return -1;
    }

    return 0;
}

/* _ped.Disk type object */
static PyMemberDef _ped_Disk_members[] = {
    {"dev", T_OBJECT, offsetof(_ped_Disk, dev), 0,
            "A _ped.Device object holding self's partition table."},
    {"type", T_OBJECT, offsetof(_ped_Disk, type), 0,
             "The type of the disk label as a _ped.DiskType."},
    {NULL}
};

static PyMethodDef _ped_Disk_methods[] = {
    {"probe", (PyCFunction) py_ped_disk_probe, METH_VARARGS,
              disk_probe_doc},
    {"clobber", (PyCFunction) py_ped_disk_clobber, METH_VARARGS,
                disk_clobber_doc},
    {"clobber_exclude", (PyCFunction) py_ped_disk_clobber_exclude,
                        METH_VARARGS, disk_clobber_exclude_doc},
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
    PyObject_HEAD_INIT(&PyType_Type)
    .tp_name = "_ped.Disk",
    .tp_basicsize = sizeof(_ped_Disk),
 /* .tp_itemsize = XXX */
    .tp_dealloc = (destructor) _ped_Disk_dealloc,
 /* .tp_print = XXX */
 /* .tp_getattr = XXX */
 /* .tp_setattr = XXX */
 /* .tp_compare = XXX */
 /* .tp_repr = XXX */
 /* .tp_as_number = XXX */
 /* .tp_as_sequence = XXX */
 /* .tp_as_mapping = XXX */
 /* .tp_hash = XXX */
 /* .tp_call = XXX */
 /* .tp_str = XXX */
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_setattro = PyObject_GenericSetAttr,
 /* .tp_as_buffer = XXX */
    .tp_flags = Py_TPFLAGS_HAVE_CLASS | Py_TPFLAGS_BASETYPE |
                Py_TPFLAGS_HAVE_GC,
    .tp_doc = _ped_Disk_doc,
    .tp_traverse = (traverseproc) _ped_Disk_traverse,
    .tp_clear = (inquiry) _ped_Disk_clear,
 /* .tp_richcompare = XXX */
 /* .tp_weaklistoffset = XXX */
 /* .tp_iter = XXX */
 /* .tp_iternext = XXX */
    .tp_methods = _ped_Disk_methods,
    .tp_members = _ped_Disk_members,
    .tp_getset = _ped_Disk_getset,
    .tp_base = NULL,
 /* .tp_dict = XXX */
 /* .tp_descr_get = XXX */
 /* .tp_descr_set = XXX */
 /* .tp_dictoffset = XXX */
    .tp_init = (initproc) _ped_Disk_init,
    .tp_alloc = PyType_GenericAlloc,
    .tp_new = PyType_GenericNew,
 /* .tp_free = XXX */
 /* .tp_is_gc = XXX */
    .tp_bases = NULL,
 /* .tp_mro = XXX */
 /* .tp_cache = XXX */
 /* .tp_subclasses = XXX */
 /* .tp_weaklist = XXX */
 /* .tp_del = XXX */
};

/* _ped.Disk functions */
void _ped_Disk_dealloc(_ped_Disk *self) {
    PyObject_GC_UnTrack(self);
    Py_XDECREF(self->dev);
    Py_XDECREF(self->type);
    PyObject_GC_Del(self);
}

int _ped_Disk_traverse(_ped_Disk *self, visitproc visit, void *arg) {
    int err;

    if (self->dev) {
        if ((err = visit(self->dev, arg))) {
            return err;
        }
    }

    if (self->type) {
        if ((err = visit(self->type, arg))) {
            return err;
        }
    }

    return 0;
}

int _ped_Disk_clear(_ped_Disk *self) {
    Py_XDECREF(self->dev);
    self->dev = NULL;

    Py_XDECREF(self->type);
    self->type = NULL;

    return 0;
}

int _ped_Disk_init(_ped_Disk *self, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"dev", "type", NULL};
    PedDevice *device = NULL;
    PedDisk *disk = NULL;
    PedDiskType *type = NULL;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!|O!", kwlist,
                                     &_ped_Device_Type_obj, &self->dev,
                                     &_ped_DiskType_Type_obj, &self->type)) {
        return -1;
    }

    if (self->dev) {
        device = _ped_Device2PedDevice(self->dev);
    }

    if (device) {
        disk = ped_disk_new(device);
    } else if (device && self->type) {
        type = _ped_DiskType2PedDiskType(self->type);
        disk = ped_disk_new_fresh(device, type);
    } else {
        PyErr_SetString(CreateException, "You must provide as least a Device when creating a Disk");
        PyObject_GC_Del(self);
        return -1;
    }

    if (disk == NULL) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException)) {
                PyErr_SetString(IOException, partedExnMessage);
            }
        } else {
            PyErr_Format(IOException, "Failed to read partition table from device %s", device->path);
        }

        if (type) {
            free(type);
        }

        ped_device_destroy(device);
        PyObject_GC_Del(self);
        return -1;
    }

    Py_XDECREF(self);
    self = PedDisk2_ped_Disk(disk);

    if (type) {
        free(type);
    }

    ped_disk_destroy(disk);
    ped_device_destroy(device);

    return 0;
}

/* _ped.DiskType type object */
static PyMemberDef _ped_DiskType_members[] = {
    {NULL}
};

static PyMethodDef _ped_DiskType_methods[] = {
    {"register", (PyCFunction) py_ped_disk_type_register, METH_VARARGS,
                 disk_type_register_doc},
    {"unregister", (PyCFunction) py_ped_disk_type_unregister,
                   METH_VARARGS, disk_type_unregister_doc},
    {"get_next", (PyCFunction) py_ped_disk_type_get_next, METH_VARARGS,
                 disk_type_get_next_doc},
    {"get", (PyCFunction) py_ped_disk_type_get, METH_VARARGS,
            disk_type_get_doc},
    {"check_feature", (PyCFunction) py_ped_disk_type_check_feature,
                      METH_VARARGS, disk_type_check_feature_doc},
    {NULL}
};

static PyGetSetDef _ped_DiskType_getset[] = {
    {"name", (getter) _ped_DiskType_get, (setter) _ped_DiskType_set,
             "The name of the partition table type.", "name"},
    {"features", (getter) _ped_DiskType_get, (setter) _ped_DiskType_set,
                 "A bitmask of features supported by this DiskType.",
                 "features"},
    {NULL}  /* Sentinel */
};

PyTypeObject _ped_DiskType_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    .tp_name = "_ped.DiskType",
    .tp_basicsize = sizeof(_ped_DiskType),
 /* .tp_itemsize = XXX */
    .tp_dealloc = (destructor) _ped_DiskType_dealloc,
 /* .tp_print = XXX */
 /* .tp_getattr = XXX */
 /* .tp_setattr = XXX */
 /* .tp_compare = XXX */
 /* .tp_repr = XXX */
 /* .tp_as_number = XXX */
 /* .tp_as_sequence = XXX */
 /* .tp_as_mapping = XXX */
 /* .tp_hash = XXX */
 /* .tp_call = XXX */
 /* .tp_str = XXX */
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_setattro = PyObject_GenericSetAttr,
 /* .tp_as_buffer = XXX */
    .tp_flags = Py_TPFLAGS_HAVE_CLASS | Py_TPFLAGS_CHECKTYPES |
                Py_TPFLAGS_HAVE_GC | Py_TPFLAGS_BASETYPE,
    .tp_doc = _ped_DiskType_doc,
    .tp_traverse = (traverseproc) _ped_DiskType_traverse,
    .tp_clear = (inquiry) _ped_DiskType_clear,
 /* .tp_richcompare = XXX */
 /* .tp_weaklistoffset = XXX */
 /* .tp_iter = XXX */
 /* .tp_iternext = XXX */
    .tp_methods = _ped_DiskType_methods,
    .tp_members = _ped_DiskType_members,
    .tp_getset = _ped_DiskType_getset,
    .tp_base = NULL,
 /* .tp_dict = XXX */
 /* .tp_descr_get = XXX */
 /* .tp_descr_set = XXX */
 /* .tp_dictoffset = XXX */
    .tp_init = NULL,
    .tp_alloc = PyType_GenericAlloc,
    .tp_new = NULL,
 /* .tp_free = XXX */
 /* .tp_is_gc = XXX */
    .tp_bases = NULL,
 /* .tp_mro = XXX */
 /* .tp_cache = XXX */
 /* .tp_subclasses = XXX */
 /* .tp_weaklist = XXX */
 /* .tp_del = XXX */
};

/* _ped.DiskType functions */
void _ped_DiskType_dealloc(_ped_DiskType *self) {
    PyObject_GC_UnTrack(self);
    PyObject_GC_Del(self);
}

int _ped_DiskType_traverse(_ped_DiskType *self, visitproc visit, void *arg) {
    return 0;
}

int _ped_DiskType_clear(_ped_DiskType *self) {
    return 0;
}

PyObject *_ped_DiskType_get(_ped_DiskType *self, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.DiskType()");
        return NULL;
    }

    if (!strcmp(member, "name")) {
        return PyString_FromString(self->name);
    } else if (!strcmp(member, "features")) {
        return PyLong_FromLongLong(self->features);
    } else {
        PyErr_Format(PyExc_AttributeError, "_ped.DiskType object has no attribute %s", member);
        return NULL;
    }
}

int _ped_DiskType_set(_ped_DiskType *self, PyObject *value, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.DiskType()");
        return -1;
    }

    if (!strcmp(member, "name")) {
        self->name = PyString_AsString(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else if (!strcmp(member, "features")) {
        self->features = PyLong_AsLongLong(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else {
        PyErr_Format(PyExc_AttributeError, "_ped.DiskType object has no attribute %s", member);
        return -1;
    }

    return 0;
}

/* 1:1 function mappings for disk.h in libparted */
PyObject *py_ped_disk_type_register(PyObject *s, PyObject *args) {
    PedDiskType *disktype = NULL;

    disktype = _ped_DiskType2PedDiskType(s);
    if (disktype == NULL) {
        return NULL;
    }

    ped_disk_type_register(disktype);
    /* do not free out_disktype here because it's now in libparted's list */

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_type_unregister(PyObject *s, PyObject *args) {
    PedDiskType *disktype = NULL;

    disktype = _ped_DiskType2PedDiskType(s);
    if (disktype == NULL) {
        return NULL;
    }

    ped_disk_type_unregister(disktype);
    free(disktype);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_type_get_next(PyObject *s, PyObject *args) {
    PedDiskType *type = NULL, *ret_type = NULL;
    _ped_DiskType *ret = NULL;

    if (s == Py_None)
       type = NULL;
    else
       type = _ped_DiskType2PedDiskType(s);

    ret_type = ped_disk_type_get_next(type);
    if (ret_type) {
        ret = PedDiskType2_ped_DiskType(ret_type);
        return (PyObject *) ret;
    }
    else {
        PyErr_SetNone(PyExc_IndexError);
        return NULL;
    }
}

PyObject *py_ped_disk_type_get(PyObject *s, PyObject *args) {
    char *in_name = NULL;
    PedDiskType *out_type = NULL;
    _ped_DiskType *ret = NULL;

    if (!PyArg_ParseTuple(args, "z", &in_name)) {
        return NULL;
    }

    if (in_name) {
        out_type = ped_disk_type_get(in_name);

        if (out_type == NULL) {
            PyErr_SetString(UnknownTypeException, in_name);
            return NULL;
        }

        ret = PedDiskType2_ped_DiskType(out_type);
        if (ret == NULL) {
            return NULL;
        }

    }

    return (PyObject *) ret;
}

PyObject *py_ped_disk_type_check_feature(PyObject *s, PyObject *args) {
    PedDiskType *disktype = NULL;
    PedDiskTypeFeature feature = -1;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "l", &feature)) {
        return NULL;
    }

    disktype = _ped_DiskType2PedDiskType(s);
    if (disktype && feature) {
        ret = ped_disk_type_check_feature(disktype, feature);
        free(disktype);
    }
    else {
        return NULL;
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_probe(PyObject *s, PyObject *args) {
    PedDevice *device = NULL;
    PedDiskType *type = NULL;
    _ped_DiskType *ret = NULL;

    device = _ped_Device2PedDevice(s);
    if (device) {
        type = ped_disk_probe(device);
        if (type == NULL) {
            PyErr_Format(IOException, "Could not probe device %s", device->path);
            return NULL;
        }

        ret = PedDiskType2_ped_DiskType(type);
        if (ret == NULL) {
            return NULL;
        }

        free(type);
    }

    return (PyObject *) ret;
}

PyObject *py_ped_disk_clobber(PyObject *s, PyObject *args) {
    PedDevice *device = NULL;
    int ret = 0;

    device = _ped_Device2PedDevice(s);
    if (device) {
        ret = ped_disk_clobber(device);
        if (ret == 0) {
            PyErr_Format(DiskException, "Failed to clobber partition table on device %s", device->path);
            return NULL;
        }

        ped_device_destroy(device);
    }
    else {
        return NULL;
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_clobber_exclude(PyObject *s, PyObject *args) {
    PyObject *in_disktype = NULL;
    PedDevice *device = NULL;
    PedDiskType *out_disktype = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!", &_ped_DiskType_Type_obj, &in_disktype)) {
        return NULL;
    }

    device = _ped_Device2PedDevice(s);
    if (device == NULL) {
        return NULL;
    }

    out_disktype = _ped_DiskType2PedDiskType(in_disktype);
    if (out_disktype == NULL) {
        return NULL;
    }

    ret = ped_disk_clobber_exclude(device, out_disktype);
    if (ret == 0) {
        PyErr_Format(DiskException, "Failed to clobber partition table on device %s", device->path);
        return NULL;
    }

    ped_device_destroy(device);
    free(out_disktype);

    return PyBool_FromLong(ret);
}

/* XXX: is this necessary? */
PyObject *py_ped_disk_duplicate(PyObject *s, PyObject *args) {
    PedDisk *disk = NULL, *pass_disk = NULL;
    _ped_Disk *ret = NULL;

    disk = _ped_Disk2PedDisk(s);
    if (disk) {
        pass_disk = ped_disk_duplicate(disk);
        if (pass_disk == NULL) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException))
                    PyErr_SetString(IOException, partedExnMessage);
            }
            else
                PyErr_Format(DiskException, "Could not duplicate device %s", disk->dev->path);

            return NULL;
        }

        ret = PedDisk2_ped_Disk(pass_disk);
        if (ret == NULL) {
            return NULL;
        }

        ped_disk_destroy(disk);
        ped_disk_destroy(pass_disk);
    }
    else {
        return NULL;
    }

    return (PyObject *) ret;
}

PyObject *py_ped_disk_destroy(PyObject *s, PyObject *args) {
    PyObject_GC_Del(s);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_commit(PyObject *s, PyObject *args) {
    PedDisk *disk = NULL;
    int ret = 0;

    disk = _ped_Disk2PedDisk(s);
    if (disk) {
        ret = ped_disk_commit(disk);
        if (ret == 0) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException))
                    PyErr_SetString(IOException, partedExnMessage);
            }
            else
                PyErr_Format(DiskException, "Could not commit to disk %s", disk->dev->path);

            return NULL;
        }

        ped_disk_destroy(disk);
    }
    else {
        return NULL;
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_commit_to_dev(PyObject *s, PyObject *args) {
    PedDisk *disk = NULL;
    int ret = 0;

    disk = _ped_Disk2PedDisk(s);
    if (disk) {
        ret = ped_disk_commit_to_dev(disk);
        if (ret == 0) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException))
                    PyErr_SetString(IOException, partedExnMessage);
            }
            else
                PyErr_Format(DiskException, "Could not commit to disk %s", disk->dev->path);

            return NULL;
        }

        ped_disk_destroy(disk);
    }
    else {
        return NULL;
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_commit_to_os(PyObject *s, PyObject *args) {
    PedDisk *disk = NULL;
    int ret = 0;

    disk = _ped_Disk2PedDisk(s);
    if (disk) {
        ret = ped_disk_commit_to_os(disk);
        if (ret == 0) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException))
                    PyErr_SetString(IOException, partedExnMessage);
            }
            else
                PyErr_Format(DiskException, "Could not commit to disk %s", disk->dev->path);

            return NULL;
        }

        ped_disk_destroy(disk);
    }
    else {
        return NULL;
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_check(PyObject *s, PyObject *args) {
    PedDisk *disk = NULL;
    int ret = 0;

    disk = _ped_Disk2PedDisk(s);
    if (disk) {
        ret = ped_disk_check(disk);
        if (ret == 0) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException))
                    PyErr_SetString(IOException, partedExnMessage);
            }
            else
                PyErr_Format(DiskException, "Could not check disk %s", disk->dev->path);

            return NULL;
        }

        ped_disk_destroy(disk);
    }
    else {
        return NULL;
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_print(PyObject *s, PyObject *args) {
    PedDisk *disk = NULL;

    disk = _ped_Disk2PedDisk(s);
    if (disk) {
        ped_disk_print(disk);
        ped_disk_destroy(disk);
    }
    else {
        return NULL;
    }

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_disk_get_primary_partition_count(PyObject *s, PyObject *args) {
    PedDisk *disk = NULL;
    int ret = 0;

    disk = _ped_Disk2PedDisk(s);
    if (disk) {
        ret = ped_disk_get_primary_partition_count(disk);
        ped_disk_destroy(disk);
    }
    else {
        return NULL;
    }

    return PyInt_FromLong(ret);
}

PyObject *py_ped_disk_get_last_partition_num(PyObject *s, PyObject *args) {
    PedDisk *disk = NULL;
    int ret = 0;

    disk = _ped_Disk2PedDisk(s);
    if (disk) {
        ret = ped_disk_get_last_partition_num(disk);
        ped_disk_destroy(disk);
    }
    else {
        return NULL;
    }

    return PyInt_FromLong(ret);
}

PyObject *py_ped_disk_get_max_primary_partition_count(PyObject *s,
                                                      PyObject *args) {
    PedDisk *disk = NULL;
    int ret = 0;

    disk = _ped_Disk2PedDisk(s);
    if (disk) {
        ret = ped_disk_get_max_primary_partition_count(disk);
        ped_disk_destroy(disk);
    }
    else {
        return NULL;
    }

    return PyInt_FromLong(ret);
}

PyObject *py_ped_partition_destroy(PyObject *s, PyObject *args) {
    PedPartition *partition = NULL;

    partition = _ped_Partition2PedPartition(s);
    if (partition) {
        ped_partition_destroy(partition);
    }
    else {
        return NULL;
    }

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_partition_is_active(PyObject *s, PyObject *args) {
    PedPartition *partition = NULL;
    int ret = 0;

    partition = _ped_Partition2PedPartition(s);
    if (partition) {
        ret = ped_partition_is_active(partition);
        ped_partition_destroy(partition);
    }
    else {
        return NULL;
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_partition_set_flag(PyObject *s, PyObject *args) {
    int in_state = -1;
    PedPartition *part = NULL;
    long flag;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "li", &flag, &in_state)) {
        return NULL;
    }

    part = _ped_Partition2PedPartition(s);
    if (part == NULL) {
        return NULL;
    }

    if (part && flag && in_state > -1) {
        ret = ped_partition_set_flag(part, flag, in_state);
        if (ret == 0) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException))
                    PyErr_SetString(PartitionException, partedExnMessage);
            }
            else
                PyErr_Format(PartitionException, "Could not set flag on partition %s%d", part->disk->dev->path, part->num);

            return NULL;
        }

        ped_partition_destroy(part);
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_partition_get_flag(PyObject *s, PyObject *args) {
    PedPartition *part = NULL;
    long flag;
    int ret = -1;

    if (!PyArg_ParseTuple(args, "l", &flag)) {
        return NULL;
    }

    part = _ped_Partition2PedPartition(s);
    if (part == NULL) {
        return NULL;
    }

    ret = ped_partition_get_flag(part, flag);
    ped_partition_destroy(part);

    return PyInt_FromLong(ret);
}

PyObject *py_ped_partition_is_flag_available(PyObject *s, PyObject *args) {
    PedPartition *part = NULL;
    long flag;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "l", &flag)) {
        return NULL;
    }

    part = _ped_Partition2PedPartition(s);
    if (part == NULL) {
        return NULL;
    }

    ret = ped_partition_is_flag_available(part, flag);
    ped_partition_destroy(part);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_partition_set_system(PyObject *s, PyObject *args) {
    PyObject *in_fstype = NULL;
    PedPartition *part = NULL;
    PedFileSystemType *out_fstype = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!", &_ped_FileSystemType_Type_obj, &in_fstype)) {
        return NULL;
    }

    part = _ped_Partition2PedPartition(s);
    if (part == NULL) {
        return NULL;
    }

    out_fstype = _ped_FileSystemType2PedFileSystemType(in_fstype);
    if (out_fstype == NULL) {
        return NULL;
    }

    ret = ped_partition_set_system(part, out_fstype);
    if (ret == 0) {
        PyErr_Format(PartitionException, "Could not set system flag on partition %s%d", part->disk->dev->path, part->num);
        return NULL;
    }

    ped_partition_destroy(part);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_partition_set_name(PyObject *s, PyObject *args) {
    PedPartition *part = NULL;
    char *in_name = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "z", &in_name)) {
        return NULL;
    }

    part = _ped_Partition2PedPartition(s);
    if (part) {
        ret = ped_partition_set_name(part, in_name);
        if (ret == 0) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException))
                    PyErr_SetString(PartitionException, partedExnMessage);
            }
            else
                PyErr_Format(PartitionException, "Could not set name on partition %s%d", part->disk->dev->path, part->num);

            return NULL;
        }

        ped_partition_destroy(part);
    }
    else {
        return NULL;
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_partition_get_name(PyObject *s, PyObject *args) {
    PedPartition *part = NULL;
    char *ret = NULL;

    part = _ped_Partition2PedPartition(s);
    if (part) {
        ret = (char *) ped_partition_get_name(part);
        if (ret == NULL) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException))
                    PyErr_SetString(PartitionException, partedExnMessage);
            }
            else
                PyErr_Format(PartitionException, "Could not read name on partition %s%d", part->disk->dev->path, part->num);

            return NULL;
        }

        ped_partition_destroy(part);
    }
    else {
        return NULL;
    }

    return PyString_FromString(ret);
}

PyObject *py_ped_partition_is_busy(PyObject *s, PyObject *args) {
    PedPartition *part = NULL;
    int ret = 0;

    part = _ped_Partition2PedPartition(s);
    if (part) {
        ret = ped_partition_is_busy(part);
        ped_partition_destroy(part);
    }
    else {
        return NULL;
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_partition_get_path(PyObject *s, PyObject *args) {
    PedPartition *part = NULL;
    char *ret = NULL;

    part = _ped_Partition2PedPartition(s);
    if (part) {
        ret = ped_partition_get_path(part);
        if (ret == NULL) {
            PyErr_Format(PartitionException, "Could not get path for partition %s%d", part->disk->dev->path, part->num);
            return NULL;
        }

        ped_partition_destroy(part);
    }
    else {
        return NULL;
    }

    return PyString_FromString(ret);
}

PyObject *py_ped_partition_type_get_name(PyObject *s, PyObject *args) {
    long type;
    char *ret = NULL;

    if (!PyArg_ParseTuple(args, "l", &type)) {
        return NULL;
    }

    if (type) {
        ret = (char *) ped_partition_type_get_name(type);
    }

    return PyString_FromString(ret);
}

PyObject *py_ped_partition_flag_get_name(PyObject *s, PyObject *args) {
    long flag;
    char *ret = NULL;

    if (!PyArg_ParseTuple(args, "l", &flag)) {
        return NULL;
    }

    if (flag) {
        ret = (char *) ped_partition_flag_get_name(flag);

        if (!ret) {
            /* Re-raise the libparted exception. */
            partedExnRaised = 0;
            return NULL;
        }
    }

    return PyString_FromString(ret);
}

PyObject *py_ped_partition_flag_get_by_name(PyObject *s, PyObject *args) {
    char *name = NULL;

    if (!PyArg_ParseTuple(args, "z", &name)) {
        return NULL;
    }

    return PyLong_FromLongLong(ped_partition_flag_get_by_name(name));
}

PyObject *py_ped_partition_flag_next(PyObject *s, PyObject *args) {
    long flag;

    if (!PyArg_ParseTuple(args, "l", &flag)) {
        return NULL;
    }

    return PyLong_FromLongLong(ped_partition_flag_next(flag));
}

PyObject *py_ped_disk_add_partition(PyObject *s, PyObject *args) {
    PyObject *in_part = NULL, *in_constraint = NULL;
    PedDisk *disk = NULL;
    PedPartition *out_part = NULL;
    PedConstraint *out_constraint = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_Partition_Type_obj,
                          &in_part, &_ped_Constraint_Type_obj, &in_constraint)) {
        return NULL;
    }

    disk = _ped_Disk2PedDisk(s);
    if (disk == NULL) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part == NULL) {
        return NULL;
    }

    out_constraint = _ped_Constraint2PedConstraint(in_constraint);
    if (out_constraint == NULL) {
        return NULL;
    }

    ret = ped_disk_add_partition(disk, out_part, out_constraint);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(PartitionException, partedExnMessage);
        }
        else
            PyErr_Format(PartitionException, "Could not create partition %s%d", out_part->disk->dev->path, out_part->num);

        return NULL;
    }

    ped_disk_destroy(disk);
    ped_partition_destroy(out_part);
    ped_constraint_destroy(out_constraint);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_remove_partition(PyObject *s, PyObject *args) {
    PyObject *in_part = NULL;
    PedDisk *disk = NULL;
    PedPartition *out_part = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Partition_Type_obj, &in_part)) {
        return NULL;
    }

    disk = _ped_Disk2PedDisk(s);
    if (disk == NULL) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part == NULL) {
        return NULL;
    }

    ret = ped_disk_remove_partition(disk, out_part);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(PartitionException, partedExnMessage);
        }
        else
            PyErr_Format(PartitionException, "Could not remove partition %s%d", out_part->disk->dev->path, out_part->num);

        return NULL;
    }

    ped_disk_destroy(disk);
    ped_partition_destroy(out_part);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_delete_partition(PyObject *s, PyObject *args) {
    PyObject *in_part = NULL;
    PedDisk *disk = NULL;
    PedPartition *out_part = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Partition_Type_obj, &in_part)) {
        return NULL;
    }

    disk = _ped_Disk2PedDisk(s);
    if (disk == NULL) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part == NULL) {
        return NULL;
    }

    ret = ped_disk_delete_partition(disk, out_part);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(PartitionException, partedExnMessage);
        }
        else
            PyErr_Format(PartitionException, "Could not remove partition %s%d", out_part->disk->dev->path, out_part->num);

        return NULL;
    }

    ped_disk_destroy(disk);
    ped_partition_destroy(out_part);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_delete_all(PyObject *s, PyObject *args) {
    PedDisk *disk = NULL;
    int ret = 0;

    disk = _ped_Disk2PedDisk(s);
    if (disk) {
        ret = ped_disk_delete_all(disk);
        if (ret == 0) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException))
                    PyErr_SetString(PartitionException, partedExnMessage);
            }
            else
                PyErr_Format(PartitionException, "Could not remove all partitions on %s", disk->dev->path);

            return NULL;
        }

        ped_disk_destroy(disk);
    }
    else {
        return NULL;
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_set_partition_geom(PyObject *s, PyObject *args) {
    PyObject *in_part = NULL, *in_constraint = NULL;
    PedDisk *disk = NULL;
    PedPartition *out_part = NULL;
    PedConstraint *out_constraint = NULL;
    PedSector start, end;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!O!ll", &_ped_Partition_Type_obj, &in_part,
                          &_ped_Constraint_Type_obj, &in_constraint,
                          &start, &end)) {
        return NULL;
    }

    disk = _ped_Disk2PedDisk(s);
    if (disk == NULL) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part == NULL) {
        return NULL;
    }

    out_constraint = _ped_Constraint2PedConstraint(in_constraint);
    if (out_constraint == NULL) {
        return NULL;
    }

    ret = ped_disk_set_partition_geom(disk, out_part, out_constraint,
                                      start, end);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(PartitionException, partedExnMessage);
        }
        else
            PyErr_Format(PartitionException, "Could not set geometry on %s%d", disk->dev->path, out_part->num);

        return NULL;
    }

    ped_disk_destroy(disk);
    ped_partition_destroy(out_part);
    ped_constraint_destroy(out_constraint);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_maximize_partition(PyObject *s, PyObject *args) {
    PyObject *in_part = NULL, *in_constraint = NULL;
    PedDisk *disk = NULL;
    PedPartition *out_part = NULL;
    PedConstraint *out_constraint = NULL;
    int ret = 0;

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_Partition_Type_obj, &in_part,
                          &_ped_Constraint_Type_obj, &in_constraint)) {
        return NULL;
    }

    disk = _ped_Disk2PedDisk(s);
    if (disk == NULL) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part == NULL) {
        return NULL;
    }

    out_constraint = _ped_Constraint2PedConstraint(in_constraint);
    if (out_constraint == NULL) {
        return NULL;
    }

    ret = ped_disk_maximize_partition(disk, out_part, out_constraint);
    if (ret == 0) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(PartitionException, partedExnMessage);
        }
        else
            PyErr_Format(PartitionException, "Could not maximize partition size for %s%d", disk->dev->path, out_part->num);

        return NULL;
    }

    ped_disk_destroy(disk);
    ped_partition_destroy(out_part);
    ped_constraint_destroy(out_constraint);

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_get_max_partition_geometry(PyObject *s, PyObject *args) {
    PyObject *in_part = NULL, *in_constraint = NULL;
    PedDisk *disk = NULL;
    PedPartition *out_part = NULL;
    PedConstraint *out_constraint = NULL;
    PedGeometry *pass_geom = NULL;
    _ped_Geometry *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!O!", &_ped_Partition_Type_obj, &in_part,
                          &_ped_Constraint_Type_obj, &in_constraint)) {
        return NULL;
    }

    disk = _ped_Disk2PedDisk(s);
    if (disk == NULL) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part == NULL) {
        return NULL;
    }

    out_constraint = _ped_Constraint2PedConstraint(in_constraint);
    if (out_constraint == NULL) {
        return NULL;
    }

    pass_geom = ped_disk_get_max_partition_geometry(disk, out_part,
                                                    out_constraint);
    if (pass_geom == NULL) {
        if (partedExnRaised) {
            partedExnRaised = 0;

            if (!PyErr_ExceptionMatches(PartedException))
                PyErr_SetString(PartitionException, partedExnMessage);
        }
        else
            PyErr_Format(PartitionException, "Could not get maximum partition size for %s%d", disk->dev->path, out_part->num);

        return NULL;
    }

    ret = PedGeometry2_ped_Geometry(pass_geom);
    if (ret == NULL) {
        return NULL;
    }

    ped_geometry_destroy(pass_geom);
    ped_disk_destroy(disk);
    ped_partition_destroy(out_part);
    ped_constraint_destroy(out_constraint);

    return (PyObject *) ret;
}

PyObject *py_ped_disk_minimize_extended_partition(PyObject *s, PyObject *args) {
    PedDisk *disk = NULL;
    int ret = 0;

    disk = _ped_Disk2PedDisk(s);
    if (disk) {
        ret = ped_disk_minimize_extended_partition(disk);
        if (ret == 0) {
            if (partedExnRaised) {
                partedExnRaised = 0;

                if (!PyErr_ExceptionMatches(PartedException))
                    PyErr_SetString(PartitionException, partedExnMessage);
            }
            else
                PyErr_Format(PartitionException, "Could not shrink extended partition on %s", disk->dev->path);

            return NULL;
        }

        ped_disk_destroy(disk);
    }
    else {
        return NULL;
    }

    return PyBool_FromLong(ret);
}

PyObject *py_ped_disk_next_partition(PyObject *s, PyObject *args) {
    PyObject *in_part = NULL;
    PedDisk *disk = NULL;
    PedPartition *out_part = NULL;
    PedPartition *pass_part = NULL;
    _ped_Partition *ret = NULL;

    if (!PyArg_ParseTuple(args, "O!", &_ped_Partition_Type_obj, &in_part)) {
        return NULL;
    }

    disk = _ped_Disk2PedDisk(s);
    if (disk == NULL) {
        return NULL;
    }

    out_part = _ped_Partition2PedPartition(in_part);
    if (out_part == NULL) {
        return NULL;
    }

    pass_part = ped_disk_next_partition(disk, out_part);
    if (pass_part == NULL) {
        PyErr_SetNone(PyExc_IndexError);
        return NULL;
    }

    ret = PedPartition2_ped_Partition(pass_part);
    if (ret == NULL) {
        return NULL;
    }

    ped_disk_destroy(disk);
    ped_partition_destroy(out_part);
    ped_partition_destroy(pass_part);

    return (PyObject *) ret;
}

PyObject *py_ped_disk_get_partition(PyObject *s, PyObject *args) {
    int num;
    PedDisk *disk = NULL;
    PedPartition *pass_part = NULL;
    _ped_Partition *ret = NULL;

    if (!PyArg_ParseTuple(args, "i", &num)) {
        return NULL;
    }

    disk = _ped_Disk2PedDisk(s);
    if (disk) {
        pass_part = ped_disk_get_partition(disk, num);
        if (pass_part == NULL) {
            PyErr_SetString(PartitionException, "Partition does not exist");
            return NULL;
        }

        ret = PedPartition2_ped_Partition(pass_part);
        if (ret == NULL) {
            return NULL;
        }

        ped_disk_destroy(disk);
        ped_partition_destroy(pass_part);
    }
    else {
        return NULL;
    }

    return (PyObject *) ret;
}

PyObject *py_ped_disk_get_partition_by_sector(PyObject *s, PyObject *args) {
    PedDisk *disk = NULL;
    PedSector sector;
    PedPartition *pass_part = NULL;
    _ped_Partition *ret = NULL;

    if (!PyArg_ParseTuple(args, "l", &sector)) {
        return NULL;
    }

    disk = _ped_Disk2PedDisk(s);
    if (disk == NULL) {
        return NULL;
    }

    pass_part = ped_disk_get_partition_by_sector(disk, sector);
    if (pass_part == NULL) {
        PyErr_SetString(PartitionException, "Partition does not exist");
        return NULL;
    }

    ret = PedPartition2_ped_Partition(pass_part);
    if (ret == NULL) {
        return NULL;
    }

    ped_disk_destroy(disk);
    ped_partition_destroy(pass_part);

    return (PyObject *) ret;
}

PyObject *py_ped_disk_extended_partition(PyObject *s, PyObject *args) {
    PedDisk *disk = NULL;
    PedPartition *pass_part = NULL;
    _ped_Partition *ret = NULL;

    disk = _ped_Disk2PedDisk(s);
    if (disk) {
        pass_part = ped_disk_extended_partition(disk);
        if (pass_part == NULL) {
            PyErr_SetString(PartitionException, "Extended partition does not exist");
            return NULL;
        }

        ret = PedPartition2_ped_Partition(pass_part);
        if (ret == NULL) {
            return NULL;
        }

        ped_disk_destroy(disk);
        ped_partition_destroy(pass_part);
    }
    else {
        return NULL;
    }

    return (PyObject *) ret;
}

/* vim:tw=78:ts=4:et:sw=4
 */
