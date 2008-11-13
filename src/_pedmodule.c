/*
 * _pedmodule.c
 * libparted Python bindings.  This module is low-level in that it directly
 * maps to the libparted API.  It is intended to be used by a higher level
 * Python module that implements the libparted functionality via Python
 * classes and other high level language features.
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
#include <parted/parted.h>

#include "_pedmodule.h"
#include "exceptions.h"
#include "pyconstraint.h"
#include "pydevice.h"
#include "pydisk.h"
#include "pyfilesys.h"
#include "pygeom.h"
#include "pynatmath.h"
#include "pytimer.h"
#include "pyunit.h"

#include "config.h"

/* Docs strings are broken out of the module structure here to be at least a
 * little bit readable.
 */
PyDoc_STRVAR(libparted_version_doc,
             "libparted_version() -> string\n\nReturn the version of libparted "
             "that pyparted was built against.");

PyDoc_STRVAR(pyparted_version_doc,
             "pyparted_version() -> string\n\nReturn the version of the "
             "pyparted module.");

PyDoc_STRVAR(round_up_to_doc,
             "round_up_to_doc(sector, grain_size) -> sector\n\nRounds sector "
             "up to the closest multiple of grain_size.");

PyDoc_STRVAR(round_down_to_doc,
             "round_down_to_doc(sector, grain_size) -> sector\n\nRounds sector "
             "down to the closest multiple of grain_size.");

PyDoc_STRVAR(round_to_nearest_doc,
             "round_down_to(sector, grain_size) -> sector\n\nRounds sector to "
             "the closest multiple of grain_size.");

PyDoc_STRVAR(greatest_common_divisor_doc,
             "greatest_common_divisor_doc(a, b) -> sector\n\nReturns the "
             "largest divisor of both a and b.");

PyDoc_STRVAR(div_round_up_doc,
             "div_round_up(a, b) -> sector\n\nRounds up the result of a "
             "divided by b.");

PyDoc_STRVAR(div_round_to_nearest_doc,
             "div_round_to_nearest(a, b) -> sector\n\nReturns the closest "
             "result of a divided by b.");

/* all of the methods for the _ped module */
static struct PyMethodDef PyPedModuleMethods[] = {
    {"libparted_version", (PyCFunction) py_libparted_get_version, METH_VARARGS,
                          libparted_version_doc},
    {"pyparted_version", (PyCFunction) py_pyparted_version, METH_VARARGS,
                         pyparted_version_doc},

    /* pynatmath.c */
    {"round_up_to", (PyCFunction) py_ped_round_up_to, METH_VARARGS,
                    round_up_to_doc},
    {"round_down_to", (PyCFunction) py_ped_round_down_to, METH_VARARGS,
                      round_down_to_doc},
    {"round_to_nearest", (PyCFunction) py_ped_round_to_nearest, METH_VARARGS,
                         round_to_nearest_doc},
    {"greatest_common_divisor", (PyCFunction) py_ped_greatest_common_divisor,
                                METH_VARARGS, greatest_common_divisor_doc},
    {"div_round_up", (PyCFunction) py_ped_div_round_up, METH_VARARGS,
                     div_round_up_doc},
    {"div_round_to_nearest", (PyCFunction) py_ped_div_round_to_nearest,
                             METH_VARARGS, div_round_to_nearest_doc},

