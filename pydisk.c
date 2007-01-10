/* -*- Mode: c; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*-
 *
 * Matt Wilson <msw@redhat.com>
 * Peter Jones <pjones@redhat.com>
 *
 * Copyright 2000,2005 Red Hat, Inc.
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
#include "pydisk.h"
#include "pydevice.h"
#include "pygeometry.h"
#include "pyfilesystem.h"
#include "pyconstraint.h"

static int py_ped_disk_contains_partition (PedDisk * disk, PedPartition *part);
/* disk type implementation */

PyPedDiskTypeObj *
py_ped_disk_type_obj_new (const PedDiskType *type)
{
        PyPedDiskTypeObj *d;
    
        d = (PyPedDiskTypeObj *) PyObject_NEW(PyObject, &PyPedDiskTypeType);
        d->type = (PedDiskType *) type;
        return d;
}

static PyObject *
py_ped_disk_type_check_feature (PyPedDiskTypeObj *d, PyObject * args)
{
        int feature;

        if (!PyArg_ParseTuple(args, "i", &feature))
                return NULL;
        
        return PyInt_FromLong (ped_disk_type_check_feature (d->type,
                                                            feature));
}

static void
py_ped_disk_type_dealloc (PyPedDiskTypeObj * d)
{
        PyObject_DEL(d);
}

static struct PyMethodDef PyPedDiskTypeMethods[] = {
        { "check_feature", (PyCFunction) py_ped_disk_type_check_feature,
          METH_VARARGS, NULL },
	{ NULL, NULL, 0, NULL }	
};

static PyObject *
py_ped_disk_type_get (PyPedDiskTypeObj * d, char * name)
{
        if (!strcmp (name, "name"))
                return PyString_FromString (d->type->name);
        return NULL;
}

static PyGetSetDef PyPedDiskTypeGetSeters[] = {
	{"name", (getter)py_ped_disk_type_get, NULL, "disk name", "name"},
	{NULL}
};

static char PyPedDiskTypeType__doc__[] = "This is the PartEd disk type object";

PyTypeObject PyPedDiskTypeType = {
	PyObject_HEAD_INIT(&PyType_Type)
	.tp_name = "PedDiskType",
	.tp_basicsize = sizeof(PyPedDiskTypeObj),
	.tp_dealloc = (destructor) py_ped_disk_type_dealloc,
	.tp_getset = PyPedDiskTypeGetSeters,
	.tp_methods = PyPedDiskTypeMethods,
	.tp_doc = PyPedDiskTypeType__doc__,
	.tp_new = PyType_GenericNew,
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_CHECKTYPES |
                    Py_TPFLAGS_BASETYPE,
};

/* disk implementation */

PyPedDisk *
py_ped_disk_obj_new (PedDisk *disk, int borrowed)
{
        PyPedDisk *d;
    
        d = (PyPedDisk *) PyObject_NEW(PyObject, &PyPedDiskType);
        d->borrowed = borrowed;
        d->closed = 0;
        d->disk = disk;

        return d;
}

static PyObject *
py_ped_disk_next_partition (PyPedDisk * d, PyObject * args)
{
        PyPedPartition *part = NULL;
        PedPartition *ppart;
    
        if (!PyArg_ParseTuple(args, "|O!", &PyPedPartitionType, &part)) {
                PyErr_SetString(PyExc_TypeError,
                                "optional parameter must be a PedPartition");
                return NULL;
        }
        
        if (part && !py_ped_disk_contains_partition (d->disk, part->part)) {
                PyErr_SetString(PyExc_TypeError, "partition passed as current "
                                "partition does not exist in PedDisk");
                return NULL;
        }
        
        ppart = ped_disk_next_partition (d->disk, part ? part->part : NULL);
        if (ppart != NULL)
                return (PyObject *) py_ped_partition_obj_new (ppart, d, 1);

        Py_INCREF(Py_None);
        return Py_None;
}

