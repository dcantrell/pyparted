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

#ifndef PYEXCEPTION_H_INCLUDED
#define PYEXCEPTION_H_INCLUDED

#include <Python.h>

#include "parted/exception.h"

extern PyTypeObject PyPedExceptionType;

typedef struct _PyPedException PyPedException;

struct _PyPedException {
        PyObject_HEAD;
        PedException *ex;
};

PyPedException *
py_ped_exception_obj_new (PedException *ex);

#endif /* PYEXCEPTION_H_INCLUDED */

