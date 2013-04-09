/*
 * pyunit.h
 * pyparted type definitions for pyunit.c
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

#ifndef PYUNIT_H_INCLUDED
#define PYUNIT_H_INCLUDED

#include <Python.h>

#include <parted/parted.h>

/* a PedUnit is a long int in C, so we store it that way in Python */

/* 1:1 function mappings for unit.h in libparted */
PyObject *py_ped_unit_set_default(PyObject *, PyObject *);
PyObject *py_ped_unit_get_default(PyObject *, PyObject *);
PyObject *py_ped_unit_get_name(PyObject *, PyObject *);
PyObject *py_ped_unit_get_by_name(PyObject *, PyObject *);

#endif /* PYUNIT_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4
 */
