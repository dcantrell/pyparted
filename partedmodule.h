/*
 * partedmodule.h
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

#ifndef PARTEDMODULE_H_INCLUDED
#define PARTEDMODULE_H_INCLUDED

#include <Python.h>

extern PyObject *PyPartedError;
int py_ped_exception_string_check (void);
void py_ped_exception_string_clear (void);
extern void py_ped_set_error_from_ped_exception (void);

#if (PYTHON_API_VERSION >= 1011)
#define PYPARTED_TYPE_PAD 0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L
#else
#define PYPARTED_TYPE_PAD 0L,0L,0L,0L
#endif

#endif /* PARTEDMODULE_H_INCLUDED */
