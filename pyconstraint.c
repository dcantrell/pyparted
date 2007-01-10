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
 *
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

