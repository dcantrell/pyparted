/*
 * pydevice.h
 * pyparted type definitions for pydevice.c
 *
 * Copyright (C) 2007-2013 Red Hat, Inc.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions of
 * the GNU General Public License v.2, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY expressed or implied, including the implied warranties of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.  You should have received a copy of the
 * GNU General Public License along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.  Any Red Hat trademarks that are incorporated in the
 * source code or documentation are not subject to the GNU General Public
 * License and may only be used or replicated with the express permission of
 * Red Hat, Inc.
 *
 * Author(s): David Cantrell <dcantrell@redhat.com>
 *            Alex Skinner <alex@lx.lc>
 */

#ifndef PYDEVICE_H_INCLUDED
#define PYDEVICE_H_INCLUDED

#include <Python.h>

#include <parted/parted.h>

/* _ped.CHSGeometry type is the Python equiv of PedCHSGeometry in libparted */
typedef struct {
    PyObject_HEAD

    /* a PedCHSGeometry stores three ints */
    int cylinders;
    int heads;
    int sectors;
} _ped_CHSGeometry;

void _ped_CHSGeometry_dealloc(_ped_CHSGeometry *);
int _ped_CHSGeometry_compare(_ped_CHSGeometry *, PyObject *);
PyObject *_ped_CHSGeometry_richcompare(_ped_CHSGeometry *, PyObject *, int);
PyObject *_ped_CHSGeometry_str(_ped_CHSGeometry *);
int _ped_CHSGeometry_traverse(_ped_CHSGeometry *, visitproc, void *);
int _ped_CHSGeometry_clear(_ped_CHSGeometry *);
PyObject *_ped_CHSGeometry_get(_ped_CHSGeometry *, void *);

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

void _ped_Device_dealloc(_ped_Device *);
int _ped_Device_compare(_ped_Device *, PyObject *);
PyObject *_ped_Device_richcompare(_ped_Device *, PyObject *, int);
PyObject *_ped_Device_str(_ped_Device *);
int _ped_Device_traverse(_ped_Device *, visitproc, void *);
int _ped_Device_clear(_ped_Device *);
PyObject *_ped_Device_get(_ped_Device *, void *);

extern PyTypeObject _ped_Device_Type_obj;

/* 1:1 function mappings for device.h in libparted */
PyObject *py_ped_disk_probe(PyObject *, PyObject *);
PyObject *py_ped_device_probe_all(PyObject *, PyObject *);
PyObject *py_ped_device_free_all(PyObject *, PyObject *);
PyObject *py_ped_device_get(PyObject *, PyObject *);
PyObject *py_ped_device_get_next(PyObject *, PyObject *);
PyObject *py_ped_device_is_busy(PyObject *, PyObject *);
PyObject *py_ped_device_open(PyObject *, PyObject *);
PyObject *py_ped_device_close(PyObject *, PyObject *);
PyObject *py_ped_device_destroy(PyObject *, PyObject *);
PyObject *py_ped_device_cache_remove(PyObject *, PyObject *);
PyObject *py_ped_device_begin_external_access(PyObject *, PyObject *);
PyObject *py_ped_device_end_external_access(PyObject *, PyObject *);
PyObject *py_ped_device_read(PyObject *, PyObject *);
PyObject *py_ped_device_write(PyObject *, PyObject *);
PyObject *py_ped_device_sync(PyObject *, PyObject *);
PyObject *py_ped_device_sync_fast(PyObject *, PyObject *);
PyObject *py_ped_device_check(PyObject *, PyObject *);
PyObject *py_ped_device_get_constraint(PyObject *, PyObject *);
PyObject *py_ped_device_get_minimal_aligned_constraint(PyObject *, PyObject *);
PyObject *py_ped_device_get_optimal_aligned_constraint(PyObject *, PyObject *);
PyObject *py_ped_device_get_minimum_alignment(PyObject *, PyObject *);
PyObject *py_ped_device_get_optimum_alignment(PyObject *, PyObject *);
PyObject *py_ped_unit_get_size(PyObject *, PyObject *);
PyObject *py_ped_unit_format_custom_byte(PyObject *, PyObject *);
PyObject *py_ped_unit_format_byte(PyObject *, PyObject *);
PyObject *py_ped_unit_format_custom(PyObject *, PyObject *);
PyObject *py_ped_unit_format(PyObject *, PyObject *);
PyObject *py_ped_unit_parse(PyObject *, PyObject *);
PyObject *py_ped_unit_parse_custom(PyObject *, PyObject *);

#endif /* PYDEVICE_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4
 */
