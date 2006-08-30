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

#ifndef PYDEVICE_H_INCLUDED
#define PYDEVICE_H_INCLUDED

#include <Python.h>

#include <parted/constraint.h>
#include <parted/device.h>

extern PyTypeObject PyPedDeviceType;
typedef struct _PyPedDevice PyPedDevice;

struct _PyPedDevice {
        PyObject_HEAD;
        PedDevice *dev;
};

PyPedDevice * py_ped_device_new (PedDevice *dev);

extern PyTypeObject PyPedDeviceConstructorType;

#endif /* PYDEVICE_H_INCLUDED */

