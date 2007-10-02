#
# Makefile.am for pyparted
#
# Copyright (C) 2007  Red Hat, Inc.  All rights reserved.
#
# This copyrighted material is made available to anyone wishing to use,
# modify, copy, or redistribute it subject to the terms and conditions of
# the GNU General Public License v.2, or (at your option) any later version.
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY expressed or implied, including the implied warranties of
# MERCHANTABILITY or FITNESS FOR A * PARTICULAR PURPOSE.  See the GNU General
# Public License for more details.  You should have received a copy of the
# GNU General Public License along with this program; if not, write to the
# Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
# 02110-1301, USA.  Any Red Hat trademarks that are incorporated in the
# source code or documentation are not subject to the GNU General Public
# License and may only be used or replicated with the express permission of
# Red Hat, Inc.
#
# Red Hat Author(s): David Cantrell
#

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = include src

EXTRA_DIST = pyparted.spec mkrelease.sh

MAINTAINERCLEANFILES = Makefile.in config.guess config.h.in config.sub \
                       depcomp install-sh ltmain.sh missing

MOSTLYCLEANDIRS = m4

MAINTAINERCLEANFILES += ABOUT-NLS INSTALL aclocal.m4 configure

pyparted.spec: pyparted.spec.in
	sed 's/@''PACKAGE@/$(PACKAGE)/;s/@''VERSION@/$(VERSION)/;s/@''LIBPARTED_VERSION@/$(LIBPARTED_VERSION)/;' $< > $@-tmp
	mv $@-tmp $@
