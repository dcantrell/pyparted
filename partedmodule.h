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
