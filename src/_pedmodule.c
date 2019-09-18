/*
 * _pedmodule.c
 * libparted Python bindings.  This module is low-level in that it directly
 * maps to the libparted API.  It is intended to be used by a higher level
 * Python module that implements the libparted functionality via Python
 * classes and other high level language features.
 *
 * Copyright (C) 2007-2015 Red Hat, Inc.
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

#include <Python.h>
#include <parted/parted.h>
#include <unistd.h>
#include <sys/types.h>

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

#if PED_DISK_LAST_FLAG < 2
#define PED_DISK_GPT_PMBR_BOOT 2
#endif

#if PED_PARTITION_LAST_FLAG < 15
#define PED_PARTITION_LEGACY_BOOT 15
#endif

#if PED_PARTITION_LAST_FLAG < 16
#define PED_PARTITION_MSFT_DATA 16
#endif

char *partedExnMessage = NULL;
unsigned int partedExnRaised = 0;

PyObject *exn_handler = NULL;

/* Docs strings are broken out of the module structure here to be at least a
 * little bit readable.
 */
PyDoc_STRVAR(libparted_version_doc,
"libparted_version() -> string\n\n"
"Return the version of libparted that pyparted was built against.");

PyDoc_STRVAR(pyparted_version_doc,
"pyparted_version() -> (major, minor, update)\n\n"
"Return the version of the pyparted module.");

PyDoc_STRVAR(constraint_new_from_min_max_doc,
"constraint_new_from_min_max(min, max) -> Constraint\n\n"
"min and max are Geometry objects.  Return a Constraint that requires the region\n"
"to be entirely contained inside max and to entirely contain min.");

PyDoc_STRVAR(constraint_new_from_min_doc,
"constraint_new_from_min(Geometry) -> Constraint\n\n"
"Return a Constraint that requires a region to entirely contain Geometry.");

PyDoc_STRVAR(constraint_new_from_max,
"constraint_new_from_max(Geometry) -> Constraint\n\n"
"Return a Constraint that requires a region to be entirely contained inside\n"
"Geometry.");

PyDoc_STRVAR(constraint_any_doc,
"constraint_any(Device) -> Constraint\n\n"
"Return a Constraint that any region on Device will satisfy.");

PyDoc_STRVAR(constraint_exact_doc,
"constraint_exact(Geometry) -> Constraint\n\n"
"Return a Constraint that only the given Geometry will satisfy.");

PyDoc_STRVAR(device_get_doc,
"device_get(string) -> Device\n\n"
"Return the Device corresponding to the given path.  Typically, path will\n"
"be a device name like /dev/sda.");

PyDoc_STRVAR(device_get_next_doc,
"device_get_next(Device) -> Device\n\n"
"Return the next Device in the list detected by _ped.device_probe_all().");

PyDoc_STRVAR(device_probe_all_doc,
"device_probe_all()\n\n"
"Attempt to detect all devices.");

PyDoc_STRVAR(device_free_all_doc,
"device_free_all()\n\n"
"Close and free all devices.");

PyDoc_STRVAR(file_system_probe_doc,
"file_system_probe(Geometry) -> FileSystemType\n\n"
"Attempt to detect a file system in the region described by Geometry.\n"
"This function tries to be clever at dealing with ambiguous\n"
"situations, such as when one file system was not completely erased\n"
"before a new file system was created on top of it.");

PyDoc_STRVAR(file_system_probe_specific_doc,
"file_system_probe_specific(FileSystemType, Geometry) -> Geometry\n\n"
"Attempt to find a file system and return the region it occupies.");

PyDoc_STRVAR(file_system_type_get_doc,
"file_system_type_get(self, string) -> _ped.FileSystemType\n\n"
"Get a FileSystemType by its name, or raise _ped.UnknownTypeException if no\n"
"type by that name exists.");

PyDoc_STRVAR(file_system_type_get_next_doc,
"file_system_type_get_next(self) -> _ped.FileSystemType\n\n"
"Get the next FileSystemType in parted's list after self, or raise IndexError\n"
"if there are no more types.");

PyDoc_STRVAR(disk_type_get_next_doc,
"disk_type_get_next(self) -> DiskType\n\n"
"Return the next DiskType after self.  If self is the last DiskType, raise\n"
"IndexError.");

PyDoc_STRVAR(disk_type_get_doc,
"disk_type_get(string) -> DiskType\n\n"
"Return a DiskType object with the given name.  If no DiskType exists with\n"
"that name, raise _ped.UnknownTypeException.");

