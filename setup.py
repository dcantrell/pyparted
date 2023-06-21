# setup.py script for pyparted
# Copyright (C) 2011-2017 Red Hat, Inc.
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
#            Alex Skinner <alex@lx.lc>
#
# pylint: skip-file

import subprocess
import glob
import os
import platform
import sys
from setuptools import setup, Extension

pyparted_version = '3.13.0'
python_version = sys.version_info

need_libparted_version = '3.4'
need_python_version = (3, 5)

if python_version < need_python_version:
    raise RuntimeError("pyparted requires Python version %d.%d or higher"
                       % need_python_version)
# Recipe from:
# http://code.activestate.com/recipes/502261-python-distutils-pkg-config/
def pkgconfig(*packages, **kwargs):
    flag_map = {'-I': 'include_dirs', '-L': 'library_dirs', '-l': 'libraries'}
    try:
        for token in subprocess.check_output(["pkg-config", "--libs", "--cflags"] + list(packages),
                                            universal_newlines=True).split():
            kwargs.setdefault(flag_map.get(token[:2]), []).append(token[2:])
        return kwargs
    except subprocess.CalledProcessError as e:
        sys.exit("Cannot find pkg-config dependencies:\n" + e.output)

# This list is in the format necessary for the define_macros parameter
# for an Extension() module definition.  See:
# http://docs.python.org/distutils/setupscript.html#preprocessor-options
features = [('PYPARTED_VERSION', "\"%s\"" % pyparted_version),
            ('PY_SSIZE_T_CLEAN', None)]

setup(name='pyparted',
      version=pyparted_version,
      author='David Cantrell',
      author_email='dcantrell@redhat.com',
      url='https://github.com/dcantrell/pyparted/',
      description='Python bindings for GNU parted',
      license='GPLv2+',
      packages=['parted'],
      package_dir={'parted': 'src/parted'},
      ext_modules=[Extension('_ped',
                             sorted(glob.glob(os.path.join('src', '*.c'))),
                             define_macros=features,
                             **pkgconfig('libparted >= %s' % need_libparted_version,
                                         include_dirs=['include']))
                  ])
