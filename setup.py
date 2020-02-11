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
from distutils.ccompiler import new_compiler
from distutils.errors import CompileError
from distutils.errors import LinkError
from distutils.core import setup
from distutils.core import Extension
from distutils.version import LooseVersion

pyparted_version = '3.11.4'
python_version = sys.version_info

need_libparted_version = '2.3'
need_python_version = (2, 7)

if python_version < need_python_version:
    raise RuntimeError("pyparted requires Python version %d.%d or higher"
                       % need_python_version)
# Recipe from:
# http://code.activestate.com/recipes/502261-python-distutils-pkg-config/
def pkgconfig(*packages, **kwargs):
    flag_map = {'-I': 'include_dirs', '-L': 'library_dirs', '-l': 'libraries'}
    for token in subprocess.check_output(["pkg-config", "--libs", "--cflags"] + list(packages)).decode('utf-8').split():

        kwargs.setdefault(flag_map.get(token[:2]), []).append(token[2:])
    return kwargs

def check_mod_version(module, version):
    modversion = subprocess.check_output(["pkg-config", "--modversion", module]).decode('utf-8').split()[0]
    if not LooseVersion(modversion) >= LooseVersion(version):
        sys.stderr.write("*** Minimum required %s version: %s, found: %s\n" % (module, version, modversion,))
        sys.exit(1)
    return

check_mod_version('libparted', need_libparted_version)

# This list is in the format necessary for the define_macros parameter
# for an Extension() module definition.  See:
# http://docs.python.org/distutils/setupscript.html#preprocessor-options
features = [('PYPARTED_VERSION', "\"%s\"" % pyparted_version)]

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
                             **pkgconfig('libparted',
                                         include_dirs=['include']))
                  ])