static int
py_ped_disk_contains_partition (PedDisk * disk, PedPartition *part)
{
        PedPartition *walk = ped_disk_next_partition (disk, NULL);

        while (walk) {
                if (walk == part)
                        return 1;
                walk = ped_disk_next_partition (disk, walk);
        }
        return 0;
}



static PyObject *
py_ped_disk_delete_partition (PyPedDisk * d, PyObject * args)
{
        PyPedPartition *part;
        
        if (!PyArg_ParseTuple(args, "O!", &PyPedPartitionType, &part)) {
                PyErr_SetString(PyExc_TypeError, "PedPartition expected");
                return NULL;
        }

        py_ped_exception_string_clear ();

        if (!py_ped_disk_contains_partition (d->disk, part->part)) {
                PyErr_SetString(PyExc_TypeError, "PedPartition does not exist "
                                "in PedDisk");
                return NULL;
        }
        
        if (!ped_disk_delete_partition (d->disk, part->part)) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }
        /* the partition structure is freed as part of the
           delete_partition call.  Protect from the double free
           by marking it borrowed */
        part->borrowed = 1;

        Py_INCREF(Py_None);
        return Py_None;
}


static PyObject *
py_ped_disk_delete_all (PyPedDisk * d, PyObject * args)
{
        py_ped_exception_string_clear ();
        if (!ped_disk_delete_all (d->disk)) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }

        Py_INCREF(Py_None);
        return Py_None;
}

static PyObject *
py_ped_disk_add_partition (PyPedDisk * d, PyObject * args)
{
        PyPedPartition *part;
        PyPedConstraint *constraint;

        if (!PyArg_ParseTuple(args, "O!O!", &PyPedPartitionType, &part,
                              &PyPedConstraintType, &constraint)) {
                PyErr_SetString(PyExc_TypeError, "bad argument");
                return NULL;
        }

        py_ped_exception_string_clear ();
        if (!ped_disk_add_partition (d->disk, part->part,
                                     constraint->constraint)) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }

        /* the disk takes ownership of the data in this partition
           when added, protect from double free */
        part->borrowed = 1;

        Py_INCREF(Py_None);
        return Py_None;
}

static PyObject *
py_ped_disk_get_partition_by_sector (PyPedDisk * d, PyObject * args)
{
        PedPartition *part;
        PedSector sect;
        
        if (!PyArg_ParseTuple(args, "L", &sect))
                return NULL;

        if ((part =
             ped_disk_get_partition_by_sector (d->disk, sect)) == NULL) {
                PyErr_SetString(PyPartedError, "partition not found");
                return NULL;
        }
        
        return (PyObject *) py_ped_partition_obj_new (part, d, 1);
}

static PyObject *
py_ped_disk_get_partition (PyPedDisk * d, PyObject * args)
{
        PedPartition *part;
        int num;
        
        if (!PyArg_ParseTuple(args, "i", &num))
                return NULL;

        if ((part = ped_disk_get_partition (d->disk, num)) == NULL) {
                PyErr_SetString(PyPartedError, "partition not found");
                return NULL;
        }
        
        return (PyObject *) py_ped_partition_obj_new (part, d, 1);
}

static PyObject *
py_ped_partition_new (PyPedDisk * self, PyObject * args)
{
        PyPedFileSystemTypeObj *fs_type;
        PedSector start, end;
        PedPartition *part;
        PedPartitionType type;
        PyPedPartition *pypart;

        if (!PyArg_ParseTuple(args, "iOLL",
                              &type,
                              &fs_type,
                              &start, &end))
                return NULL;

        if ((PyObject *) fs_type != Py_None
            && fs_type->ob_type != &PyPedFileSystemTypeType) {
                PyErr_SetString(PyExc_TypeError,
                                "The second argument must be either None "
                                "or a PedFileSystemType.");
                return NULL;
        }
        
        if ((PyObject *) fs_type == Py_None)
                fs_type = NULL;
        
        py_ped_exception_string_clear ();
        part = ped_partition_new (self->disk, type,
                                  fs_type ? fs_type->fs_type : NULL,
                                  start, end);
        if (part == NULL) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }

        pypart = py_ped_partition_obj_new (part, self, 0);

        return (PyObject *) pypart;
}

