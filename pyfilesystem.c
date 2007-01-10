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

#include "pyfilesystem.h"
#include "pyconstraint.h"
#include "pygeometry.h"

/*
 * filesystem implementation
 */

PyPedFileSystem *
py_ped_file_system_obj_new (PedFileSystem *filesys,
                            PyPedGeometry *geom)
{
        PyPedFileSystem *fs;
    
        fs = (PyPedFileSystem *) PyObject_NEW(PyObject, &PyPedFileSystemType);
        fs->fs = filesys;
        fs->geom = geom;
        Py_INCREF (geom);
        return fs;
}

static PyObject *
py_ped_file_system_check (PyPedFileSystem * fs, PyObject * args)
{
        py_ped_exception_string_clear ();
        if (!ped_file_system_check (fs->fs, NULL)) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }

        Py_INCREF(Py_None);
        return Py_None;
}

static PyObject *
py_ped_file_system_copy (PyPedFileSystem * fs, PyObject * args)
{
        PyPedGeometry *geom;

        if (!PyArg_ParseTuple(args, "O!", &PyPedGeometryType, &geom))
                return NULL;                

        py_ped_exception_string_clear ();
        if (!ped_file_system_copy (fs->fs, geom->geom, NULL)) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }

        Py_INCREF(Py_None);
        return Py_None;
}

static PyObject *
py_ped_file_system_resize (PyPedFileSystem * fs, PyObject * args)
{
        PyPedGeometry *geom;

        if (!PyArg_ParseTuple(args, "O!", &PyPedGeometryType, &geom))
                return NULL;                

        py_ped_exception_string_clear ();
        if (!ped_file_system_resize (fs->fs, geom->geom, NULL)) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }

        Py_INCREF(Py_None);
        return Py_None;
}

static PyObject *
py_ped_file_system_get_resize_constraint (PyPedFileSystem * fs, PyObject * args)
{
        PedConstraint *constraint;

        py_ped_exception_string_clear ();
        if ((constraint = ped_file_system_get_resize_constraint (fs->fs)) == NULL) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }

        return (PyObject *) py_ped_constraint_obj_new (constraint, fs->geom->dev, 0);
}

static void
py_ped_file_system_dealloc (PyPedFileSystem * fs)
{
        ped_file_system_close (fs->fs);
        Py_XDECREF (fs->geom);
        PyObject_DEL(fs);
}

static struct PyMethodDef PyPedFileSystemMethods[] = {
	{ "check", (PyCFunction) py_ped_file_system_check, METH_VARARGS, NULL },
	{ "copy", (PyCFunction) py_ped_file_system_copy, METH_VARARGS, NULL },
	{ "resize", (PyCFunction) py_ped_file_system_resize, METH_VARARGS, NULL },
	{ "get_resize_constraint", (PyCFunction) py_ped_file_system_get_resize_constraint, METH_VARARGS, NULL },
	{ NULL, NULL, 0, NULL }	
};

static PyObject *
py_ped_file_system_getattr (PyPedFileSystem * fs, char * name)
{
        if (!strcmp (name, "type"))
                return PyString_FromString (fs->fs->type->name);
        if (!strcmp (name, "geom"))
                return (PyObject *) py_ped_geometry_obj_new (fs->fs->geom, fs->geom->dev, 1);
        return Py_FindMethod (PyPedFileSystemMethods, (PyObject *) fs, name);
}

static char PyPedFileSystemType__doc__[] = "This is the PartEd filesystem object";
PyTypeObject PyPedFileSystemType = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,				/* ob_size */
	"PedFileSystem",			/* tp_name */
	sizeof(PyPedFileSystem),		/* tp_size */
	0,				/* tp_itemsize */
	(destructor) py_ped_file_system_dealloc, 	/* tp_dealloc */
	0,				/* tp_print */
	(getattrfunc) py_ped_file_system_getattr, 	/* tp_getattr */
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
	PyPedFileSystemType__doc__,
	PYPARTED_TYPE_PAD
};

/*
 * file system type implementation
 */

PyPedFileSystemTypeObj *
py_ped_file_system_type_obj_new (const PedFileSystemType *fs_type)
{
        PyPedFileSystemTypeObj *fst;
    
        fst = (PyPedFileSystemTypeObj *)
                PyObject_NEW(PyObject, &PyPedFileSystemTypeType);
        fst->fs_type = fs_type;
        return fst;
}

static void
py_ped_file_system_type_dealloc (PyPedFileSystemTypeObj* fst)
{
        PyObject_DEL(fst);
}

static PyObject *
py_ped_file_system_probe_specific (PyPedFileSystemTypeObj * fst,
                                   PyObject * args)
{
        PyPedGeometry *geom;
        PedGeometry *probedgeom;

        if (!PyArg_ParseTuple(args, "O!", &PyPedGeometryType, &geom))
                return NULL;                

        probedgeom = ped_file_system_probe_specific (fst->fs_type, geom->geom);
        if (!probedgeom) {
                Py_INCREF(Py_None);
                return Py_None;
        }
        return (PyObject *) py_ped_geometry_obj_new (probedgeom, NULL, 0);
}

static struct PyMethodDef PyPedFileSystemTypeMethods[] = {
	{ "probe_specific", (PyCFunction) py_ped_file_system_probe_specific, METH_VARARGS, NULL },
	{ NULL, NULL, 0, NULL }	
};

static PyObject *
py_ped_file_system_type_getattr (PyPedFileSystemTypeObj * fst, char * name)
{
        if (!strcmp (name, "name"))
                return PyString_FromString (fst->fs_type->name);
        return Py_FindMethod (PyPedFileSystemTypeMethods,
                              (PyObject *) fst, name);
}

static char PyPedFileSystemTypeType__doc__[] = "This is the PartEd filesystem object";
PyTypeObject PyPedFileSystemTypeType = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,				/* ob_size */
	"PedFileSystemType",			/* tp_name */
	sizeof(PyPedFileSystemType),		/* tp_size */
	0,				/* tp_itemsize */
	(destructor) py_ped_file_system_type_dealloc, 	/* tp_dealloc */
	0,				/* tp_print */
	(getattrfunc) py_ped_file_system_type_getattr, 	/* tp_getattr */
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
	PyPedFileSystemTypeType__doc__,
	PYPARTED_TYPE_PAD
};
