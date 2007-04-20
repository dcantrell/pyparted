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

#include <Python.h>

#include "parted/parted.h"

#include "partedmodule.h"
#include "pydevice.h"
#include "pydisk.h"
#include "pyexception.h"
#include "pygeometry.h"
#include "pyfilesystem.h"
#include "pyconstraint.h"

/* global error object */
PyObject *PyPartedError;

/* static global variables */
static PyObject *PyExceptionHandler;
static char *py_exception_string = NULL;

/* common functions */
int
py_ped_exception_string_check (void)
{
        return (py_exception_string != NULL);
}
        

void
py_ped_exception_string_clear (void)
{
        if (py_exception_string != NULL) {
                free (py_exception_string);
                py_exception_string = NULL;
        }
}

void
py_ped_set_error_from_ped_exception (void)
{
        if (py_exception_string != NULL) {
                PyErr_SetString(PyPartedError, py_exception_string);
                py_ped_exception_string_clear ();
                return;
        }
        PyErr_SetString(PyPartedError, "unknown error occured");
}

/* toplevel implementation */

static PedExceptionOption
py_exception_handler (PedException* ex)
{
        PyObject * result, * args = NULL;
        long rc;
        char * type;
        char * buf;
        int len;

        if (py_exception_string != NULL)
                free (py_exception_string);
    
        type = ped_exception_get_type_string (ex->type);
        len = strlen (type) + strlen (ex->message) + 3;
        buf = malloc (len);
        snprintf (buf, len, "%s: %s", type, ex->message);
        py_exception_string = buf;
    
        if (PyExceptionHandler == NULL) {
                return PED_EXCEPTION_UNHANDLED;
        }
    
        args = Py_BuildValue("(N)", (PyObject *) py_ped_exception_obj_new (ex));

        result = PyEval_CallObject(PyExceptionHandler, args);
        Py_XDECREF(args);

        if (result == NULL) {
                PyErr_Print();
                PyErr_Clear();
                return PED_EXCEPTION_UNHANDLED;
        }

        if (!PyInt_Check (result)) {
                fprintf (stderr,
                         "ERROR: python exception handler did not "
                         "return expected int value\n");
                return PED_EXCEPTION_UNHANDLED;
        }
    
        rc = PyInt_AsLong (result);
        Py_DECREF (result);
        return (PedExceptionOption) rc;
}

static PyObject *
py_ped_exception_set_handler (PyObject * self, PyObject * args)
{
        PyObject *cb;

        /* if we already have a callback, releae it */
        if (PyExceptionHandler != NULL) {
                Py_DECREF (PyExceptionHandler);
                PyExceptionHandler = NULL;
        }
    
        if (!PyArg_ParseTuple(args, "O", &cb))
                return NULL;

        if (!PyCallable_Check (cb)) {
                PyErr_SetString(PyExc_TypeError, "parameter must be callable");
                return NULL;
        }
        Py_INCREF (cb);
        PyExceptionHandler = cb;
    
        Py_INCREF(Py_None);
        return Py_None;
}

/* XXX remove me after deprecation period */
PyObject *
PedDevice_get (PyObject * self, PyObject * args);

static PyObject *
py_ped_device_get (PyObject * self, PyObject * args)
{
        fprintf (stderr, "parted.ped_device_get is deprecated.  "
                 "use parted.PedDevice.get('dev') to get a PedDevice "
                 "object\n");
        
        return PedDevice_get(self, args);
}

static PyObject *
py_ped_file_system_type_get (PyObject * self, PyObject * args)
{
        char *type;
        PedFileSystemType *fst;
    
        if (!PyArg_ParseTuple(args, "s", &type)) {
                return NULL;
        }

        if ((fst = ped_file_system_type_get (type)) == NULL) {
                PyErr_SetString(PyPartedError, "unknown file system type");
                return NULL;
        }

        return (PyObject *) py_ped_file_system_type_obj_new (fst);
}

static PyObject *
py_ped_file_system_type_get_next (PyObject * self, PyObject * args)
{
        PyPedFileSystemTypeObj *pyfst = NULL;
        PedFileSystemType *fst;
     
        if (!PyArg_ParseTuple(args, "|O!", &PyPedFileSystemTypeType, &pyfst)) {
                PyErr_SetString(PyExc_TypeError,
                                "optional parameter must be a "
                                "PedFileSystemType");
                return NULL;
        }

        fst = ped_file_system_type_get_next (pyfst ? pyfst->fs_type : NULL);
        if (fst) 
                return (PyObject *) py_ped_file_system_type_obj_new (fst);

        Py_INCREF(Py_None);
        return Py_None;
}

