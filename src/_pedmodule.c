/*
 * _pedmodule.c
 * libparted Python bindings.  This module is low-level in that it directly
 * maps to the libparted API.  It is intended to be used by a higher level
 * Python module that implements the libparted functionality via Python
 * classes and other high level language features.
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

#include <Python.h>
#include <parted/parted.h>

#include "_pedmodule.h"
#include "pyconstraint.h"
#include "pydevice.h"
#include "pydisk.h"
#include "pygeom.h"
#include "pynatmath.h"
#include "pytimer.h"
#include "pyunit.h"

static struct PyMethodDef PyPedModuleMethods[] = {
    {"get_version", (PyCFunction) py_ped_get_version, METH_VARARGS, NULL },

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

PyObject *py_ped_get_version(PyObject *s, PyObject *args) {
    char *ret = (char *) ped_get_version();

    return Py_BuildValue("s", ret);
}

PyMODINIT_FUNC init_ped(void) {
    PyObject *m, *d, *o;

    /* init the main Python module and add methods */
    m = Py_InitModule("_ped", PyPedModuleMethods);
    d = PyModule_GetDict(m);

    /* add PedUnit type as _ped.Unit */
    _ped_Unit_Type_obj.tp_new = PyType_GenericNew;
    if (PyType_Ready(&_ped_Unit_Type_obj) < 0)
        return;

    m = Py_InitModule3("_ped", _ped_Unit_methods,
                       "Unit object describes boundary and size.");

    Py_INCREF(&_ped_Unit_Type_obj);
    PyModule_AddObject(m, "Unit", (PyObject *)&_ped_Unit_Type_obj);

    ENUM(UNIT_SECTOR);
    ENUM(UNIT_BYTE);
    ENUM(UNIT_KILOBYTE);
    ENUM(UNIT_MEGABYTE);
    ENUM(UNIT_GIGABYTE);
    ENUM(UNIT_TERABYTE);
    ENUM(UNIT_COMPACT);
    ENUM(UNIT_CYLINDER);
    ENUM(UNIT_CHS);
    ENUM(UNIT_PERCENT);
    ENUM(UNIT_KIBIBYTE);
    ENUM(UNIT_MEBIBYTE);
    ENUM(UNIT_GIBIBYTE);
    ENUM(UNIT_TEBIBYTE);

    /* add PedSector type as _ped.Sector */
    _ped_Sector_Type_obj.tp_new = PyType_GenericNew;
    if (PyType_Ready(&_ped_Sector_Type_obj) < 0)
        return;

    m = Py_InitModule3("_ped", _ped_Sector_methods,
                       "Sector object s used to describe disk sizes.");

    Py_INCREF(&_ped_Sector_Type_obj);
    PyModule_AddObject(m, "Sector", (PyObject *)&_ped_Sector_Type_obj);

    /* add PedCHSGeometry type as _ped.CHSGeometry */
    _ped_CHSGeometry_Type_obj.tp_new = PyType_GenericNew;
    if (PyType_Ready(&_ped_CHSGeometry_Type_obj) < 0)
        return;

    m = Py_InitModule3("_ped", _ped_CHSGeometry_methods,
                       "CHSGeometry objects used to describe disk sizes.");

    Py_INCREF(&_ped_CHSGeometry_Type_obj);
    PyModule_AddObject(m, "CHSGeometry",
                       (PyObject *)&_ped_CHSGeometry_Type_obj);

    /* add PedDevice type as _ped.Device */
    _ped_Device_Type_obj.tp_new = PyType_GenericNew;
    if (PyType_Ready(&_ped_Device_Type_obj) < 0)
        return;

    m = Py_InitModule3("_ped", _ped_Device_methods, NULL);

    Py_INCREF(&_ped_Device_Type_obj);
    PyModule_AddObject(m, "Device", (PyObject *)&_ped_Device_Type_obj);

    ENUM(DEVICE_UNKNOWN);
    ENUM(DEVICE_SCSI);
    ENUM(DEVICE_IDE);
    ENUM(DEVICE_DAC960);
    ENUM(DEVICE_CPQARRAY);
    ENUM(DEVICE_FILE);
    ENUM(DEVICE_ATARAID);
    ENUM(DEVICE_I2O);
    ENUM(DEVICE_UBD);
    ENUM(DEVICE_DASD);
    ENUM(DEVICE_VIODASD);
    ENUM(DEVICE_SX8);
    ENUM(DEVICE_DM);
    ENUM(DEVICE_XVD);

    /* add PedTimer type as _ped.Timer */
    _ped_Timer_Type_obj.tp_new = PyType_GenericNew;
    if (PyType_Ready(&_ped_Timer_Type_obj) < 0)
        return;

    m = Py_InitModule3("_ped", _ped_Timer_methods, NULL);

    Py_INCREF(&_ped_Timer_Type_obj);
    PyModule_AddObject(m, "Timer", (PyObject *)&_ped_Timer_Type_obj);

    /* add PedGeometry type as _ped.Geometry */
    _ped_Geometry_Type_obj.tp_new = PyType_GenericNew;
    if (PyType_Ready(&_ped_Geometry_Type_obj) < 0)
        return;

    m = Py_InitModule3("_ped", _ped_Geometry_methods, NULL);

    Py_INCREF(&_ped_Geometry_Type_obj);
    PyModule_AddObject(m, "Geometry", (PyObject *)&_ped_Geometry_Type_obj);

    /* add PedAlignment type as _ped.Alignment */
    _ped_Alignment_Type_obj.tp_new = PyType_GenericNew;
    if (PyType_Ready(&_ped_Alignment_Type_obj) < 0)
        return;

    m = Py_InitModule3("_ped", _ped_Alignment_methods, NULL);

    Py_INCREF(&_ped_Alignment_Type_obj);
    PyModule_AddObject(m, "Alignment", (PyObject *)&_ped_Alignment_Type_obj);

    /* add PedConstraint type as _ped.Constraint */
    _ped_Constraint_Type_obj.tp_new = PyType_GenericNew;
    if (PyType_Ready(&_ped_Constraint_Type_obj) < 0)
        return;

    m = Py_InitModule3("_ped", _ped_Constraint_methods, NULL);

    Py_INCREF(&_ped_Constraint_Type_obj);
    PyModule_AddObject(m, "Constraint", (PyObject *)&_ped_Constraint_Type_obj);

    /* add PedPartition type as _ped.Partition */
    _ped_Partition_Type_obj.tp_new = PyType_GenericNew;
    if (PyType_Ready(&_ped_Partition_Type_obj) < 0)
        return;

    m = Py_InitModule3("_ped", _ped_Partition_methods, NULL);

    Py_INCREF(&_ped_Partition_Type_obj);
    PyModule_AddObject(m, "Partition", (PyObject *)&_ped_Partition_Type_obj);

    /* add PedPartitionType as _ped.PartitionType */
    _ped_PartitionType_Type_obj.tp_new = PyType_GenericNew;
    if (PyType_Ready(&_ped_PartitionType_Type_obj) < 0)
        return;

    m = Py_InitModule3("_ped", _ped_PartitionType_methods, NULL);

    Py_INCREF(&_ped_PartitionType_Type_obj);
    PyModule_AddObject(m, "PartitionType",
                       (PyObject *)&_ped_PartitionType_Type_obj);

    /* add PedPartitionFlag as _ped.PartitionFlag */
    _ped_PartitionFlagType_Type_obj.tp_new = PyType_GenericNew;
    if (PyType_Ready(&_ped_PartitionFlagType_Type_obj) < 0)
        return;

    m = Py_InitModule3("_ped", _ped_PartitionFlag_methods, NULL);

    Py_INCREF(&_ped_PartitionFlagType_Type_obj);
    PyModule_AddObject(m, "PartitionFlag",
                       (PyObject *)&_ped_PartitionFlagType_Type_obj);

    ENUM(PARTITION_NORMAL);
    ENUM(PARTITION_LOGICAL);
    ENUM(PARTITION_EXTENDED);
    ENUM(PARTITION_FREESPACE);
    ENUM(PARTITION_METADATA);
    ENUM(PARTITION_PROTECTED);

    ENUM(PARTITION_BOOT);
    ENUM(PARTITION_ROOT);
    ENUM(PARTITION_SWAP);
    ENUM(PARTITION_HIDDEN);
    ENUM(PARTITION_RAID);
    ENUM(PARTITION_LVM);
    ENUM(PARTITION_LBA);
    ENUM(PARTITION_HPSERVICE);
    ENUM(PARTITION_PALO);
    ENUM(PARTITION_PREP);
    ENUM(PARTITION_MSFT_RESERVED);

    ENUM(DISK_TYPE_EXTENDED);
    ENUM(DISK_TYPE_PARTITION_NAME);
}
