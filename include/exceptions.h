/*
 * exceptions.h
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
 * Author(s): Chris Lumens <clumens@redhat.com>
 *            Alex Skinner <alex@lx.lc>
 */

#ifndef _EXCEPTIONS_H_INCLUDED
#define _EXCEPTIONS_H_INCLUDED

#include <Python.h>

/* custom exceptions for _ped */
PyObject *AlignmentException;
PyObject *CreateException;
PyObject *ConstraintException;
PyObject *DeviceException;
PyObject *DiskException;
PyObject *DiskLabelException;
PyObject *FileSystemException;
PyObject *GeometryException;
PyObject *IOException;
PyObject *NotNeededException;
PyObject *PartedException;
PyObject *PartitionException;
PyObject *TimerException;
PyObject *UnknownDeviceException;
PyObject *UnknownTypeException;

extern unsigned int partedExnRaised;
extern char *partedExnMessage;

#endif /* _EXCEPTIONS_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4
 */