static PyObject *
py_ped_disk_minimize_extended_partition (PyPedDisk * self, PyObject * args)
{
        py_ped_exception_string_clear ();
        if (!ped_disk_minimize_extended_partition (self->disk)) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }
        Py_INCREF(Py_None);
        return Py_None;
        
}

static PyObject *
py_ped_disk_maximize_partition (PyPedDisk * self, PyObject * args)
{
        PyPedPartition *part;
        PyPedConstraint *constraint;

        if (!PyArg_ParseTuple(args, "O!O!", &PyPedPartitionType, &part,
                              &PyPedConstraintType, &constraint)) {
                return NULL;
        }

        py_ped_exception_string_clear ();
        if (!ped_disk_maximize_partition (self->disk, part->part,
                                          constraint->constraint)) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }
        Py_INCREF(Py_None);
        return Py_None;
}

static PyObject *
py_ped_disk_get_primary_partition_count (PyPedDisk *d, PyObject * args)
{
        int count;
        
        py_ped_exception_string_clear ();
        count = ped_disk_get_primary_partition_count (d->disk);
        if (py_ped_exception_string_check ()) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }

        return PyInt_FromLong (count);
}

static PyObject *
py_ped_disk_get_last_partition_num (PyPedDisk *d, PyObject * args)
{
        int num;
        
        py_ped_exception_string_clear ();
        num = ped_disk_get_last_partition_num (d->disk);
        if (py_ped_exception_string_check ()) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }

        return PyInt_FromLong (num);
}

static PyObject *
py_ped_disk_commit (PyPedDisk *d, PyObject * args)
{
        int rc;
        
        py_ped_exception_string_clear ();
        rc = ped_disk_commit (d->disk);
        if (py_ped_exception_string_check ()) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }

        return PyInt_FromLong (rc);
}

static void
py_ped_disk_dealloc (PyPedDisk * d)
{
        if (!d->borrowed && !d->closed)
                ped_disk_destroy (d->disk);
        PyObject_DEL(d);
}

static struct PyMethodDef PyPedDiskMethods[] = {
	{ "next_partition", (PyCFunction) py_ped_disk_next_partition,
          METH_VARARGS, NULL },
	{ "delete_partition", (PyCFunction) py_ped_disk_delete_partition,
          METH_VARARGS, NULL },
	{ "delete_all", (PyCFunction) py_ped_disk_delete_all,
          METH_VARARGS, NULL },
	{ "add_partition", (PyCFunction) py_ped_disk_add_partition,
          METH_VARARGS, NULL },
	{ "get_partition", (PyCFunction) py_ped_disk_get_partition,
          METH_VARARGS, NULL },
	{ "get_partition_by_sector",
          (PyCFunction) py_ped_disk_get_partition_by_sector,
          METH_VARARGS, NULL },
        { "partition_new", (PyCFunction) py_ped_partition_new,
          METH_VARARGS, NULL },
        { "minimize_extended_partition",
          (PyCFunction) py_ped_disk_minimize_extended_partition,
          METH_VARARGS, NULL },
        { "maximize_partition", (PyCFunction) py_ped_disk_maximize_partition,
          METH_VARARGS, NULL },
        { "get_primary_partition_count",
          (PyCFunction) py_ped_disk_get_primary_partition_count,
          METH_VARARGS, NULL },
        { "get_last_partition_num",
          (PyCFunction) py_ped_disk_get_last_partition_num,
          METH_VARARGS, NULL },
        { "commit",
          (PyCFunction) py_ped_disk_commit,
          METH_VARARGS, NULL },
	{ NULL, NULL, 0, NULL }	
};