PyDoc_STRVAR(partition_type_get_name_doc,
"partition_type_get_name(integer) -> string\n\n"
"Return a name for a partition type constant.  This mainly exists just to\n"
"present something in user interfaces.  It doesn't really provide the best\n"
"names for partition types.");

PyDoc_STRVAR(partition_flag_get_name_doc,
"partition_flag_get_name(integer) -> string\n\n"
"Return a name for a partition flag constant.  If an invalid flag is provided,\n"
"_ped.PartedExeption will be raised.");

PyDoc_STRVAR(partition_flag_get_by_name_doc,
"partition_flag_get_by_name(string) -> integer\n\n"
"Return a partition flag given its name, or 0 if no flag matches the name.");

PyDoc_STRVAR(partition_flag_next_doc,
"partition_flag_next(integer) -> integer\n\n"
"Given a partition flag, return the next flag.  If there is no next flag, 0\n"
"is returned.");

PyDoc_STRVAR(disk_new_fresh_doc,
"disk_new_fresh(Device, DiskType) -> Disk\n\n"
"Given the Device and DiskType, create a new Disk object with using the\n"
"DiskType specified.  The new disk label is only in-memory, the caller\n"
"will have to use the commit_to_dev() method to write the new label to\n"
"the disk.");

PyDoc_STRVAR(disk_new_doc,
"disk_new(Device) -> Disk\n\n"
"Given the Device, create a new Disk object. And probe, read the details of\n"
"the disk.");

PyDoc_STRVAR(disk_flag_get_name_doc,
"disk_flag_get_name(integer) -> string\n\n"
"Return a name for a disk flag constant.  If an invalid flag is provided,\n"
"a ValueError will be raised.");

PyDoc_STRVAR(disk_flag_get_by_name_doc,
"disk_flag_get_by_name(string) -> integer\n\n"
"Return a disk flag given its name, or 0 if no flag matches the name.");

PyDoc_STRVAR(disk_flag_next_doc,
"disk_flag_next(integer) -> integer\n\n"
"Given a disk flag, return the next flag.  If there is no next flag, 0\n"
"is returned.");

PyDoc_STRVAR(unit_set_default_doc,
"unit_set_default(Unit)\n\n"
"Sets the default Unit to be used by further unit_* calls.  This\n"
"primarily affects the formatting of error messages.");

PyDoc_STRVAR(unit_get_default_doc,
"unit_get_default() -> Unit\n\n"
"Returns the default Unit.");

PyDoc_STRVAR(unit_get_name_doc,
"unit_get_name(Unit) -> string\n\n"
"Returns a textual representation of a given Unit.");

PyDoc_STRVAR(unit_get_by_name_doc,
"unit_get_by_name(string) -> Unit\n\n"
"Returns a Unit given its textual representation.  Returns one of the\n"
"UNIT_* constants.");

PyDoc_STRVAR(register_exn_handler_doc,
"register_exn_handler(function)\n\n"
"When parted raises an exception, the function registered here will be called\n"
"to help determine what action to take.  This does not bypass the exception\n"
"handler pyparted uses.  Instead, it can be used to pop up a dialog to ask the\n"
"user which course of action to take, or to provide a different default answer,\n"
"or other actions.\n\n"
"The given function must accept as arguments:  (1) an integer corresponding to\n"
"one of the EXCEPTION_TYPE_* constants; (2) an integer corresponding to one of the\n"
"EXCEPTION_OPT_* constants; and (3) a string that is the problem encountered by\n"
"parted.  This string will already be translated.  The given function must return\n"
"one of the EXCEPTION_RESOLVE_* constants instructing parted how to proceed.");

PyDoc_STRVAR(clear_exn_handler_doc,
"clear_exn_handler()\n\n"
"Clear any previously added exception handling function.  This means the\n"
"default behavior for all parted exceptions will be used, so only safe\n"
"answers to any questions parted asks will be automatically provided.");

PyDoc_STRVAR(_ped_doc,
"This module implements an interface to libparted.\n\n"
"pyparted provides two API layers:  a lower level that exposes the complete\n"
"libparted API, and a higher level built on top of that which provides a\n"
"more Python-like view.  The _ped module is the base of the lower level\n"
"API.  It provides:\n\n"
"\t- Access to all the basic objects and submodules of pyparted\n"
"\t- Basic unit handling and mathematical functions\n"
"\t- A few basic device probing functions\n"
"\t- The DEVICE_*, PARTITION_*, and UNIT_* constants from libparted\n"
"\t- A variety of exceptions for handling error conditions\n\n"
"For complete documentation, refer to the docs strings for each _ped\n"
"method, exception class, and subclass.");

