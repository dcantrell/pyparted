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
#include "pyexception.h"
/* exception implementation */

PyPedException *
py_ped_exception_obj_new (PedException *ex)
{
        PyPedException *e;
    
        e = (PyPedException *) PyObject_NEW(PyObject, &PyPedExceptionType);
        e->ex = ex;
        return e;
}

static void
py_ped_exception_dealloc (PyPedException * s)
{
        PyObject_DEL(s);
}

static PyObject *
py_ped_exception_getattr (PyPedException * e, char * name)
{
        if (!strcmp (name, "message"))
                return PyString_FromString (e->ex->message);
        else if (!strcmp (name, "type"))
                return PyInt_FromLong (e->ex->type);
        else if (!strcmp (name, "type_string"))
                return PyString_FromString (ped_exception_get_type_string (e->ex->type));
        else if (!strcmp (name, "options"))
                return PyInt_FromLong (e->ex->options);
        return NULL;
}

static char PyPedExceptionType__doc__[] = "This is the PartEd exception object";

PyTypeObject PyPedExceptionType = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,				/* ob_size */
	"PedException",			/* tp_name */
	sizeof(PyPedException),		/* tp_size */
	0,				/* tp_itemsize */
	(destructor) py_ped_exception_dealloc, 	/* tp_dealloc */
	0,				/* tp_print */
	(getattrfunc) py_ped_exception_getattr, 	/* tp_getattr */
	0,				/* tp_setattr */
	0,				/* tp_compare */
	0,				/* tp_repr */
	0,				/* tp_as_number */
	0,	 			/* tp_as_sequence */
	0,				/* tp_as_mapping */
	0,           			/*tp_hash*/
	0,                		/*tp_call*/
	0,                    		/*tp_str*/
	0,				/*tp_getattro*/
	0,				/*tp_setattro*/
	0,				/*tp_as_buffer*/
	0L,	       			/*tp_flags*/
	PyPedExceptionType__doc__,
	PYPARTED_TYPE_PAD
};