    /* pyunit.c */
    {"unit_set_default", (PyCFunction) py_ped_unit_set_default, METH_VARARGS,
                         NULL},
    {"unit_get_default", (PyCFunction) py_ped_unit_get_default, METH_VARARGS,
                         NULL},
    {"unit_get_size", (PyCFunction) py_ped_unit_get_size, METH_VARARGS, NULL},
    {"unit_get_name", (PyCFunction) py_ped_unit_get_name, METH_VARARGS, NULL},
    {"unit_get_by_name", (PyCFunction) py_ped_unit_get_by_name, METH_VARARGS,
                         NULL},
    {"unit_format_custom_byte", (PyCFunction) py_ped_unit_format_custom_byte,
                                METH_VARARGS, NULL},
    {"unit_format_byte", (PyCFunction) py_ped_unit_format_byte, METH_VARARGS,
                         NULL},
    {"unit_format_custom", (PyCFunction) py_ped_unit_format_custom,
                           METH_VARARGS, NULL},
    {"unit_format", (PyCFunction) py_ped_unit_format, METH_VARARGS, NULL},
    {"unit_parse", (PyCFunction) py_ped_unit_parse, METH_VARARGS, NULL},
    {"unit_parse_custom", (PyCFunction) py_ped_unit_parse_custom,
                          METH_VARARGS, NULL},

    { NULL, NULL, 0, NULL }
};

PyObject *py_libparted_get_version(PyObject *s, PyObject *args) {
    char *ret = (char *) ped_get_version();
    return PyString_FromString(ret);
}

PyObject *py_pyparted_version(PyObject *s, PyObject *args) {
    char *ret = VERSION;
    return PyString_FromString(ret);
}

/* This function catches libparted exceptions and converts them into Python
 * exceptions that the various methods can catch and do something with.  The
 * main motivation for this function is that methods in our parted module need
 * to be able to raise specific, helpful exceptions instead of something
 * generic.
 */
static PedExceptionOption partedExnHandler(PedException *e) {
    switch (e->type) {
        /* Ignore these for now. */
        case PED_EXCEPTION_INFORMATION:
        case PED_EXCEPTION_WARNING:
            partedExnRaised = 0;
            return PED_EXCEPTION_IGNORE;

        /* Set global flags so parted module methods can raise specific
         * exceptions with the message.
         */
        case PED_EXCEPTION_ERROR:
        case PED_EXCEPTION_FATAL:
            partedExnRaised = 1;

            /* Clear out any old error message, then copy the new one in. */
            if (e->message != NULL)
                free(e->message);

            partedExnMessage = strdup(e->message);

            if (partedExnMessage == NULL)
                PyErr_NoMemory();

            return PED_EXCEPTION_CANCEL;

        /* Raise exceptions for internal parted bugs immediately. */
        case PED_EXCEPTION_BUG:
            partedExnRaised = 1;
            PyErr_SetString (PartedException, e->message);
            return PED_EXCEPTION_CANCEL;

        /* Raise NotImplemented exceptions immediately too. */
        case PED_EXCEPTION_NO_FEATURE:
            partedExnRaised = 1;
            PyErr_SetString (PyExc_NotImplementedError, e->message);
            return PED_EXCEPTION_CANCEL;
    }

    return PED_EXCEPTION_IGNORE;
}