PyObject *py_ped_register_exn_handler(PyObject *s, PyObject *args) {
    PyObject *fn = NULL;

    if (!PyArg_ParseTuple(args, "O", &fn)) {
        return NULL;
    }

    Py_DECREF(exn_handler);
    exn_handler = fn;

    Py_RETURN_TRUE;
}

PyObject *py_ped_clear_exn_handler(PyObject *s, PyObject *args) {
    exn_handler = Py_None;
    Py_INCREF(exn_handler);
    Py_RETURN_TRUE;
}

/* all of the methods for the _ped module */
static struct PyMethodDef PyPedModuleMethods[] = {
    {"libparted_version", (PyCFunction) py_libparted_get_version, METH_VARARGS,
                          libparted_version_doc},
    {"pyparted_version", (PyCFunction) py_pyparted_version, METH_VARARGS,
                         pyparted_version_doc},

    {"register_exn_handler", (PyCFunction) py_ped_register_exn_handler, METH_VARARGS,
                             register_exn_handler_doc},
    {"clear_exn_handler", (PyCFunction) py_ped_clear_exn_handler, METH_VARARGS,
                          clear_exn_handler_doc},

    /* pyconstraint.c */
    {"constraint_new_from_min_max", (PyCFunction) py_ped_constraint_new_from_min_max,
                                    METH_VARARGS, constraint_new_from_min_max_doc},
    {"constraint_new_from_min", (PyCFunction) py_ped_constraint_new_from_min,
                                METH_VARARGS, constraint_new_from_min_doc},
    {"constraint_new_from_max", (PyCFunction) py_ped_constraint_new_from_max,
                                METH_VARARGS, constraint_new_from_max},
    {"constraint_any", (PyCFunction) py_ped_constraint_any, METH_VARARGS,
                       constraint_any_doc},
    {"constraint_exact", (PyCFunction) py_ped_constraint_exact, METH_VARARGS,
                         constraint_exact_doc},

    /* pydevice.c */
    {"device_get", (PyCFunction) py_ped_device_get, METH_VARARGS,
                   device_get_doc},
    {"device_get_next", (PyCFunction) py_ped_device_get_next, METH_VARARGS,
                        device_get_next_doc},
    {"device_probe_all", (PyCFunction) py_ped_device_probe_all, METH_VARARGS,
                         device_probe_all_doc},
    {"device_free_all", (PyCFunction) py_ped_device_free_all, METH_VARARGS,
                        device_free_all_doc},

    /* pydisk.c */
    {"disk_type_get_next", (PyCFunction) py_ped_disk_type_get_next, METH_VARARGS,
                           disk_type_get_next_doc},
    {"disk_type_get", (PyCFunction) py_ped_disk_type_get, METH_VARARGS,
                      disk_type_get_doc},
    {"partition_type_get_name", (PyCFunction) py_ped_partition_type_get_name,
                                METH_VARARGS, partition_type_get_name_doc},
    {"partition_flag_get_name", (PyCFunction) py_ped_partition_flag_get_name,
                                METH_VARARGS, partition_flag_get_name_doc},
    {"partition_flag_get_by_name", (PyCFunction) py_ped_partition_flag_get_by_name,
                            METH_VARARGS, partition_flag_get_by_name_doc},
    {"partition_flag_next", (PyCFunction) py_ped_partition_flag_next,
                            METH_VARARGS, partition_flag_next_doc},
    {"disk_new_fresh", (PyCFunction) py_ped_disk_new_fresh,
                       METH_VARARGS, disk_new_fresh_doc},
    {"disk_new", (PyCFunction) py_ped_disk_new,
                       METH_VARARGS, disk_new_doc},
    {"disk_flag_get_name", (PyCFunction) py_ped_disk_flag_get_name,
                                METH_VARARGS, disk_flag_get_name_doc},
    {"disk_flag_get_by_name", (PyCFunction) py_ped_disk_flag_get_by_name,
                            METH_VARARGS, disk_flag_get_by_name_doc},
    {"disk_flag_next", (PyCFunction) py_ped_disk_flag_next,
                            METH_VARARGS, disk_flag_next_doc},

