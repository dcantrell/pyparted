/*
 * pydevice.h
 * pyparted type definitions for pydevice.c
 *
 * Copyright (C) 2007  Red Hat, Inc.
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
 * Red Hat Author(s): David Cantrell <dcantrell@redhat.com>
 */

#ifndef PYDEVICE_H_INCLUDED
#define PYDEVICE_H_INCLUDED

#include <Python.h>
#include <structmember.h>

#include <parted/parted.h>

/* 1:1 function mappings for device.h in libparted */
PyObject *py_ped_device_probe_all(PyObject *s, PyObject *args);
PyObject *py_ped_device_free_all(PyObject *s, PyObject *args);
PyObject *py_ped_device_get(PyObject *s, PyObject *args);
PyObject *py_ped_device_get_next(PyObject *s, PyObject *args);
PyObject *py_ped_device_is_busy(PyObject *s, PyObject *args);
PyObject *py_ped_device_open(PyObject *s, PyObject *args);
PyObject *py_ped_device_close(PyObject *s, PyObject *args);
PyObject *py_ped_device_destroy(PyObject *s, PyObject *args);
PyObject *py_ped_device_cache_remove(PyObject *s, PyObject *args);
PyObject *py_ped_device_begin_external_access(PyObject *s, PyObject *args);
PyObject *py_ped_device_end_external_access(PyObject *s, PyObject *args);
PyObject *py_ped_device_read(PyObject *s, PyObject *args);
PyObject *py_ped_device_write(PyObject *s, PyObject *args);
PyObject *py_ped_device_sync(PyObject *s, PyObject *args);
PyObject *py_ped_device_sync_fast(PyObject *s, PyObject *args);
PyObject *py_ped_device_check(PyObject *s, PyObject *args);
PyObject *py_ped_device_get_constraint(PyObject *s, PyObject *args);

/* _ped.CHSGeometry type is the Python equiv of PedCHSGeometry in libparted */
typedef struct {
    PyObject_HEAD

    /* a PedCHSGeometry stores three ints */
    int cylinders;
    int heads;
    int sectors;
} _ped_CHSGeometry;

void _ped_CHSGeometry_dealloc(_ped_CHSGeometry *self);
int _ped_CHSGeometry_traverse(_ped_CHSGeometry *self, visitproc visit, void *arg);
int _ped_CHSGeometry_clear(_ped_CHSGeometry *self);
PyObject *_ped_CHSGeometry_get(_ped_CHSGeometry *self, void *closure);
int _ped_CHSGeometry_set(_ped_CHSGeometry *self, PyObject *value,
                         void *closure);

extern PyTypeObject _ped_CHSGeometry_Type_obj;

/* _ped.Device type is the Python equivalent of PedDevice in libparted */
typedef struct {
    PyObject_HEAD

    /* a PedDevice is complex, we will store primitives when appropriate or
     * just other Python objects we've created for the typedefs in libparted */
    char *model;
    char *path;
    long long type;
    long long sector_size;
    long long phys_sector_size;
    long long length;             /* PedSector */
    int open_count;
    int read_only;
    int external_mode;
    int dirty;
    int boot_dirty;
    PyObject *hw_geom;            /* a _ped.CHSGeometry */
    PyObject *bios_geom;          /* a _ped.CHSGeometry */
    short host;
    short did;
} _ped_Device;

void _ped_Device_dealloc(_ped_Device *self);
int _ped_Device_traverse(_ped_Device *self, visitproc visit, void *arg);
int _ped_Device_clear(_ped_Device *self);
PyObject *_ped_Device_get(_ped_Device *self, void *closure);
int _ped_Device_set(_ped_Device *self, PyObject *value, void *closure);

extern PyTypeObject _ped_Device_Type_obj;

#endif /* PYDEVICE_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4
 */
