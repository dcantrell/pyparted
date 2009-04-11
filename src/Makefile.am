#
# Makefile.am for pyparted src subdirectory
#
# Copyright (C) 2007, 2008, 2009  Red Hat, Inc.
#
# This copyrighted material is made available to anyone wishing to use,
# modify, copy, or redistribute it subject to the terms and conditions of
# the GNU General Public License v.2, or (at your option) any later version.
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY expressed or implied, including the implied warranties of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
# Public License for more details.  You should have received a copy of the
# GNU General Public License along with this program; if not, write to the
# Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
# 02110-1301, USA.  Any Red Hat trademarks that are incorporated in the
# source code or documentation are not subject to the GNU General Public
# License and may only be used or replicated with the express permission of
# Red Hat, Inc.
#
# Red Hat Author(s): David Cantrell <dcantrell@redhat.com>
#

SUBDIRS = parted

CFLAGS += -Werror -Wmissing-prototypes -fno-strict-aliasing

INCLUDES = -I$(top_srcdir)/include $(PYTHON_INCLUDES) $(LIBPARTED_INCLUDES)

# install Python module code directly in to site-packages
pkgpyexecdir = $(pyexecdir)
pkgpythondir = $(pythondir)

# libparted binding
pkgpyexec_LTLIBRARIES = _pedmodule.la
_pedmodule_la_CFLAGS = $(LIBPARTED_CFLAGS)
_pedmodule_la_LDFLAGS = -module -avoid-version $(PYTHON_LDFLAGS) \
                           $(LIBPARTED_LDFLAGS)
_pedmodule_la_LIBADD = $(PYTHON_LIBS) $(LIBPARTED_LIBS)
_pedmodule_la_SOURCES = convert.c _pedmodule.c pyconstraint.c pydevice.c \
                        pydisk.c pyfilesys.c pygeom.c pynatmath.c pytimer.c \
                        pyunit.c

MAINTAINERCLEANFILES = Makefile.in
