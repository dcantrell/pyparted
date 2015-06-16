#
# filesystem.py
# Python bindings for libparted (built on top of the _ped Python module).
#
# Copyright (C) 2009-2013 Red Hat, Inc.
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
# Author(s): David Cantrell <dcantrell@redhat.com>
#            Alex Skinner <alex@lx.lc>
#

import _ped
import parted

from parted.decorators import localeC

# XXX: add docstrings!

class FileSystem(object):
    # pylint: disable=W0622
    @localeC
    def __init__(self, type=None, geometry=None, checked=False, PedFileSystem=None):
        if checked:
            c = 1
        else:
            c = 0

        if PedFileSystem is None:
            if type is None:
                raise parted.FileSystemException("no type specified")
            elif geometry is None:
                raise parted.FileSystemException("no geometry specified")

            self._type = type
            self._geometry = geometry
            self._checked = checked
            self.__fileSystem = _ped.FileSystem(type=fileSystemType[type], geom=geometry.getPedGeometry(), checked=c)
        else:
            self.__fileSystem = PedFileSystem
            self._type = self.__fileSystem.type.name
            self._geometry = parted.Geometry(PedGeometry=self.__fileSystem.geom)

            if self.__fileSystem.checked:
                self._checked = True
            else:
                self._checked = False

    def __eq__(self, other):
        return not self.__ne__(other)

    def __ne__(self, other):
        if not isinstance(self, other.__class__):
            return True

        return self.type != other.type or self.geometry != other.geometry

    def __str__(self):
        s = ("parted.FileSystem instance --\n"
             "  type: %(type)s  geometry: %(geometry)r  checked: %(checked)s\n"
             "  PedFileSystem: %(ped)r" %
             {"type": self.type, "geometry": self.geometry,
              "checked": self.checked, "ped": self.__fileSystem})
        return s

    @property
    def type(self):
        """The type of this filesystem, e.g. ext3."""
        return self._type

    @property
    def geometry(self):
        """The Geometry object describing this filesystem."""
        return self._geometry

    @property
    def checked(self):
        """True if this filesystem has been checked, False otherwise."""
        return bool(self._checked)

    def getPedFileSystem(self):
        """Return the _ped.FileSystem object contained in this FileSystem.
           For internal module use only."""
        return self.__fileSystem

# collect all filesystem types and store them in a hash
fileSystemType = {}
__type = _ped.file_system_type_get_next()
fileSystemType[__type.name] = __type

while True:
    try:
        __type = _ped.file_system_type_get_next(__type)
        fileSystemType[__type.name] = __type
    except (IndexError, TypeError, _ped.UnknownTypeException):
        break