static PyObject *
py_ped_partition_flag_next (PyObject *obj, PyObject * args)
{
        int flag, next;
        
        if (!PyArg_ParseTuple(args, "i", &flag))
                return NULL;
        
        next = ped_partition_flag_next (flag);

        return PyInt_FromLong (next);
}

static PyObject *
py_ped_partition_flag_get_by_name (PyObject *obj, PyObject * args)
{
        char *name;
        int flag;
        
        if (!PyArg_ParseTuple(args, "s", &name))
                return NULL;
        
        flag = ped_partition_flag_get_by_name (name);

        return PyInt_FromLong (flag);
}

static PyObject *
py_ped_partition_flag_get_name (PyObject *obj, PyObject * args)
{
        int flag;
        const char *name;
        
        if (!PyArg_ParseTuple(args, "i", &flag))
                return NULL;
        
        py_ped_exception_string_clear ();
        name = ped_partition_flag_get_name (flag);
        if (py_ped_exception_string_check ()) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }                
        
        return PyString_FromString (name);
}

static PyObject *
py_ped_disk_type_get (PyObject * self, PyObject * args)
{
        char *type;
        PedDiskType *dt;
    
        if (!PyArg_ParseTuple(args, "s", &type)) {
                return NULL;
        }

        if ((dt = ped_disk_type_get (type)) == NULL) {
                PyErr_SetString(PyPartedError, "unknown disk type");
                return NULL;
        }

        return (PyObject *) py_ped_disk_type_obj_new (dt);
}

static PyObject *
py_ped_disk_type_get_next (PyObject * self, PyObject * args)
{
        PyPedDiskTypeObj *pydt = NULL;
        PedDiskType *dt;
     
        if (!PyArg_ParseTuple(args, "|O!", &PyPedDiskTypeType, &pydt)) {
                PyErr_SetString(PyExc_TypeError,
                                "optional parameter must be a "
                                "PedDiskType");
                return NULL;
        }

        dt = ped_disk_type_get_next (pydt ? pydt->type : NULL);
        if (dt) 
                return (PyObject *) py_ped_disk_type_obj_new (dt);

        Py_INCREF(Py_None);
        return Py_None;
}

static struct PyMethodDef PyPedModuleMethods[] = {
    	{ "device_get",
          (PyCFunction) py_ped_device_get, METH_VARARGS, NULL },
        { "exception_set_handler",
          (PyCFunction) py_ped_exception_set_handler,
          METH_VARARGS, NULL },
        { "file_system_type_get",
          (PyCFunction) py_ped_file_system_type_get,
          METH_VARARGS, NULL },
        { "file_system_type_get_next",
          (PyCFunction) py_ped_file_system_type_get_next,
          METH_VARARGS, NULL },
        { "partition_flag_next",
          (PyCFunction) py_ped_partition_flag_next,
          METH_VARARGS, NULL },
        { "partition_flag_get_by_name",
          (PyCFunction) py_ped_partition_flag_get_by_name,
          METH_VARARGS, NULL },
        { "partition_flag_get_name",
          (PyCFunction) py_ped_partition_flag_get_name,
          METH_VARARGS, NULL },
        { "disk_type_get",
          (PyCFunction) py_ped_disk_type_get,
          METH_VARARGS, NULL },
        { "disk_type_get_next",
          (PyCFunction) py_ped_disk_type_get_next,
          METH_VARARGS, NULL },
        { NULL, NULL, 0, NULL }	
};

