dnl configure.ac for pyparted
dnl
dnl Copyright (C) 2007, 2008, 2009  Red Hat, Inc.
dnl
dnl This copyrighted material is made available to anyone wishing to use,
dnl modify, copy, or redistribute it subject to the terms and conditions of
dnl the GNU General Public License v.2, or (at your option) any later version.
dnl This program is distributed in the hope that it will be useful, but WITHOUT
dnl ANY WARRANTY expressed or implied, including the implied warranties of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
dnl Public License for more details.  You should have received a copy of the
dnl GNU General Public License along with this program; if not, write to the
dnl Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
dnl 02110-1301, USA.  Any Red Hat trademarks that are incorporated in the
dnl source code or documentation are not subject to the GNU General Public
dnl License and may only be used or replicated with the express permission of
dnl Red Hat, Inc.
dnl
dnl Red Hat Author(s): David Cantrell <dcantrell@redhat.com>

m4_define(libparted_required_version, 2.1)
m4_define(python_required_version, 2.5)

AC_PREREQ(2.59)
AC_INIT([pyparted], [3.0], [dcantrell@redhat.com])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/_pedmodule.c])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile include/Makefile include/docstrings/Makefile include/typeobjects/Makefile src/Makefile src/parted/Makefile tests/Makefile tests/_ped/Makefile tests/parted/Makefile])

AC_CONFIG_MACRO_DIR([m4])

AC_DEFINE_UNQUOTED([BUILD_DATE], ["`date +%m%d%Y`"], [Date of pyparted build])

AC_DISABLE_STATIC
AC_PROG_CPP
AC_PROG_CC

AC_HEADER_STDC

AC_PROG_LIBTOOL

dnl Check for Python
AM_PATH_PYTHON(python_required_version)
AM_CHECK_PYTHON_HEADERS(,[AC_MSG_ERROR(could not find Python headers or library)])

dnl Check for python-decorators
AM_CHECK_PYMOD(decorator, decorator, [],
               [AC_MSG_ERROR(could not import decorator Python module)])

dnl Check for GNU parted
AM_CHECK_LIBPARTED(libparted_required_version)

AC_OUTPUT
