/* -*- Mode: c; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*-
 *
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
#include "pygeometry.h"
#include "pyfilesystem.h"

/* geometry implementation */

PyPedGeometry *
py_ped_geometry_obj_new (PedGeometry* geom, PyPedDevice *dev, int borrowed)
{
        PyPedGeometry *g;
    
        g = (PyPedGeometry *) PyObject_NEW (PyObject, &PyPedGeometryType);
        g->geom = geom;
        g->borrowed = borrowed;
        g->dev = dev;
        Py_XINCREF (dev);
        return g;
}

static PyObject *
py_ped_file_system_open (PyPedGeometry * self, PyObject * args)
{
        PedFileSystem *fs;
        PyPedFileSystem *pyfs;

        py_ped_exception_string_clear ();
        if ((fs = ped_file_system_open (self->geom)) == NULL) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }

        pyfs = py_ped_file_system_obj_new (fs, self);
        return (PyObject *) pyfs;
}

static PyObject *
py_ped_file_system_probe (PyPedGeometry * self, PyObject * args)
{
        PedFileSystemType *fst;

        py_ped_exception_string_clear ();
        if ((fst = ped_file_system_probe (self->geom)) == NULL) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }

        return (PyObject *) py_ped_file_system_type_obj_new (fst);
}

static PyObject *
py_ped_file_system_create (PyPedGeometry * self, PyObject * args)
{
        PedFileSystem *fs;
        PyPedFileSystemTypeObj *fs_type;
        PyPedFileSystem *pyfs;

        if (!PyArg_ParseTuple(args, "O!",
                              &PyPedFileSystemTypeType, &fs_type))
                return NULL;

        py_ped_exception_string_clear ();
        if ((fs = ped_file_system_create (self->geom, fs_type->fs_type,
                                          NULL)) == NULL) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }

        pyfs = py_ped_file_system_obj_new (fs, self);
        return (PyObject *) pyfs;
}


static PyObject *
py_ped_file_system_clobber (PyPedGeometry * self, PyObject * args)
{
        py_ped_exception_string_clear ();
        if (!ped_file_system_clobber (self->geom)) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }
        
        Py_INCREF(Py_None);
        return Py_None;
}

static PyObject *
py_ped_geometry_set_end (PyPedGeometry * self, PyObject * args)
{
        PedSector end;

        if (!PyArg_ParseTuple(args, "L", &end))
                return NULL;
        
        py_ped_exception_string_clear ();
        if (!ped_geometry_set_end (self->geom, end)) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }
        
        Py_INCREF(Py_None);
        return Py_None;
}


static PyObject *
py_ped_geometry_set_start (PyPedGeometry * self, PyObject * args)
{
        PedSector start;

        if (!PyArg_ParseTuple(args, "L", &start))
                return NULL;
        
        py_ped_exception_string_clear ();
        if (!ped_geometry_set_start (self->geom, start)) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }
        
        Py_INCREF(Py_None);
        return Py_None;
}

static PyObject *
py_ped_geometry_duplicate (PyPedGeometry * self, PyObject * args)
{
        PedGeometry *geom;

        py_ped_exception_string_clear ();
        if ((geom = ped_geometry_duplicate (self->geom)) == NULL) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }

        return (PyObject *) py_ped_geometry_obj_new (geom, self->dev, 0);
}

static struct PyMethodDef PyPedGeometryMethods[] = {
	{ "file_system_open",
          (PyCFunction) py_ped_file_system_open, METH_VARARGS, NULL },
        { "file_system_probe",
          (PyCFunction) py_ped_file_system_probe, METH_VARARGS, NULL },
        { "file_system_create",
          (PyCFunction) py_ped_file_system_create, METH_VARARGS, NULL },
        { "file_system_clobber",
          (PyCFunction) py_ped_file_system_clobber, METH_VARARGS, NULL },
        { "set_start",
          (PyCFunction) py_ped_geometry_set_start, METH_VARARGS, NULL },
        { "set_end",
          (PyCFunction) py_ped_geometry_set_end, METH_VARARGS, NULL },
        { "duplicate",
          (PyCFunction) py_ped_geometry_duplicate, METH_VARARGS, NULL },

	{ NULL, NULL, 0, NULL }	
};

static void
py_ped_geometry_dealloc (PyPedGeometry * g)
{
        if (!g->borrowed)
                ped_geometry_destroy (g->geom);
        Py_XDECREF (g->dev);
        PyMem_DEL(g);
}

static PyObject *
py_ped_geometry_getattr (PyPedGeometry * g, char * name)
{
        if (!strcmp (name, "dev"))
                return (PyObject *) py_ped_device_new(g->geom->dev);
        
#define RETURN_GEOM_VALUE(var) 					\
        if (!strcmp (name, #var)) 				\
		return PyLong_FromLongLong (g->geom->var)

        RETURN_GEOM_VALUE(start);
        RETURN_GEOM_VALUE(end);
        RETURN_GEOM_VALUE(length);

        return Py_FindMethod (PyPedGeometryMethods, (PyObject *) g, name);
}

static int
py_ped_geometry_setattr (PyPedGeometry * g, char * name, PyObject *value)
{
#define SET_GEOM_VALUE(var) 					\
        if (!strcmp (name, #var)) {				\
                if (!PyLong_Check (value)) { 			\
                        PyErr_SetString(PyExc_TypeError,	\
                                         "expected long long");	\
                        return -1; 				\
		}						\
                g->geom->var = PyLong_AsLongLong (value);	\
                return 0; 					\
        }
        SET_GEOM_VALUE(start);
        SET_GEOM_VALUE(end);
        SET_GEOM_VALUE(length);
        
        PyErr_SetString( PyExc_AttributeError, "unknown attribute" );
        return -1;
}

static char PyPedGeometryType__doc__[] = "This is the PartEd geometry object";
PyTypeObject PyPedGeometryType = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,				/* ob_size */
	"PedGeometry",			/* tp_name */
	sizeof(PyPedGeometry),		/* tp_size */
	0,				/* tp_itemsize */
	(destructor) py_ped_geometry_dealloc, 	/* tp_dealloc */
	0,				/* tp_print */
	(getattrfunc) py_ped_geometry_getattr, 	/* tp_getattr */
	(setattrfunc) py_ped_geometry_setattr,  /* tp_setattr */
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
	PyPedGeometryType__doc__,
	PYPARTED_TYPE_PAD
};

