INCLUDES = @CFLAGS@ -I@PYTHON_INC@ 
CVSV=$(subst .,_,@PYPARTED_VERSION@)
CVSR=$(shell echo $(subst .,_,@PYPARTED_RELEASE@) | sed -e 's|%.*$$||g')
CVSTAG=pyparted-$(CVSV)-$(CVSR)

pythondir = $(libdir)/python@PYTHON_VERSION@/site-packages

python_PROGRAMS = partedmodule.so

partedmodule_so_LDFLAGS = -module -shared -avoid-version -export-symbols-regex initparted
partedmodule_so_LDADD = -lparted
partedmodule_so_SOURCES = 	partedmodule.c	\
				pydevice.c	\
				pydisk.c	\
				pygeometry.c	\
				pyexception.c	\
				pyfilesystem.c	\
				pyconstraint.c

noinst_HEADERS =		partedmodule.h	\
				pydevice.h	\
				pyexception.h	\
				pygeometry.h	\
				pyconstraint.h	\
				pydisk.h	\
				pyfilesystem.h

EXTRA_DIST=pyparted.spec

tag:
	echo "going to tag as $(CVSTAG)"
	cvs tag -F $(CVSTAG)

srpm: tag dist-gzip
	rpmbuild -ts $(distdir).tar.gz