static PyObject *
py_ped_disk_get (PyPedDisk * d, char * name)
{
        if (!strcmp (name, "dev"))
                return (PyObject *) py_ped_device_new (d->disk->dev);
        if (!strcmp (name, "type"))
                return (PyObject *) py_ped_disk_type_obj_new (d->disk->type);
        if (!strcmp (name, "max_primary_partition_count")) {
                int num;

                py_ped_exception_string_clear ();
                num = ped_disk_get_max_primary_partition_count (d->disk);
                if (num == 0) {
                        py_ped_set_error_from_ped_exception ();
                        return NULL;
                }
                return PyInt_FromLong (num);
        }
        if (!strcmp (name, "extended_partition")) {
                PedPartition *ppart;

                py_ped_exception_string_clear ();
                ppart = ped_disk_extended_partition (d->disk);
                if (ppart != NULL)
                        return (PyObject *) py_ped_partition_obj_new (ppart,
                                                                      d, 1);

                if (py_ped_exception_string_check ()) {
                        py_ped_set_error_from_ped_exception ();
                        return NULL;
                }
                Py_INCREF(Py_None);
                return Py_None;
        }
	return NULL;
}

static PyGetSetDef PyPedDiskGetSeters[] = {
	{"dev", (getter)py_ped_disk_get, NULL,
	 "the PedDevice for this disk", "dev"},
	{"type", (getter)py_ped_disk_get, NULL,
	 "the PedDiskType for this disk", "type"},
	{"max_primary_partition_count", (getter)py_ped_disk_get, NULL,
	 "the maximum number of primary partitions for this disk",
	 "max_primary_partition_count"},
	{"extended_partition", (getter)py_ped_disk_get, NULL,
	 "extended_partition", "extended_partition"},
	{NULL}
};

static char PyPedDiskType__doc__[] = "This is the PartEd disk object";

PyTypeObject PyPedDiskType = {
	PyObject_HEAD_INIT(&PyType_Type)
	.tp_name = "PedDisk",
	.tp_basicsize = sizeof(PyPedDisk),
	.tp_dealloc = (destructor) py_ped_disk_dealloc,
	.tp_getset = PyPedDiskGetSeters,
	.tp_methods = PyPedDiskMethods,
	.tp_new = PyType_GenericNew,
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_CHECKTYPES |
		   Py_TPFLAGS_BASETYPE,
	.tp_doc = PyPedDiskType__doc__,
};


/* partition implementation */

PyPedPartition *
py_ped_partition_obj_new (PedPartition *part, PyPedDisk *disk, int borrowed)
{
        PyPedPartition *p;
    
        p = (PyPedPartition *) PyObject_NEW(PyObject, &PyPedPartitionType);
        p->part = part;
        p->borrowed = borrowed;
        p->disk = disk;
        Py_INCREF (disk);

        return p;
}

static PyObject *
py_ped_partition_is_flag_available (PyPedPartition *p, PyObject * args)
{
        int av, flag;
        
        if (!PyArg_ParseTuple(args, "i", &flag))
                return NULL;
        
        py_ped_exception_string_clear ();
        av = ped_partition_is_flag_available (p->part, flag);
        if (py_ped_exception_string_check ()) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }

        return PyInt_FromLong (av);
}

static PyObject *
py_ped_partition_set_flag (PyPedPartition *p, PyObject * args)
{
        int flag, state;
        
        if (!PyArg_ParseTuple(args, "ii", &flag, &state))
                return NULL;
        
        py_ped_exception_string_clear ();
        if (!ped_partition_set_flag (p->part, flag, state)) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }

        Py_INCREF(Py_None);
        return Py_None;
}

static PyObject *
py_ped_partition_get_flag (PyPedPartition *p, PyObject * args)
{
        int flag, state;
        
        if (!PyArg_ParseTuple(args, "i", &flag))
                return NULL;
        
        py_ped_exception_string_clear ();
        state = ped_partition_get_flag (p->part, flag);
        if (py_ped_exception_string_check ()) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }

        return PyInt_FromLong (state);
}


