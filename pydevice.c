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
#include "partedmodule.h"
#include "pydevice.h"
#include "pydisk.h"
#include "pygeometry.h"
#include "pyconstraint.h"

/* device implementation */

PyPedDevice *
py_ped_device_new (PedDevice *dev)
{
        PyPedDevice *d;
    
        d = (PyPedDevice *) PyObject_NEW(PyObject, &PyPedDeviceType);
        d->dev = dev;
        return d;
}

static void
py_ped_device_dealloc (PyPedDevice * s)
{
        /* s->dev will be destroyed if ped_device_free_all() is called */
        /*      ped_device_destroy (s->dev); */
        PyObject_DEL(s);
}

static void
py_ped_device_destroy (PyPedDevice * s)
{
        ped_device_destroy (s->dev);
        PyObject_DEL(s);
}

static PyObject *
py_ped_device_open (PyPedDevice * s, PyObject * args)
{
        py_ped_exception_string_clear ();
        if (!ped_device_open (s->dev)) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }

        Py_INCREF(Py_None);
        return Py_None;
}

static PyObject *
py_ped_device_close (PyPedDevice * s, PyObject * args)
{
        py_ped_exception_string_clear ();
        if (!ped_device_close (s->dev)) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }
    
        Py_INCREF(Py_None);
        return Py_None;
}

static PyObject *
py_ped_device_sync (PyPedDevice * s, PyObject * args)
{
        py_ped_exception_string_clear ();        
        if (!ped_device_sync (s->dev)) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }

        Py_INCREF(Py_None);
        return Py_None;
}

static PyObject *
py_ped_disk_probe (PyPedDevice * s, PyObject * args)
{
        PedDiskType *type;
        
        py_ped_exception_string_clear ();
        if ((type = ped_disk_probe (s->dev)) == NULL) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }

        return (PyObject *) py_ped_disk_type_obj_new (type);
}

static PyObject *
py_ped_device_is_busy (PyPedDevice *d, PyObject * args)
{
        int state;
        
        py_ped_exception_string_clear ();
        state = ped_device_is_busy (d->dev);
        if (py_ped_exception_string_check ()) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }

        return PyInt_FromLong (state);
}

static PyObject *
py_ped_geometry_new (PyPedDevice * self, PyObject * args)
{
        PedSector start, length;
        PedGeometry *geom;
        PyPedGeometry *pygeom;

        if (!PyArg_ParseTuple(args, "LL", &start, &length)) {
                return NULL;
        }

        py_ped_exception_string_clear ();
        geom = ped_geometry_new (self->dev, start, length);
        if (geom == NULL) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }

        pygeom = py_ped_geometry_obj_new (geom, self, 0);
        pygeom->dev = self;
        Py_INCREF (self);

        return (PyObject *) pygeom;
}

static PyObject *
py_ped_constraint_any (PyPedDevice * self, PyObject * args)
{
        PedConstraint *constraint;
        PyPedConstraint *pyconstraint;
        
        py_ped_exception_string_clear ();
        constraint = ped_constraint_any (self->dev);
        if (constraint == NULL) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }
        
        pyconstraint = py_ped_constraint_obj_new (constraint, self, 0);

        return (PyObject *) pyconstraint;
}

static PyObject *
py_ped_disk_new_fresh (PyPedDevice * self, PyObject * args)
{
        PyPedDiskTypeObj *type;
        PedDisk *disk;

        if (!PyArg_ParseTuple(args, "O!", &PyPedDiskTypeType, &type)) {
                PyErr_SetString(PyExc_TypeError,
                                "parameter must be a PedDiskType");
                return NULL;
        }

        py_ped_exception_string_clear ();
        if ((disk = ped_disk_new_fresh (self->dev, type->type)) == NULL) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }

        return (PyObject *) py_ped_disk_obj_new (disk, 0);
}

static struct PyMethodDef PyPedDeviceMethods[] = {
        { "open", (PyCFunction) py_ped_device_open,
          METH_VARARGS, NULL },
        { "close", (PyCFunction) py_ped_device_close,
          METH_VARARGS, NULL },
        { "sync", (PyCFunction) py_ped_device_sync,
          METH_VARARGS, NULL },
        { "disk_probe", (PyCFunction) py_ped_disk_probe,
          METH_VARARGS, NULL },
        { "is_busy", (PyCFunction) py_ped_device_is_busy,
          METH_VARARGS, NULL },
        { "geometry_new", (PyCFunction) py_ped_geometry_new,
          METH_VARARGS, NULL },
        { "constraint_any", (PyCFunction) py_ped_constraint_any,
          METH_VARARGS, NULL },
        { "disk_new_fresh", (PyCFunction) py_ped_disk_new_fresh,
          METH_VARARGS, NULL },
        { "destroy", (PyCFunction) py_ped_device_destroy,
          METH_VARARGS, NULL },
        { NULL, NULL, 0, NULL }	
};