PyMODINIT_FUNC init_ped(void) {
    PyObject *m = NULL;
    PyObject *d = NULL;
    PyObject *o = NULL;

    /* init the main Python module and add methods */
    m = Py_InitModule("_ped", PyPedModuleMethods);
    d = PyModule_GetDict(m);

    /* PedUnit possible values */
    o = PyInt_FromLong(PED_UNIT_SECTOR);
    PyDict_SetItemString(d, "UNIT_SECTOR", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_UNIT_BYTE);
    PyDict_SetItemString(d, "UNIT_BYTE", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_UNIT_KILOBYTE);
    PyDict_SetItemString(d, "UNIT_KILOBYTE", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_UNIT_MEGABYTE);
    PyDict_SetItemString(d, "UNIT_MEGABYTE", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_UNIT_GIGABYTE);
    PyDict_SetItemString(d, "UNIT_GIGABYTE", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_UNIT_TERABYTE);
    PyDict_SetItemString(d, "UNIT_TERABYTE", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_UNIT_COMPACT);
    PyDict_SetItemString(d, "UNIT_COMPACT", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_UNIT_CYLINDER);
    PyDict_SetItemString(d, "UNIT_CYLINDER", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_UNIT_CHS);
    PyDict_SetItemString(d, "UNIT_CHS", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_UNIT_PERCENT);
    PyDict_SetItemString(d, "UNIT_PERCENT", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_UNIT_KIBIBYTE);
    PyDict_SetItemString(d, "UNIT_KIBIBYTE", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_UNIT_MEBIBYTE);
    PyDict_SetItemString(d, "UNIT_MEBIBYTE", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_UNIT_GIBIBYTE);
    PyDict_SetItemString(d, "UNIT_GIBIBYTE", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_UNIT_TEBIBYTE);
    PyDict_SetItemString(d, "UNIT_TEBIBYTE", o);
    Py_DECREF(o);

    /* add PedCHSGeometry type as _ped.CHSGeometry */
    _ped_CHSGeometry_Type_obj.tp_new = PyType_GenericNew;
    if (PyType_Ready(&_ped_CHSGeometry_Type_obj) < 0)
        return;

    Py_INCREF(&_ped_CHSGeometry_Type_obj);
    PyModule_AddObject(m, "CHSGeometry",
                       (PyObject *)&_ped_CHSGeometry_Type_obj);

    /* add PedDevice type as _ped.Device */
    _ped_Device_Type_obj.tp_new = PyType_GenericNew;
    if (PyType_Ready(&_ped_Device_Type_obj) < 0)
        return;

    Py_INCREF(&_ped_Device_Type_obj);
    PyModule_AddObject(m, "Device", (PyObject *)&_ped_Device_Type_obj);

    o = PyInt_FromLong(PED_DEVICE_UNKNOWN);
    PyDict_SetItemString(d, "DEVICE_UNKNOWN", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_DEVICE_SCSI);
    PyDict_SetItemString(d, "DEVICE_SCSI", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_DEVICE_IDE);
    PyDict_SetItemString(d, "DEVICE_IDE", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_DEVICE_DAC960);
    PyDict_SetItemString(d, "DEVICE_DAC960", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_DEVICE_CPQARRAY);
    PyDict_SetItemString(d, "DEVICE_CPQARRAY", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_DEVICE_FILE);
    PyDict_SetItemString(d, "DEVICE_FILE", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_DEVICE_ATARAID);
    PyDict_SetItemString(d, "DEVICE_ATARAID", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_DEVICE_I2O);
    PyDict_SetItemString(d, "DEVICE_I2O", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_DEVICE_UBD);
    PyDict_SetItemString(d, "DEVICE_UBD", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_DEVICE_DASD);
    PyDict_SetItemString(d, "DEVICE_DASD", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_DEVICE_VIODASD);
    PyDict_SetItemString(d, "DEVICE_VIODASD", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_DEVICE_SX8);
    PyDict_SetItemString(d, "DEVICE_SX8", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_DEVICE_DM);
    PyDict_SetItemString(d, "DEVICE_DM", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_DEVICE_XVD);
    PyDict_SetItemString(d, "DEVICE_XVD", o);
    Py_DECREF(o);

    /* add PedTimer type as _ped.Timer */
    _ped_Timer_Type_obj.tp_new = PyType_GenericNew;
    if (PyType_Ready(&_ped_Timer_Type_obj) < 0)
        return;

    Py_INCREF(&_ped_Timer_Type_obj);
    PyModule_AddObject(m, "Timer", (PyObject *)&_ped_Timer_Type_obj);

    /* add PedGeometry type as _ped.Geometry */
    _ped_Geometry_Type_obj.tp_new = PyType_GenericNew;
    if (PyType_Ready(&_ped_Geometry_Type_obj) < 0)
        return;

    Py_INCREF(&_ped_Geometry_Type_obj);
    PyModule_AddObject(m, "Geometry", (PyObject *)&_ped_Geometry_Type_obj);

    /* add PedAlignment type as _ped.Alignment */
    _ped_Alignment_Type_obj.tp_new = PyType_GenericNew;
    if (PyType_Ready(&_ped_Alignment_Type_obj) < 0)
        return;

    Py_INCREF(&_ped_Alignment_Type_obj);
    PyModule_AddObject(m, "Alignment", (PyObject *)&_ped_Alignment_Type_obj);

    /* add PedConstraint type as _ped.Constraint */
    _ped_Constraint_Type_obj.tp_new = PyType_GenericNew;
    if (PyType_Ready(&_ped_Constraint_Type_obj) < 0)
        return;

    Py_INCREF(&_ped_Constraint_Type_obj);
    PyModule_AddObject(m, "Constraint", (PyObject *)&_ped_Constraint_Type_obj);

    /* add PedPartition type as _ped.Partition */
    _ped_Partition_Type_obj.tp_new = PyType_GenericNew;
    if (PyType_Ready(&_ped_Partition_Type_obj) < 0)
        return;

    Py_INCREF(&_ped_Partition_Type_obj);
    PyModule_AddObject(m, "Partition", (PyObject *)&_ped_Partition_Type_obj);

    /* add PedDisk as _ped.Disk */
    _ped_Disk_Type_obj.tp_new = PyType_GenericNew;
    if (PyType_Ready(&_ped_Disk_Type_obj) < 0)
        return;

    Py_INCREF(&_ped_Disk_Type_obj);
    PyModule_AddObject(m, "Disk", (PyObject *)&_ped_Disk_Type_obj);

    /* add PedDiskType as _ped.DiskType */
    _ped_DiskType_Type_obj.tp_new = PyType_GenericNew;
    if (PyType_Ready(&_ped_DiskType_Type_obj) < 0)
        return;

    Py_INCREF(&_ped_DiskType_Type_obj);
    PyModule_AddObject(m, "DiskType", (PyObject *)&_ped_DiskType_Type_obj);

    /* possible PedDiskTypeFeature values */
    o = PyInt_FromLong(PED_PARTITION_NORMAL);
    PyDict_SetItemString(d, "PARTITION_NORMAL", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_PARTITION_LOGICAL);
    PyDict_SetItemString(d, "PARTITION_LOGICAL", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_PARTITION_EXTENDED);
    PyDict_SetItemString(d, "PARTITION_EXTENDED", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_PARTITION_FREESPACE);
    PyDict_SetItemString(d, "PARTITION_FREESPACE", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_PARTITION_METADATA);
    PyDict_SetItemString(d, "PARTITION_METADATA", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_PARTITION_PROTECTED);
    PyDict_SetItemString(d, "PARTITION_PROTECTED", o);
    Py_DECREF(o);


    o = PyInt_FromLong(PED_PARTITION_BOOT);
    PyDict_SetItemString(d, "PARTITION_BOOT", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_PARTITION_ROOT);
    PyDict_SetItemString(d, "PARTITION_ROOT", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_PARTITION_SWAP);
    PyDict_SetItemString(d, "PARTITION_SWAP", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_PARTITION_HIDDEN);
    PyDict_SetItemString(d, "PARTITION_HIDDEN", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_PARTITION_RAID);
    PyDict_SetItemString(d, "PARTITION_RAID", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_PARTITION_LVM);
    PyDict_SetItemString(d, "PARTITION_LVM", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_PARTITION_LBA);
    PyDict_SetItemString(d, "PARTITION_LBA", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_PARTITION_HPSERVICE);
    PyDict_SetItemString(d, "PARTITION_HPSERVICE", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_PARTITION_PALO);
    PyDict_SetItemString(d, "PARTITION_PALO", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_PARTITION_PREP);
    PyDict_SetItemString(d, "PARTITION_PREP", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_PARTITION_MSFT_RESERVED);
    PyDict_SetItemString(d, "PARTITION_MSFT_RESERVED", o);
    Py_DECREF(o);


    o = PyInt_FromLong(PED_DISK_TYPE_EXTENDED);
    PyDict_SetItemString(d, "DISK_TYPE_EXTENDED", o);
    Py_DECREF(o);

    o = PyInt_FromLong(PED_DISK_TYPE_PARTITION_NAME);
    PyDict_SetItemString(d, "DISK_TYPE_PARTITION_NAME", o);
    Py_DECREF(o);

    /* add PedFileSystemType as _ped.FileSystemType */
    _ped_FileSystemType_Type_obj.tp_new = PyType_GenericNew;
    if (PyType_Ready(&_ped_FileSystemType_Type_obj) < 0)
        return;

    Py_INCREF(&_ped_FileSystemType_Type_obj);
    PyModule_AddObject(m, "FileSystemType",
                       (PyObject *)&_ped_FileSystemType_Type_obj);

    /* add PedFileSystem as _ped.FileSystem */
    _ped_FileSystem_Type_obj.tp_new = PyType_GenericNew;
    if (PyType_Ready(&_ped_FileSystem_Type_obj) < 0)
        return;

    Py_INCREF(&_ped_FileSystem_Type_obj);
    PyModule_AddObject(m, "FileSystem", (PyObject *)&_ped_FileSystem_Type_obj);

    /* add our custom exceptions */
    AlignmentException = PyErr_NewException("_ped.AlignmentException", NULL,
                                             NULL);
    Py_INCREF(AlignmentException);
    PyModule_AddObject(m, "AlignmentException", AlignmentException);

    ConstraintException = PyErr_NewException("_ped.ConstraintException", NULL,
                                             NULL);
    Py_INCREF(ConstraintException);
    PyModule_AddObject(m, "ConstraintException", ConstraintException);

    CreateException = PyErr_NewException("_ped.CreateException", NULL, NULL);
    Py_INCREF(CreateException);
    PyModule_AddObject(m, "CreateException", CreateException);

    DiskException = PyErr_NewException("_ped.DiskException", NULL, NULL);
    Py_INCREF(DiskException);
    PyModule_AddObject(m, "DiskException", DiskException);

    FileSystemException = PyErr_NewException("_ped.FileSystemException", NULL,
                                             NULL);
    Py_INCREF(FileSystemException);
    PyModule_AddObject(m, "FileSystemException", FileSystemException);

    IOException = PyErr_NewException("_ped.IOException", NULL, NULL);
    Py_INCREF(IOException);
    PyModule_AddObject(m, "IOException", IOException);

    NotNeededException = PyErr_NewException("_ped.NotNeededException",
                                            NULL, NULL);
    Py_INCREF(NotNeededException);
    PyModule_AddObject(m, "NotNeededException", NotNeededException);

    PartedException = PyErr_NewException("_ped.PartedException", NULL, NULL);
    Py_INCREF(PartedException);
    PyModule_AddObject(m, "PartedException", PartedException);

    PartitionException = PyErr_NewException("_ped.PartitionException", NULL,
                                             NULL);
    Py_INCREF(PartitionException);
    PyModule_AddObject(m, "PartitionException", PartitionException);

    TimerException = PyErr_NewException("_ped.TimerException", NULL, NULL);
    Py_INCREF(TimerException);
    PyModule_AddObject(m, "TimerException", TimerException);

    UnknownDeviceException = PyErr_NewException("_ped.UnknownDeviceException",
                                                NULL, NULL);
    Py_INCREF(UnknownDeviceException);
    PyModule_AddObject(m, "UnknownDeviceException", UnknownDeviceException);

    UnknownTypeException = PyErr_NewException("_ped.UnknownTypeException", NULL,
                                              NULL);
    Py_INCREF(UnknownTypeException);
    PyModule_AddObject(m, "UnknownTypeException", UnknownTypeException);

    /* Set up our libparted exception handler. */
    ped_exception_set_handler(partedExnHandler);
}

/* vim:tw=78:ts=4:et:sw=4 */