static PyObject *
py_ped_partition_is_active (PyPedPartition *p, PyObject * args)
{
        int state;
        
        py_ped_exception_string_clear ();
        state = ped_partition_is_active (p->part);
        if (py_ped_exception_string_check ()) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }

        return PyInt_FromLong (state);
}

static PyObject *
py_ped_partition_set_system (PyPedPartition *p, PyObject * args)
{
        PyPedFileSystemTypeObj *fs_type;

        if (!PyArg_ParseTuple(args, "O!",
                              &PyPedFileSystemTypeType, &fs_type))
                return NULL;
        
        py_ped_exception_string_clear ();
        if (!ped_partition_set_system (p->part, fs_type->fs_type)) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }
        
        Py_INCREF(Py_None);
        return Py_None;
}

static PyObject *
py_ped_partition_set_name (PyPedPartition *p, PyObject * args)
{
        char *name;

        if (!PyArg_ParseTuple(args, "s", &name))
                return NULL;
        
        py_ped_exception_string_clear ();
        if (!ped_partition_set_name (p->part, name)) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }
        
        Py_INCREF(Py_None);
        return Py_None;
}

static PyObject *
py_ped_partition_get_name (PyPedPartition *p, PyObject * args)
{
        const char *name;
        
        py_ped_exception_string_clear ();
        if ((name = ped_partition_get_name (p->part)) == NULL) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }

        return PyString_FromString (name);
}


static PyObject *
py_ped_partition_is_busy (PyPedPartition *p, PyObject * args)
{
        int state;
        
        py_ped_exception_string_clear ();
        state = ped_partition_is_busy (p->part);
        if (py_ped_exception_string_check ()) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }

        return PyInt_FromLong (state);
}

/* XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX */
/*    REMOVE THIS WHEN Ped FLAGS ARE IMPLEMENTED                           */
typedef struct {
	unsigned char	system;
	int		boot;
	int		hidden;
	int		raid;
	int		lvm;
	int		lba;
} DosPartitionData;

static int
py_ped_partition_get_native_type (PedPartition *part)
{
	DosPartitionData*	dos_data;

        if (part->disk == NULL)
                return -1;
        if (strcmp (part->disk->type->name, "msdos"))
                return -1;
        
	dos_data = part->disk_specific;
        return dos_data->system;
}
/* XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX */

static void
py_ped_partition_dealloc (PyPedPartition * p)
{
        if (!p->borrowed)
                ped_partition_destroy (p->part);
        Py_XDECREF (p->disk);
        PyObject_DEL(p);
}

static struct PyMethodDef PyPedPartitionMethods[] = {
        { "is_flag_available",
          (PyCFunction) py_ped_partition_is_flag_available,
          METH_VARARGS, NULL },
        { "get_flag", (PyCFunction) py_ped_partition_get_flag,
          METH_VARARGS, NULL },
        { "set_flag", (PyCFunction) py_ped_partition_set_flag,
          METH_VARARGS, NULL },
        { "is_active", (PyCFunction) py_ped_partition_is_active,
          METH_VARARGS, NULL },
        { "set_system", (PyCFunction) py_ped_partition_set_system,
          METH_VARARGS, NULL },
        { "set_name", (PyCFunction) py_ped_partition_set_name,
          METH_VARARGS, NULL },
        { "get_name", (PyCFunction) py_ped_partition_get_name,
          METH_VARARGS, NULL },
        { "is_busy", (PyCFunction) py_ped_partition_is_busy,
          METH_VARARGS, NULL },
	{ NULL, NULL, 0, NULL }	
};

