#
# Makefile for pyparted
# Copyright (C) 2007-2011  Red Hat, Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# Author(s): David Cantrell <dcantrell@redhat.com>
#

PYTHON       ?= python
PYCHECKER    ?= pychecker

DESTDIR      ?= /

PACKAGE       = $(shell $(PYTHON) setup.py --name)
VERSION       = $(shell $(PYTHON) setup.py --version)

TAG           = $(PACKAGE)-$(VERSION)

PYCHECKEROPTS = --no-override --no-argsused --no-miximport --maxargs 0 --no-local -\# 0 --only -Q

default: all

all:
	@$(PYTHON) setup.py build

test: all
	@env PYTHONPATH=$$(find $$(pwd) -name "*.so" | head -n 1 | xargs dirname) \
	$(PYTHON) -m unittest discover -v

check: all
	env PYTHONPATH=$$(find $$(pwd) -name "*.so" | head -n 1 | xargs dirname) \
	$(PYCHECKER) $(PYCHECKEROPTS) src/parted/*.py

ChangeLog:
	git log > ChangeLog

dist: ChangeLog
	@$(PYTHON) setup.py sdist

tag: dist
	@if [ -z "$(GPGKEY)" ]; then \
		echo "GPGKEY environment variable missing, please set this to the key ID" ; \
		echo "you want to use to tag the repository." ; \
		exit 1 ; \
	fi
	@git tag -u $(GPGKEY) -m "Tag as $(TAG)" -f $(TAG)
	@echo "Tagged as $(TAG) (GPG signed)"

release: tag
	( cd dist ; gzip -dc $(PACKAGE)-$(VERSION).tar.gz | tar -xvf - )
	( cd dist/$(PACKAGE)-$(VERSION) && $(PYTHON) setup.py build ) || exit 1
	@echo
	@echo "$(PACKAGE)-$(VERSION).tar.gz is now ready to upload."
	@echo "Do not forget to push changes to the repository with:"
	@echo "    git push"
	@echo "    git push --tags"
	@echo
	@echo "Do not forget to add a new Version entry on the Trac site:"
	@echo "    https://fedorahosted.org/pyparted/admin/ticket/versions"
	@echo

rpmlog:
	@prevtag="$$(git tag -l | grep -v "^start$$" | tail -n 2 | head -n 1)" ; \
	git log --pretty="format:- %s (%ae)" $${prevtag}.. | \
	sed -e 's/@.*)/)/' | \
	sed -e 's/%/%%/g' | \
	grep -v "New version" | \
	fold -s -w 77 | \
	while read line ; do \
		if [ ! "$$(echo $$line | cut -c-2)" = "- " ]; then \
			echo "  $$line" ; \
		else \
			echo "$$line" ; \
		fi ; \
	done

bumpver:
	@NEWSUBVER=$$((`echo $(VERSION) | cut -d . -f 2` + 1)) ; \
	NEWVERSION=`echo $(VERSION).$$NEWSUBVER | cut -d . -f 1,3` ; \
	sed -i "s/pyparted_version = '$(VERSION)'/pyparted_version = '$$NEWVERSION'/" setup.py

install: all
	@$(PYTHON) setup.py install --root $(DESTDIR) -c -O1

clean:
	@$(PYTHON) setup.py clean
	@[ -d .git ] && git clean -d -x -f
