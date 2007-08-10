/*
 * pyconstraint.c
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
#include "pygeometry.h"
#include "pyconstraint.h"

PyPedConstraint *
py_ped_constraint_obj_new (PedConstraint *c, PyPedDevice *dev, int borrowed)
{
        PyPedConstraint *con;
    
        con = (PyPedConstraint *) PyObject_NEW(PyObject, &PyPedConstraintType);
        con->constraint = c;
        con->borrowed = borrowed;
        con->dev = dev;
        Py_XINCREF (dev);

        return con;
}

static PyObject *
py_ped_constraint_duplicate (PyPedConstraint * c, PyObject * args)
{
        PedConstraint *cst;

        cst = ped_constraint_duplicate (c->constraint);
        return (PyObject *) py_ped_constraint_obj_new (cst, c->dev, 0);
}

static void
py_ped_constraint_dealloc (PyPedConstraint * c)
{
        if (!c->borrowed)
                ped_constraint_destroy (c->constraint);
        Py_XDECREF (c->dev);
        PyObject_DEL(c);
}

static struct PyMethodDef PyPedConstraintMethods[] = {
	{ "duplicate",
          (PyCFunction) py_ped_constraint_duplicate,
          METH_VARARGS, NULL },
	{ NULL, NULL, 0, NULL }	
};

static PyObject *
py_ped_constraint_getattr (PyPedConstraint * c, char * name)
{
        if (!strcmp (name, "start_range"))
                return (PyObject *)
                        py_ped_geometry_obj_new (c->constraint->start_range,
                                                 c->dev, 1);
        if (!strcmp (name, "end_range"))
                return (PyObject *)
                        py_ped_geometry_obj_new (c->constraint->end_range,
                                                 c->dev, 1);
        if (!strcmp (name, "min_size"))
                return (PyObject *)
                        PyLong_FromLongLong (c->constraint->min_size);
        
        return Py_FindMethod (PyPedConstraintMethods, (PyObject *) c, name);
}


static char PyPedConstraintType__doc__[] = "This is the PartEd constraint "
                                           "object";
PyTypeObject PyPedConstraintType = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,				/* ob_size */
	"PedConstraint",			/* tp_name */
	sizeof(PyPedConstraint),		/* tp_size */
	0,				/* tp_itemsize */
	(destructor) py_ped_constraint_dealloc, 	/* tp_dealloc */
	0,				/* tp_print */
	(getattrfunc) py_ped_constraint_getattr, 	/* tp_getattr */
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
	PyPedConstraintType__doc__,
	PYPARTED_TYPE_PAD
};