static PyObject *
py_ped_partition_get (PyPedPartition * p, char * name)
{
        if (!strcmp (name, "num"))
                return PyInt_FromLong (p->part->num);
        if (!strcmp (name, "type"))
                return PyInt_FromLong (p->part->type);
        if (!strcmp (name, "disk"))
                return (PyObject *) py_ped_disk_obj_new (p->part->disk, 1);

        /* XXX REMOVE ME */
        if (!strcmp (name, "native_type"))
                return PyInt_FromLong (py_ped_partition_get_native_type (p->part));
        /*  XXX XXX XXX  */
        if (!strcmp (name, "type_name"))
                return PyString_FromString (
                        ped_partition_type_get_name (p->part->type));
        if (!strcmp (name, "geom"))
                return (PyObject *) py_ped_geometry_obj_new (&p->part->geom,
                                                             NULL, 1);
        if (!strcmp (name, "fs_type")) {
                if (p->part->fs_type)
                        return (PyObject *)
                           py_ped_file_system_type_obj_new (p->part->fs_type);
                Py_INCREF(Py_None);
                return Py_None;
        }
	return NULL;
}

static struct PyGetSetDef PyPedPartitionGetSeters[] = {
	{"num", (getter)py_ped_partition_get, NULL,
	 "partition number", "num"},
	{"type", (getter)py_ped_partition_get, NULL,
	 "partition type", "type"},
	{"disk", (getter)py_ped_partition_get, NULL,
	 "PedDisk object this partition belongs to", "disk"},
        /* XXX REMOVE ME */
	{"native_type", (getter)py_ped_partition_get, NULL,
	 "native_type", "native_type"},
        /*  XXX XXX XXX  */
	{"type_name", (getter)py_ped_partition_get, NULL,
	 "type_name", "type_name"},
	{"geom", (getter)py_ped_partition_get, NULL,
	 "the geometry of this partition", "geom"},
	{"fs_type", (getter)py_ped_partition_get, NULL,
	 "fs_type", "fs_type"},
	{NULL}
};

static char PyPedPartitionType__doc__[] = "This is the PartEd partition object";

PyTypeObject PyPedPartitionType = {
	PyObject_HEAD_INIT(&PyType_Type)
	.tp_name = "PedPartition",
	.tp_basicsize = sizeof(PyPedPartition),
	.tp_dealloc = (destructor) py_ped_partition_dealloc,
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_CHECKTYPES |
		    Py_TPFLAGS_BASETYPE,
	.tp_doc = PyPedPartitionType__doc__,
	.tp_methods = PyPedPartitionMethods,
	.tp_getset = PyPedPartitionGetSeters,
	.tp_new = PyType_GenericNew,
};

/* a small PedDisk type used for implementing parted.PedDisk.open(dev) */

static PyObject* PedDisk_new(PyObject *self, PyObject *args)
{
        PyPedDevice *dev;
        PedDisk *disk;

        if (!PyArg_ParseTuple(args, "O!", &PyPedDeviceType, &dev)) {
                PyErr_SetString(PyExc_TypeError,
                                "parameter must be a PedDevice");
                return NULL;
        }
        py_ped_exception_string_clear ();    
        if ((disk = ped_disk_new (dev->dev)) == NULL) {
                py_ped_set_error_from_ped_exception ();
                return NULL;
        }

        return (PyObject *) py_ped_disk_obj_new (disk, 0);
}

static void
py_ped_disk_constructor_dealloc (PyObject * s)
{
        PyObject_DEL(s);
}

static PyMethodDef PyPedDiskConstructorMethods[] = 
{
        { "new", PedDisk_new, METH_VARARGS, NULL },
        { NULL, NULL, 0, NULL },
};

static char PyPedDiskConstructor__doc__[] = "This is the PartEd PedDisk constructor";
PyTypeObject PyPedDiskConstructorType = {
	PyObject_HEAD_INIT(&PyType_Type)
	.tp_name = "PedDiskConstructor",
	.tp_basicsize = sizeof(PyObject),
	.tp_dealloc = (destructor) py_ped_disk_constructor_dealloc,
	.tp_methods = PyPedDiskConstructorMethods,
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_CHECKTYPES |
		    Py_TPFLAGS_BASETYPE,
	.tp_doc = PyPedDiskConstructor__doc__,
	.tp_new = PyType_GenericNew,
};
