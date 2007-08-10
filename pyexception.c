/*
 * pyexception.c
 *
 * Copyright (C) 2000  Red Hat, Inc.  All rights reserved.
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
 * Red Hat Author(s): Matt Wilson
 *                    David Cantrell
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