    /* pyfilesys.c */
    {"file_system_probe", (PyCFunction) py_ped_file_system_probe, METH_VARARGS,
                          file_system_probe_doc},
    {"file_system_probe_specific", (PyCFunction) py_ped_file_system_probe_specific,
                                   METH_VARARGS, file_system_probe_specific_doc},
    {"file_system_type_get", (PyCFunction) py_ped_file_system_type_get, METH_VARARGS,
                             file_system_type_get_doc},
    {"file_system_type_get_next", (PyCFunction) py_ped_file_system_type_get_next,
                                  METH_VARARGS, file_system_type_get_next_doc},

    /* pyunit.c */
    {"unit_set_default", (PyCFunction) py_ped_unit_set_default, METH_VARARGS,
                         unit_set_default_doc},
    {"unit_get_default", (PyCFunction) py_ped_unit_get_default, METH_VARARGS,
                         unit_get_default_doc},
    {"unit_get_name", (PyCFunction) py_ped_unit_get_name, METH_VARARGS,
                      unit_get_name_doc},
    {"unit_get_by_name", (PyCFunction) py_ped_unit_get_by_name, METH_VARARGS,
                         unit_get_by_name_doc},

    { NULL, NULL, 0, NULL }
};

#if PY_MAJOR_VERSION >= 3
#define MOD_INIT(name) PyMODINIT_FUNC PyInit_##name(void)
#define MOD_ERROR_VAL NULL
#define MOD_SUCCESS_VAL(val) val

struct PyModuleDef module_def = {
     PyModuleDef_HEAD_INIT,
     "_ped",
     _ped_doc,
     -1,
     PyPedModuleMethods,
        NULL, NULL, NULL, NULL
    };
#else
#define MOD_INIT(name) PyMODINIT_FUNC init##name(void)
#define MOD_ERROR_VAL
#define MOD_SUCCESS_VAL(val)
#endif

PyObject *py_libparted_get_version(PyObject *s, PyObject *args) {
    char *ret = (char *) ped_get_version();

    if (ret != NULL)
        return PyUnicode_FromString(ret);
    else
        return PyUnicode_FromString("");
}

PyObject *py_pyparted_version(PyObject *s, PyObject *args) {
    int t = 0;
    int major = -1, minor = -1, update = -1;
    char suffix[11];
    char *v = PYPARTED_VERSION;

    /* Read pyparted version string.  Support the following formats:
     *     X
     *     X.Y
     *     X.Y.Z
     *     X.Y.Z-string
     */
    if (index(v, '-')) {
        memset(&suffix, '\0', sizeof(suffix));
        t = sscanf(v, "%d.%d.%d-%10s", &major, &minor, &update,
                   (char *) &suffix);
    } else {
        t = sscanf(v, "%d.%d.%d", &major, &minor, &update);
    }

    if (t == 0 || t == EOF) {
        return NULL;
    }

    if (t == 1) {
        return Py_BuildValue("(i)", major);
    } else if (t == 2) {
        if (minor == -1) {
            return Py_BuildValue("(is)", major, suffix);
        } else {
            return Py_BuildValue("(ii)", major, minor);
        }
    } else if (t == 3) {
        if (update == -1) {
            return Py_BuildValue("(iis)", major, minor, suffix);
        } else {
            return Py_BuildValue("(iii)", major, minor, update);
        }
    } else {
        return Py_BuildValue("(iiis)", major, minor, update, suffix);
    }
}

/* This function catches libparted exceptions and converts them into Python
 * exceptions that the various methods can catch and do something with.  The
 * main motivation for this function is that methods in our parted module need
 * to be able to raise specific, helpful exceptions instead of something
 * generic.
 *
 * It is also possible for callers to specify a function to help in deciding
 * what to do with parted exceptions.  See the docs for the
 * py_ped_register_exn_handler function.
 */
