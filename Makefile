# Makefile for pyparted
#
# Copyright (C) 2006  Red Hat, Inc. All rights reserved.
#
# This copyrighted material is made available to anyone wishing to use,
# modify, copy, or redistribute it subject to the terms and conditions of
# the GNU General Public License v.2.  This program is distributed in the
# hope that it will be useful, but WITHOUT ANY WARRANTY expressed or implied,
# including the implied warranties of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE. See the GNU General Public License for more details.
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. Any Red Hat
# trademarks that are incorporated in the source code or documentation are
# not subject to the GNU General Public License and may only be used or
# replicated with the express permission of Red Hat, Inc.
#
# Red Hat Author(s): David Cantrell
#

SRC = partedmodule.c pyconstraint.c pydevice.c pydisk.c pyexception.c \
      pyfilesystem.c pygeometry.c
HDR = $(SRC:%.c=%.h)
OBJ = $(SRC:%.c=%.o)
TXT = Makefile pyparted.spec AUTHORS COPYING ChangeLog INSTALL NEWS README

PYVER   = $(shell python -c "import sys; print sys.version[:3]")
VERSION = $(shell awk '/Version:/ { print $$2 }' pyparted.spec)
RELEASE = $(shell awk '/Release:/ { print $$2 }' pyparted.spec | sed -e 's|%.*$$||g')

CC     ?= gcc
CFLAGS += -I/usr/include/python$(PYVER) -I. -fPIC

# Build with libparted (parted-1.8.3 and higher have a pkg-config file)
LDFLAGS += $(shell pkg-config --libs libparted)

libdir ?= $(DESTDIR)$(shell rpm --eval "%{_libdir}")/python$(PYVER)/site-packages

all: partedmodule.so

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

partedmodule.so: $(OBJ)
	$(CC) $(CFLAGS) -o $@ -shared $(OBJ) $(LDFLAGS)

clean:
	rm -f $(OBJ) partedmodule.so

install:
	mkdir -p $(libdir)
	install -m 0755 partedmodule.so $(libdir)/partedmodule.so

CVSTAG = pyparted-$(subst .,_,$(VERSION)-$(RELEASE))
tag:
	@cvs tag -cRF $(CVSTAG)
	@echo "Tagged as $(CVSTAG)"

archive: tar

tar: tag
	@rm -rf /tmp/pyparted
	@rm -rf /tmp/pyparted-$(VERSION)
	@tag=`cvs status Makefile|awk ' /Sticky Tag/ { print $$3 } '` 2>/dev/null;\
	[ x"$$tag" = x"(none)" ] && tag=HEAD; \
	[ x"$$TAG" != x ] && tag=$$TAG; \
	cvsroot=`cat CVS/Root` 2>/dev/null; \
	echo "*** Pulling off $$tag from $$cvsroot!"; \
	cd /tmp ; cvs -z3 -Q -d $$cvsroot export -r $$tag pyparted || echo "Um... export aborted."
	@mv /tmp/pyparted /tmp/pyparted-$(VERSION)
	@cd /tmp ; tar --bzip2 -cSpf pyparted-$(VERSION).tar.bz2 pyparted-$(VERSION)
	@rm -rf /tmp/pyparted-$(VERSION)
	@cp /tmp/pyparted-$(VERSION).tar.bz2 .
	@rm -f /tmp/pyparted-$(VERSION).tar.bz2
	@echo
	@echo "The final archive is in pyparted-$(VERSION).tar.bz2"

.PHONY: all clean install tar archive
