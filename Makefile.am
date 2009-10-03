#
# Makefile.am for pyparted
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

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = include src tests

EXTRA_DIST = AUTHORS BUGS COPYING NEWS README TODO ChangeLog

MAINTAINERCLEANFILES = Makefile.in config.guess config.h.in config.sub \
                       depcomp install-sh ltmain.sh missing ABOUT-NLS \
                       INSTALL aclocal.m4 configure

CLEANFILES = *~ ChangeLog

MOSTLYCLEANDIRS = m4

dist-hook:
	rm -rf `find $(distdir) -type f -name .gitignore`

ChangeLog:
	(GIT_DIR=.git git log > .changelog.tmp && mv .changelog.tmp ChangeLog; rm -f .changelog.tmp) || (touch ChangeLog; echo 'git directory not found: installing possibly empty ChangeLog.' >&2)

pychecker: all
	$(MAKE) -C src/parted pychecker

tag: dist-gzip
	@if [ -z "$(GPGKEY)" ]; then \
		echo "GPGKEY environment variable missing, please set this to the key ID" ; \
		echo "you want to use to tag the repository." ; \
		exit 1 ; \
	fi
	@git tag -u $(GPGKEY) -m "Tag as $(PACKAGE)-$(VERSION)" -f $(PACKAGE)-$(VERSION)
	@echo "Tagged as $(PACKAGE)-$(VERSION) (GPG signed)"

bumpver:
	@NEWSUBVER=$$((`echo $(PACKAGE_VERSION) |cut -d . -f 2` + 1)) ; \
	NEWVERSION=`echo $(PACKAGE_VERSION).$$NEWSUBVER |cut -d . -f 1,3` ; \
	sed -i "s/AC_INIT(\[$(PACKAGE_NAME)\], \[$(PACKAGE_VERSION)\], \[$(PACKAGE_BUGREPORT\])/AC_INIT(\[$(PACKAGE_NAME)\], \[$$NEWVERSION\], \[$(PACKAGE_BUGREPORT\])/" configure.ac

release: tag
	rm -rf $(PACKAGE)-$(VERSION)
	gzip -dc $(PACKAGE)-$(VERSION).tar.gz | tar -xvf -
	( cd $(PACKAGE)-$(VERSION) && ./configure && make ) || exit 1
	@echo
	@echo "$(PACKAGE)-$(VERSION).tar.gz is now ready to upload."
	@echo "Do not forget to push changes to the repository with:"
	@echo "    git push"
	@echo "    git push --tags"
	@echo
	@echo "Do not forget to add a new Version entry on the Trac site:"
	@echo "    https://fedorahosted.org/pyparted/admin/ticket/versions"
	@echo