static PedExceptionOption partedExnHandler(PedException *e) {
    PedExceptionOption ret;

    switch (e->type) {
        /* Raise yes/no/fix exceptions so the caller can deal with them,
         * otherwise ignore */
        case PED_EXCEPTION_INFORMATION:
        case PED_EXCEPTION_WARNING:
            if (e->options == PED_EXCEPTION_YES_NO) {
                ret = PED_EXCEPTION_NO;
            } else if (e->options & PED_EXCEPTION_IGNORE) {
                ret = PED_EXCEPTION_IGNORE;
            } else {
                partedExnRaised = 0;
                return PED_EXCEPTION_UNHANDLED;
            }

            partedExnRaised = 1;
            partedExnMessage = strdup(e->message);

            if (partedExnMessage == NULL) {
                PyErr_NoMemory();
            } else if (exn_handler && PyCallable_Check(exn_handler)) {
                PyObject *args, *retval;

                args = PyTuple_New(3);
                PyTuple_SetItem(args, 0, PyLong_FromLong(e->type));
                PyTuple_SetItem(args, 1, PyLong_FromLong(e->options));
                PyTuple_SetItem(args, 2, PyUnicode_FromString(e->message));

                retval = PyObject_CallObject(exn_handler, args);
                Py_DECREF(args);
                if (retval != NULL && (PyLong_AsLong(retval) == PED_EXCEPTION_UNHANDLED || (PyLong_AsLong(retval) & e->options) > 0)) {
                    return PyLong_AsLong(retval);
                }
            } else {
                /* If no exception handling function was registered to
                 * tell us what to do, return "no" for any yes/no
                 * questions to prevent any potential disk destruction.
                 */
                return ret;
            }

        /* Set global flags so parted module methods can raise specific
         * exceptions with the message.
         */
        case PED_EXCEPTION_ERROR:
        case PED_EXCEPTION_FATAL:
            partedExnRaised = 1;
            partedExnMessage = strdup(e->message);

            if (partedExnMessage == NULL)
                PyErr_NoMemory();
            else if (exn_handler && PyCallable_Check(exn_handler)) {
                PyObject *args, *retval;

                args = PyTuple_New(3);
                PyTuple_SetItem(args, 0, PyLong_FromLong(e->type));
                PyTuple_SetItem(args, 1, PyLong_FromLong(e->options));
                PyTuple_SetItem(args, 2, PyUnicode_FromString(e->message));

                retval = PyObject_CallObject(exn_handler, args);
                Py_DECREF(args);
                if (retval != NULL && (PyLong_AsLong(retval) == PED_EXCEPTION_UNHANDLED || (PyLong_AsLong(retval) & e->options) > 0))
                    return PyLong_AsLong(retval);
                else
                    /* Call failed, use the default value. */
                    return PED_EXCEPTION_CANCEL;
            }
            else {
                /* If no exception handling function was registered to tell us
                 * what to do, return "cancel" for any questions to prevent
                 * any potential disk destruction.
                 */
                return PED_EXCEPTION_CANCEL;
            }

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

MOD_INIT(_ped) {
    PyObject *m = NULL;

    /* init the main Python module and add methods */
#if PY_MAJOR_VERSION >= 3
    m = PyModule_Create(&module_def);
#else
    m = Py_InitModule3("_ped", PyPedModuleMethods, _ped_doc);
#endif

    /* PedUnit possible values */
    PyModule_AddIntConstant(m, "UNIT_SECTOR", PED_UNIT_SECTOR);
    PyModule_AddIntConstant(m, "UNIT_BYTE", PED_UNIT_BYTE);
    PyModule_AddIntConstant(m, "UNIT_KILOBYTE", PED_UNIT_KILOBYTE);
    PyModule_AddIntConstant(m, "UNIT_MEGABYTE", PED_UNIT_MEGABYTE);
    PyModule_AddIntConstant(m, "UNIT_GIGABYTE", PED_UNIT_GIGABYTE);
    PyModule_AddIntConstant(m, "UNIT_TERABYTE", PED_UNIT_TERABYTE);
    PyModule_AddIntConstant(m, "UNIT_COMPACT", PED_UNIT_COMPACT);
    PyModule_AddIntConstant(m, "UNIT_CYLINDER", PED_UNIT_CYLINDER);
    PyModule_AddIntConstant(m, "UNIT_CHS", PED_UNIT_CHS);
    PyModule_AddIntConstant(m, "UNIT_PERCENT", PED_UNIT_PERCENT);
    PyModule_AddIntConstant(m, "UNIT_KIBIBYTE", PED_UNIT_KIBIBYTE);
    PyModule_AddIntConstant(m, "UNIT_MEBIBYTE", PED_UNIT_MEBIBYTE);
    PyModule_AddIntConstant(m, "UNIT_GIBIBYTE", PED_UNIT_GIBIBYTE);
    PyModule_AddIntConstant(m, "UNIT_TEBIBYTE", PED_UNIT_TEBIBYTE);

    /* add PedCHSGeometry type as _ped.CHSGeometry */
    if (PyType_Ready(&_ped_CHSGeometry_Type_obj) < 0)
        return MOD_ERROR_VAL;

    Py_INCREF(&_ped_CHSGeometry_Type_obj);
    PyModule_AddObject(m, "CHSGeometry",
                       (PyObject *)&_ped_CHSGeometry_Type_obj);

    /* add PedDevice type as _ped.Device */
    if (PyType_Ready(&_ped_Device_Type_obj) < 0)
        return MOD_ERROR_VAL;

    Py_INCREF(&_ped_Device_Type_obj);
    PyModule_AddObject(m, "Device", (PyObject *)&_ped_Device_Type_obj);

    PyModule_AddIntConstant(m, "DEVICE_UNKNOWN", PED_DEVICE_UNKNOWN);
    PyModule_AddIntConstant(m, "DEVICE_SCSI", PED_DEVICE_SCSI);
    PyModule_AddIntConstant(m, "DEVICE_IDE", PED_DEVICE_IDE);
    PyModule_AddIntConstant(m, "DEVICE_DAC960", PED_DEVICE_DAC960);
    PyModule_AddIntConstant(m, "DEVICE_CPQARRAY", PED_DEVICE_CPQARRAY);
    PyModule_AddIntConstant(m, "DEVICE_FILE", PED_DEVICE_FILE);
    PyModule_AddIntConstant(m, "DEVICE_ATARAID", PED_DEVICE_ATARAID);
    PyModule_AddIntConstant(m, "DEVICE_I2O", PED_DEVICE_I2O);
    PyModule_AddIntConstant(m, "DEVICE_UBD", PED_DEVICE_UBD);
    PyModule_AddIntConstant(m, "DEVICE_DASD", PED_DEVICE_DASD);
    PyModule_AddIntConstant(m, "DEVICE_VIODASD", PED_DEVICE_VIODASD);
    PyModule_AddIntConstant(m, "DEVICE_SX8", PED_DEVICE_SX8);
    PyModule_AddIntConstant(m, "DEVICE_DM", PED_DEVICE_DM);
    PyModule_AddIntConstant(m, "DEVICE_XVD", PED_DEVICE_XVD);
    PyModule_AddIntConstant(m, "DEVICE_SDMMC", PED_DEVICE_SDMMC);
    PyModule_AddIntConstant(m, "DEVICE_VIRTBLK", PED_DEVICE_VIRTBLK);

    /* add PedTimer type as _ped.Timer */
    if (PyType_Ready(&_ped_Timer_Type_obj) < 0)
        return MOD_ERROR_VAL;

    Py_INCREF(&_ped_Timer_Type_obj);
    PyModule_AddObject(m, "Timer", (PyObject *)&_ped_Timer_Type_obj);

    /* add PedGeometry type as _ped.Geometry */
    if (PyType_Ready(&_ped_Geometry_Type_obj) < 0)
        return MOD_ERROR_VAL;

    Py_INCREF(&_ped_Geometry_Type_obj);
    PyModule_AddObject(m, "Geometry", (PyObject *)&_ped_Geometry_Type_obj);

    /* add PedAlignment type as _ped.Alignment */
    if (PyType_Ready(&_ped_Alignment_Type_obj) < 0)
        return MOD_ERROR_VAL;

    Py_INCREF(&_ped_Alignment_Type_obj);
    PyModule_AddObject(m, "Alignment", (PyObject *)&_ped_Alignment_Type_obj);

    /* add PedConstraint type as _ped.Constraint */
    if (PyType_Ready(&_ped_Constraint_Type_obj) < 0)
        return MOD_ERROR_VAL;

    Py_INCREF(&_ped_Constraint_Type_obj);
    PyModule_AddObject(m, "Constraint", (PyObject *)&_ped_Constraint_Type_obj);

    /* add PedPartition type as _ped.Partition */
    if (PyType_Ready(&_ped_Partition_Type_obj) < 0)
        return MOD_ERROR_VAL;

    Py_INCREF(&_ped_Partition_Type_obj);
    PyModule_AddObject(m, "Partition", (PyObject *)&_ped_Partition_Type_obj);

    /* add PedDisk as _ped.Disk */
    if (PyType_Ready(&_ped_Disk_Type_obj) < 0)
        return MOD_ERROR_VAL;

    Py_INCREF(&_ped_Disk_Type_obj);
    PyModule_AddObject(m, "Disk", (PyObject *)&_ped_Disk_Type_obj);

    /* add PedDiskType as _ped.DiskType */
    if (PyType_Ready(&_ped_DiskType_Type_obj) < 0)
        return MOD_ERROR_VAL;

    Py_INCREF(&_ped_DiskType_Type_obj);
    PyModule_AddObject(m, "DiskType", (PyObject *)&_ped_DiskType_Type_obj);

    /* possible PedDiskTypeFeature values */
    PyModule_AddIntConstant(m, "PARTITION_NORMAL", PED_PARTITION_NORMAL);
    PyModule_AddIntConstant(m, "PARTITION_LOGICAL", PED_PARTITION_LOGICAL);
    PyModule_AddIntConstant(m, "PARTITION_EXTENDED", PED_PARTITION_EXTENDED);
    PyModule_AddIntConstant(m, "PARTITION_FREESPACE", PED_PARTITION_FREESPACE);
    PyModule_AddIntConstant(m, "PARTITION_METADATA", PED_PARTITION_METADATA);
    PyModule_AddIntConstant(m, "PARTITION_PROTECTED", PED_PARTITION_PROTECTED);

    PyModule_AddIntConstant(m, "PARTITION_BOOT", PED_PARTITION_BOOT);
    PyModule_AddIntConstant(m, "PARTITION_ROOT", PED_PARTITION_ROOT);
    PyModule_AddIntConstant(m, "PARTITION_SWAP", PED_PARTITION_SWAP);
    PyModule_AddIntConstant(m, "PARTITION_HIDDEN", PED_PARTITION_HIDDEN);
    PyModule_AddIntConstant(m, "PARTITION_RAID", PED_PARTITION_RAID);
    PyModule_AddIntConstant(m, "PARTITION_LVM", PED_PARTITION_LVM);
    PyModule_AddIntConstant(m, "PARTITION_LBA", PED_PARTITION_LBA);
    PyModule_AddIntConstant(m, "PARTITION_HPSERVICE", PED_PARTITION_HPSERVICE);
    PyModule_AddIntConstant(m, "PARTITION_PALO", PED_PARTITION_PALO);
    PyModule_AddIntConstant(m, "PARTITION_PREP", PED_PARTITION_PREP);
    PyModule_AddIntConstant(m, "PARTITION_MSFT_RESERVED", PED_PARTITION_MSFT_RESERVED);
    PyModule_AddIntConstant(m, "PARTITION_APPLE_TV_RECOVERY", PED_PARTITION_APPLE_TV_RECOVERY);
    PyModule_AddIntConstant(m, "PARTITION_BIOS_GRUB", PED_PARTITION_BIOS_GRUB);
    PyModule_AddIntConstant(m, "PARTITION_DIAG", PED_PARTITION_DIAG);
    PyModule_AddIntConstant(m, "PARTITION_LEGACY_BOOT", PED_PARTITION_LEGACY_BOOT);
#ifdef PED_PARTITION_MSFT_DATA
    PyModule_AddIntConstant(m, "PARTITION_MSFT_DATA", PED_PARTITION_MSFT_DATA);
#endif
#ifdef PED_PARTITION_IRST
    PyModule_AddIntConstant(m, "PARTITION_IRST", PED_PARTITION_IRST);
#endif
#ifdef PED_PARTITION_ESP
    PyModule_AddIntConstant(m, "PARTITION_ESP", PED_PARTITION_ESP);
#endif

    PyModule_AddIntConstant(m, "DISK_CYLINDER_ALIGNMENT", PED_DISK_CYLINDER_ALIGNMENT);
    PyModule_AddIntConstant(m, "DISK_GPT_PMBR_BOOT", PED_DISK_GPT_PMBR_BOOT);

    PyModule_AddIntConstant(m, "DISK_TYPE_EXTENDED", PED_DISK_TYPE_EXTENDED);
    PyModule_AddIntConstant(m, "DISK_TYPE_PARTITION_NAME", PED_DISK_TYPE_PARTITION_NAME);

    /* add PedFileSystemType as _ped.FileSystemType */
    if (PyType_Ready(&_ped_FileSystemType_Type_obj) < 0)
        return MOD_ERROR_VAL;

    Py_INCREF(&_ped_FileSystemType_Type_obj);
    PyModule_AddObject(m, "FileSystemType",
                       (PyObject *)&_ped_FileSystemType_Type_obj);

    /* add PedFileSystem as _ped.FileSystem */
    if (PyType_Ready(&_ped_FileSystem_Type_obj) < 0)
        return MOD_ERROR_VAL;

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

    DeviceException = PyErr_NewException("_ped.DeviceException", NULL, NULL);
    Py_INCREF(DeviceException);
    PyModule_AddObject(m, "DeviceException", DeviceException);

    DiskException = PyErr_NewException("_ped.DiskException", NULL, NULL);
    Py_INCREF(DiskException);
    PyModule_AddObject(m, "DiskException", DiskException);

    DiskLabelException = PyErr_NewException("_ped.DiskLabelException", NULL, NULL);
    Py_INCREF(DiskLabelException);
    PyModule_AddObject(m, "DiskLabelException", DiskLabelException);

    FileSystemException = PyErr_NewException("_ped.FileSystemException", NULL,
                                             NULL);
    Py_INCREF(FileSystemException);
    PyModule_AddObject(m, "FileSystemException", FileSystemException);

    GeometryException = PyErr_NewException("_ped.GeometryException", NULL, NULL);
    Py_INCREF(GeometryException);
    PyModule_AddObject(m, "GeometryException", GeometryException);

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

    /* Exception type constants. */
    PyModule_AddIntConstant(m, "EXCEPTION_TYPE_INFORMATION", PED_EXCEPTION_INFORMATION);
    PyModule_AddIntConstant(m, "EXCEPTION_TYPE_WARNING", PED_EXCEPTION_WARNING);
    PyModule_AddIntConstant(m, "EXCEPTION_TYPE_ERROR", PED_EXCEPTION_ERROR);
    PyModule_AddIntConstant(m, "EXCEPTION_TYPE_FATAL", PED_EXCEPTION_FATAL);
    PyModule_AddIntConstant(m, "EXCEPTION_TYPE_BUG", PED_EXCEPTION_BUG);
    PyModule_AddIntConstant(m, "EXCEPTION_TYPE_NO_FEATURE", PED_EXCEPTION_NO_FEATURE);

    /* Exception resolution constants. */
    PyModule_AddIntConstant(m, "EXCEPTION_RESOLVE_UNHANDLED", PED_EXCEPTION_UNHANDLED);
    PyModule_AddIntConstant(m, "EXCEPTION_RESOLVE_FIX", PED_EXCEPTION_FIX);
    PyModule_AddIntConstant(m, "EXCEPTION_RESOLVE_YES", PED_EXCEPTION_YES);
    PyModule_AddIntConstant(m, "EXCEPTION_RESOLVE_NO", PED_EXCEPTION_NO);
    PyModule_AddIntConstant(m, "EXCEPTION_RESOLVE_OK", PED_EXCEPTION_OK);
    PyModule_AddIntConstant(m, "EXCEPTION_RESOLVE_RETRY", PED_EXCEPTION_RETRY);
    PyModule_AddIntConstant(m, "EXCEPTION_RESOLVE_IGNORE", PED_EXCEPTION_IGNORE);
    PyModule_AddIntConstant(m, "EXCEPTION_RESOLVE_CANCEL", PED_EXCEPTION_CANCEL);

    /* Exception option constants. */
    PyModule_AddIntConstant(m, "EXCEPTION_OPT_OK_CANCEL", PED_EXCEPTION_OK_CANCEL);
    PyModule_AddIntConstant(m, "EXCEPTION_OPT_YES_NO", PED_EXCEPTION_YES_NO);
    PyModule_AddIntConstant(m, "EXCEPTION_OPT_YES_NO_CANCEL", PED_EXCEPTION_YES_NO_CANCEL);
    PyModule_AddIntConstant(m, "EXCEPTION_OPT_IGNORE_CANCEL", PED_EXCEPTION_IGNORE_CANCEL);
    PyModule_AddIntConstant(m, "EXCEPTION_OPT_RETRY_CANCEL", PED_EXCEPTION_RETRY_CANCEL);
    PyModule_AddIntConstant(m, "EXCEPTION_OPT_RETRY_IGNORE_CANCEL", PED_EXCEPTION_RETRY_IGNORE_CANCEL);

    exn_handler = Py_None;
    Py_INCREF(exn_handler);

    /* Set up our libparted exception handler. */
    ped_exception_set_handler(partedExnHandler);
    return MOD_SUCCESS_VAL(m);
}

/* vim:tw=78:ts=4:et:sw=4
 */
