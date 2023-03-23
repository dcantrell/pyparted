/*
 * exceptions.h
 *
 * Copyright The pyparted Project Authors
 * SPDX-License-Identifier: GPL-2.0-or-later                                    */

#ifndef _EXCEPTIONS_H_INCLUDED
#define _EXCEPTIONS_H_INCLUDED

#include <Python.h>

/* custom exceptions for _ped */
extern PyObject *AlignmentException;
extern PyObject *CreateException;
extern PyObject *ConstraintException;
extern PyObject *DeviceException;
extern PyObject *DiskException;
extern PyObject *DiskLabelException;
extern PyObject *FileSystemException;
extern PyObject *GeometryException;
extern PyObject *IOException;
extern PyObject *NotNeededException;
extern PyObject *PartedException;
extern PyObject *PartitionException;
extern PyObject *TimerException;
extern PyObject *UnknownDeviceException;
extern PyObject *UnknownTypeException;

extern unsigned int partedExnRaised;
extern char *partedExnMessage;

#endif /* _EXCEPTIONS_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4
 */
