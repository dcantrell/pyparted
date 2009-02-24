dnl libparted.m4 - libparted checks
dnl
dnl Copyright (C) 2007  Red Hat, Inc.
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

dnl Check for GNU parted
dnl $1   Minimum version of libparted we require (e.g., 1.8.6)
AC_DEFUN([AM_CHECK_LIBPARTED],
[PKG_CHECK_MODULES(libparted, libparted >= $1)

AC_SUBST(LIBPARTED_LIBS)

AC_CHECK_LIB([parted], [ped_get_version], [:],
             [AC_MSG_FAILURE([*** Unable to find requested library libparted])])

AC_CHECK_HEADERS([parted/parted.h], [],
                 [AC_MSG_FAILURE([*** Header file $ac_header not found.])])

dnl Use pkg-config to gather compile flags
LIBPARTED_LIBS="$(pkg-config --libs libparted)"

LIBPARTED_VERSION=$1
])