static PyObject *
py_ped_device_getattr (PyPedDevice * d, char * name)
{
        if (!strcmp (name, "length"))
                return PyLong_FromLongLong (d->dev->length);
        else if (!strcmp (name, "model"))
                return PyString_FromString (d->dev->model);
        else if (!strcmp (name, "path"))
                return PyString_FromString (d->dev->path);
        else if (!strcmp (name, "sector_size"))
                return PyInt_FromLong (d->dev->sector_size);
        else if (!strcmp (name, "type"))
                return PyInt_FromLong (d->dev->type);
        else if (!strcmp (name, "heads"))
                return PyInt_FromLong (d->dev->bios_geom.heads);
        else if (!strcmp (name, "sectors"))
                return PyInt_FromLong (d->dev->bios_geom.sectors);
        else if (!strcmp (name, "cylinders"))
                return PyInt_FromLong (d->dev->bios_geom.cylinders);
        return Py_FindMethod (PyPedDeviceMethods, (PyObject *) d, name);
}

static char PyPedDeviceType__doc__[] = "This is the PartEd device object";
PyTypeObject PyPedDeviceType = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,				/* ob_size */
	"PedDevice",			/* tp_name */
	sizeof(PyPedDevice),		/* tp_size */
	0,				/* tp_itemsize */
	(destructor) py_ped_device_dealloc, 	/* tp_dealloc */
	0,				/* tp_print */
	(getattrfunc) py_ped_device_getattr, 	/* tp_getattr */
	0,				/* tp_setattr */
	0,				/* tp_compare */
	0,				/* tp_repr */
	0,				/* tp_as_number */
	0,	 			/* tp_as_sequence */
	0,				/* tp_as_mapping */
	0,           			/* tp_hash */
	0,                		/* tp_call */
	0,                    		/* tp_str */
	0,				/* tp_getattro */
	0,				/* tp_setattro */
	0,				/* tp_as_buffer */
	0L,	       			/* tp_flags */
	PyPedDeviceType__doc__,
	PYPARTED_TYPE_PAD
};

/* a small PedDevice type used for implementing parted.PedDevice.get('dev') */

/* XXX make static after deprecation period */
PyObject *
PedDevice_get (PyObject * self, PyObject * args)
{
        PyPedDevice *d;
        PedDevice *dev;
        char *path;

        if (!PyArg_ParseTuple(args, "s", &path))
                return NULL;
        
        py_ped_exception_string_clear ();
        if ((dev = ped_device_get (path)) == NULL) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }
    
        d = (PyPedDevice *) PyObject_NEW(PyObject, &PyPedDeviceType);
        d->dev = dev;
        return (PyObject *) d;
}

static void
py_ped_device_constructor_dealloc (PyObject * s)
{
        PyObject_DEL(s);
}

static PyMethodDef PyPedDeviceConstructorMethods[] = 
{
        { "get", PedDevice_get, METH_VARARGS, NULL },
        { NULL, NULL, 0, NULL },
};

static PyObject *
py_ped_device_constructor_getattr (PyObject * s, char * name)
{
        return Py_FindMethod (PyPedDeviceConstructorMethods, s, name);
}

static char PyPedDeviceConstructor__doc__[] = "This is the PartEd PedDisk constructor";
PyTypeObject PyPedDeviceConstructorType = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,				/* ob_size */
	"PedDeviceConstructor",	        /* tp_name */
	sizeof(PyObject),		/* tp_size */
	0,				/* tp_itemsize */
	(destructor) py_ped_device_constructor_dealloc,  /* tp_dealloc */
	0,				/* tp_print */
	(getattrfunc) py_ped_device_constructor_getattr, /* tp_getattr */
	0,				/* tp_setattr */
	0,				/* tp_compare */
	0,				/* tp_repr */
	0,				/* tp_as_number */
	0,	 			/* tp_as_sequence */
	0,				/* tp_as_mapping */
	0,           			/* tp_hash */
	0,                		/* tp_call */
	0,                    		/* tp_str */
	0,				/* tp_getattro */
	0,				/* tp_setattro */
	0,				/* tp_as_buffer */
	0L,	       			/* tp_flags */
	PyPedDeviceConstructor__doc__,
	PYPARTED_TYPE_PAD
};
