/*
 * pydevice.h
 * pyparted type objects for pydevice.c
 *
 * Copyright The pyparted Project Authors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef TYPEOBJECTS_PYDEVICE_H_INCLUDED
#define TYPEOBJECTS_PYDEVICE_H_INCLUDED
#define PyGC_HEAD_SIZE 0
#include <Python.h>
#include <structmember.h>

/* _ped.CHSGeometry type object */
static PyMemberDef _ped_CHSGeometry_members[] = {
    {NULL}
};

static PyMethodDef _ped_CHSGeometry_methods[] = {
    {NULL}
};

static PyGetSetDef _ped_CHSGeometry_getset[] = {
    {"cylinders", (getter) _ped_CHSGeometry_get,
                  NULL,
                  "The number of cylinders.", "cylinders"},
    {"heads", (getter) _ped_CHSGeometry_get,
              NULL,
              "The number of heads", "heads"},
    {"sectors", (getter) _ped_CHSGeometry_get,
                NULL,
                "The number of sectors", "sectors"},
    {NULL}  /* Sentinel */
};

PyTypeObject _ped_CHSGeometry_Type_obj = {
    PyVarObject_HEAD_INIT(&PyType_Type,0)
    .tp_name = "_ped.CHSGeometry",
    .tp_basicsize = sizeof(_ped_CHSGeometry),
 /* .tp_itemsize = XXX */
    .tp_dealloc = (destructor) _ped_CHSGeometry_dealloc,
 /* .tp_getattr = XXX */
 /* .tp_setattr = XXX */
 /* .tp_repr = XXX */
 /* .tp_as_number = XXX */
 /* .tp_as_sequence = XXX */
 /* .tp_as_mapping = XXX */
    .tp_hash = PyObject_HashNotImplemented,
    .tp_call = NULL,
    .tp_str = (reprfunc) _ped_CHSGeometry_str,
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_setattro = PyObject_GenericSetAttr,
 /* .tp_as_buffer = XXX */
    .tp_flags = TP_FLAGS,
    .tp_doc = _ped_CHSGeometry_doc,
    .tp_traverse = (traverseproc) _ped_CHSGeometry_traverse,
    .tp_clear = (inquiry) _ped_CHSGeometry_clear,
    .tp_richcompare = (richcmpfunc) _ped_CHSGeometry_richcompare,
 /* .tp_weaklistoffset = XXX */
 /* .tp_iter = XXX */
 /* .tp_iternext = XXX */
    .tp_methods = _ped_CHSGeometry_methods,
    .tp_members = _ped_CHSGeometry_members,
    .tp_getset = _ped_CHSGeometry_getset,
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

/* _ped.Device type object */
static PyMemberDef _ped_Device_members[] = {
    {"hw_geom", T_OBJECT, offsetof(_ped_Device, hw_geom), READONLY,
                "The CHSGeometry of the Device as reported by the hardware."},
    {"bios_geom", T_OBJECT, offsetof(_ped_Device, bios_geom), READONLY,
                  "The CHSGeometry of the Device as reported by the BIOS."},
    {NULL}
};

static PyMethodDef _ped_Device_methods[] = {
    /*
     * This is a unique function as it's in pydisk.c, but is really
     * a method on _ped.Device, so it's part of this PyMethod Def
     */
    {"disk_probe", (PyCFunction) py_ped_disk_probe, METH_VARARGS,
                   disk_probe_doc},

    /* These functions are all in pydevice.c */
    {"is_busy", (PyCFunction) py_ped_device_is_busy, METH_VARARGS,
                device_is_busy_doc},
    {"open", (PyCFunction) py_ped_device_open, METH_VARARGS,
             device_open_doc},
    {"close", (PyCFunction) py_ped_device_close, METH_VARARGS,
              device_close_doc},
    {"destroy", (PyCFunction) py_ped_device_destroy, METH_VARARGS,
                device_destroy_doc},
    {"cache_remove", (PyCFunction) py_ped_device_cache_remove,
                     METH_VARARGS, device_cache_remove_doc},
    {"begin_external_access", (PyCFunction) py_ped_device_begin_external_access,
                              METH_VARARGS, device_begin_external_access_doc},
    {"end_external_access", (PyCFunction) py_ped_device_end_external_access,
                            METH_VARARGS, device_end_external_access_doc},
    {"read", (PyCFunction) py_ped_device_read, METH_VARARGS,
             device_read_doc},
    {"write", (PyCFunction) py_ped_device_write, METH_VARARGS,
              device_write_doc},
    {"sync", (PyCFunction) py_ped_device_sync, METH_VARARGS,
             device_sync_doc},
    {"sync_fast", (PyCFunction) py_ped_device_sync_fast, METH_VARARGS,
                  device_sync_fast_doc},
    {"check", (PyCFunction) py_ped_device_check, METH_VARARGS,
              device_check_doc},
    {"get_constraint", (PyCFunction) py_ped_device_get_constraint,
                       METH_VARARGS, device_get_constraint_doc},
    {"get_minimal_aligned_constraint",
                  (PyCFunction) py_ped_device_get_minimal_aligned_constraint,
                  METH_NOARGS, device_get_minimal_aligned_constraint_doc},
    {"get_optimal_aligned_constraint",
                  (PyCFunction) py_ped_device_get_optimal_aligned_constraint,
                  METH_NOARGS, device_get_optimal_aligned_constraint_doc},
    {"get_minimum_alignment",
                  (PyCFunction) py_ped_device_get_minimum_alignment,
                  METH_NOARGS, device_get_minimum_alignment_doc},
    {"get_optimum_alignment",
                  (PyCFunction) py_ped_device_get_optimum_alignment,
                  METH_NOARGS, device_get_optimum_alignment_doc},

    /*
     * These functions are in pydisk.c, but they work best as
     * methods on a _ped.Device.
     */
    {"clobber", (PyCFunction) py_ped_disk_clobber, METH_VARARGS,
                disk_clobber_doc},

    /*
     * These functions are in pyunit.c, but they work best as methods
     * on a _ped.Device
     */
    {"unit_get_size", (PyCFunction) py_ped_unit_get_size, METH_VARARGS,
                      unit_get_size_doc},
    {"unit_format_custom_byte", (PyCFunction) py_ped_unit_format_custom_byte,
                                METH_VARARGS, unit_format_custom_byte_doc},
    {"unit_format_byte", (PyCFunction) py_ped_unit_format_byte, METH_VARARGS,
                         unit_format_byte_doc},
    {"unit_format_custom", (PyCFunction) py_ped_unit_format_custom,
                           METH_VARARGS, unit_format_custom_doc},
    {"unit_format", (PyCFunction) py_ped_unit_format, METH_VARARGS,
                    unit_format_doc},
    {"unit_parse", (PyCFunction) py_ped_unit_parse, METH_VARARGS,
                   unit_parse_doc},
    {"unit_parse_custom", (PyCFunction) py_ped_unit_parse_custom,
                          METH_VARARGS, unit_parse_custom_doc},

    {NULL}
};

static PyGetSetDef _ped_Device_getset[] = {
    {"model", (getter) _ped_Device_get, NULL,
              "A brief description of the hardware, usually mfr and model.",
              "model"},
    {"path", (getter) _ped_Device_get, NULL,
             "The operating system level path to the device node.", "path"},
    {"type", (getter) _ped_Device_get, NULL,
             "The type of device, deprecated in favor of PedDeviceType", "type"},
    {"sector_size", (getter) _ped_Device_get, NULL,
                    "Logical sector size.", "sector_size"},
    {"phys_sector_size", (getter) _ped_Device_get, NULL,
                         "Physical sector size.", "phys_sector_size"},
    {"length", (getter) _ped_Device_get, NULL,
               "Device length, in sectors (LBA).", "length"},
    {"open_count", (getter) _ped_Device_get, NULL,
                   "How many times self.open() has been called.", "open_count"},
    {"read_only", (getter) _ped_Device_get, NULL,
                  "Is the device opened in read-only mode?", "read_only"},
    {"external_mode", (getter) _ped_Device_get, NULL,
                      "PedDevice external_mode", "external_mode"},
    {"dirty", (getter) _ped_Device_get, NULL,
              "Have any unflushed changes been made to self?", "dirty"},
    {"boot_dirty", (getter) _ped_Device_get, NULL,
                   "Have any unflushed changes been made to the bootloader?",
                   "boot_dirty"},
    {"host", (getter) _ped_Device_get, NULL,
             "Any SCSI host ID associated with self.", "host"},
    {"did", (getter) _ped_Device_get, NULL,
            "Any SCSI device ID associated with self.", "did"},
    {NULL}  /* Sentinel */
};

PyTypeObject _ped_Device_Type_obj = {
    PyVarObject_HEAD_INIT(&PyType_Type,0)
    .tp_name = "_ped.Device",
    .tp_basicsize = PyGC_HEAD_SIZE + sizeof(_ped_Device),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor) _ped_Device_dealloc,
 /* .tp_getattr = XXX */
 /* .tp_setattr = XXX */
 /* .tp_repr = XXX */
 /* .tp_as_number = XXX */
 /* .tp_as_sequence = XXX */
 /* .tp_as_mapping = XXX */
    .tp_hash = PyObject_HashNotImplemented,
    .tp_call = NULL,
    .tp_str = (reprfunc) _ped_Device_str,
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_setattro = PyObject_GenericSetAttr,
 /* .tp_as_buffer = XXX */
    .tp_flags = TP_FLAGS,
    .tp_doc = _ped_Device_doc,
    .tp_traverse = (traverseproc) _ped_Device_traverse,
    .tp_clear = (inquiry) _ped_Device_clear,
    .tp_richcompare = (richcmpfunc) _ped_Device_richcompare,
 /* .tp_weaklistoffset = XXX */
 /* .tp_iter = XXX */
 /* .tp_iternext = XXX */
    .tp_methods = _ped_Device_methods,
    .tp_members = _ped_Device_members,
    .tp_getset = _ped_Device_getset,
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

#endif /* TYPEOBJECTS_PYDEVICE_H_INCLUDED */
