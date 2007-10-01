#!/bin/sh
if [ -f Makefile ]; then
    make -k distclean
fi
if [ ! -d m4 ]; then
    mkdir -p m4
fi
aclocal -I m4
#libtoolize --copy --force
autoconf
autoheader
touch config.h.in
automake --foreign --add-missing --copy
rm -rf autom4te.cache