void initparted(void) {
    PyObject *m, *d, *o;

    m = Py_InitModule("parted", PyPedModuleMethods);
    d = PyModule_GetDict(m);

    if (PyType_Ready(&PyPedGeometryType) < 0)
        return;
    Py_INCREF(&PyPedGeometryType);

    if (PyType_Ready(&PyPedDiskTypeType) < 0)
        return;
    Py_INCREF(&PyPedDiskTypeType);

    if (PyType_Ready(&PyPedDiskType) < 0)
        return;
    Py_INCREF(&PyPedDiskType);

    if (PyType_Ready(&PyPedPartitionType) < 0)
        return;
    Py_INCREF(&PyPedPartitionType);

    if (PyType_Ready(&PyPedDiskConstructorType) < 0)
        return;
    Py_INCREF(&PyPedDiskConstructorType);

    o = PyObject_NEW(PyObject, &PyPedDeviceConstructorType);
    PyDict_SetItemString(d, "PedDevice", o);
    Py_DECREF(o);

    o = PyObject_NEW(PyObject, &PyPedDiskConstructorType);
    PyDict_SetItemString(d, "PedDisk", o);
    Py_DECREF(o);

    /* enum registration */
#define REGISTER_ENUM(val) \
    PyDict_SetItemString(d, #val, o=PyInt_FromLong(PED_ ## val)); \
    Py_DECREF(o);

    REGISTER_ENUM(DEVICE_UNKNOWN);
    REGISTER_ENUM(DEVICE_SCSI);
    REGISTER_ENUM(DEVICE_IDE);
    REGISTER_ENUM(DEVICE_DAC960);
    REGISTER_ENUM(DEVICE_CPQARRAY);
    REGISTER_ENUM(DEVICE_ATARAID);
#ifdef DEVICE_I2O
    REGISTER_ENUM(DEVICE_I2O);
#endif
#ifdef DEVICE_SX8
    REGISTER_ENUM(DEVICE_SX8);
#endif

#define PED_PARTITION_PRIMARY 0
        
    REGISTER_ENUM(PARTITION_PRIMARY);
    REGISTER_ENUM(PARTITION_LOGICAL);
    REGISTER_ENUM(PARTITION_EXTENDED);
    REGISTER_ENUM(PARTITION_FREESPACE);
    REGISTER_ENUM(PARTITION_METADATA);
    REGISTER_ENUM(PARTITION_PROTECTED);

    REGISTER_ENUM(PARTITION_BOOT);
    REGISTER_ENUM(PARTITION_ROOT);
    REGISTER_ENUM(PARTITION_SWAP);
    REGISTER_ENUM(PARTITION_HIDDEN);
    REGISTER_ENUM(PARTITION_RAID);
    REGISTER_ENUM(PARTITION_LVM);
    REGISTER_ENUM(PARTITION_LBA);
    REGISTER_ENUM(PARTITION_HPSERVICE);
    REGISTER_ENUM(PARTITION_PALO);
    REGISTER_ENUM(PARTITION_PREP);
    REGISTER_ENUM(PARTITION_MSFT_RESERVED);

    REGISTER_ENUM(PARTITION_FIRST_FLAG);
    REGISTER_ENUM(PARTITION_LAST_FLAG);

    REGISTER_ENUM(DISK_TYPE_EXTENDED);
    REGISTER_ENUM(DISK_TYPE_PARTITION_NAME);

    REGISTER_ENUM(EXCEPTION_INFORMATION);
    REGISTER_ENUM(EXCEPTION_WARNING);
    REGISTER_ENUM(EXCEPTION_ERROR);
    REGISTER_ENUM(EXCEPTION_FATAL);
    REGISTER_ENUM(EXCEPTION_BUG);
    REGISTER_ENUM(EXCEPTION_NO_FEATURE);
    
    REGISTER_ENUM(EXCEPTION_UNHANDLED);
    REGISTER_ENUM(EXCEPTION_FIX);        
    REGISTER_ENUM(EXCEPTION_YES);
    REGISTER_ENUM(EXCEPTION_NO);
    REGISTER_ENUM(EXCEPTION_OK);
    REGISTER_ENUM(EXCEPTION_RETRY);
    REGISTER_ENUM(EXCEPTION_IGNORE);
    REGISTER_ENUM(EXCEPTION_CANCEL);

    REGISTER_ENUM(EXCEPTION_OK_CANCEL);
    REGISTER_ENUM(EXCEPTION_YES_NO);
    REGISTER_ENUM(EXCEPTION_YES_NO_CANCEL);
    REGISTER_ENUM(EXCEPTION_IGNORE_CANCEL);
    REGISTER_ENUM(EXCEPTION_RETRY_CANCEL);
    REGISTER_ENUM(EXCEPTION_RETRY_IGNORE_CANCEL);
    /* end enum registration */

    /* register the exception handler marhaller function */
    ped_exception_set_handler (py_exception_handler);

    /* set up our exception class */
    PyPartedError = PyErr_NewException("parted.error", NULL, NULL);
    PyDict_SetItemString(d, "error", PyPartedError);
    Py_DECREF(PyPartedError);
}
