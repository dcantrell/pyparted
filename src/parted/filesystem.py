#
# filesystem.py
# Python bindings for libparted (built on top of the _ped Python module).
#
# Copyright (C) 2009  Red Hat, Inc.
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

import _ped
import parted

# XXX: add docstrings!

class FileSystem(object):
    def __init__(self, type=None, geometry=None, checked=False, PedFileSystem=None):
        if checked:
            c = 1
        else:
            c = 0

        if PedFileSystem is None:
            if type is None:
                raise _ped.PartedException, "no type specified"
            elif geometry is None:
                raise _ped.PartedException, "no geometry specified"
            else:
                raise _ped.PartedException, "no PedFileSystem specified"

            s._type = type
            s._geometry = geometry
            s._checked = checked
            s.__fileSystem = _ped.FileSystem(type=fileSystemType[type], geom=geometry, checked=c)
        else:
            s.__fileSystem = PedFileSystem
            s._type = s.__fileSystem.type.name
            s._geometry = s.__fileSystem.geom

            if s.__fileSystem.checked:
                s._checked = True
            else:
                s._checked = False

    def __readOnly(self, property):
        raise parted.ReadOnlyProperty, property

    type = property(lambda s: s.__type, lambda s, v: s.__readOnly("type"))
    geometry = property(lambda s: s.__geometry, lambda s, v: s.__readOnly("geometry"))
    checked = property(lambda s: s.__checked, lambda s, v: s.__readOnly("checked"))

    def clobber(self):
        return self.__fileSystem.clobber()

    def open(self):
        return self.__fileSystem.open()

    # XXX: this can take in a Timer
    def create(self):
        return self.__fileSystem.create()

    def close(self):
        return self.__fileSystem.close()

    # XXX: this can take in a Timer
    def check(self):
        return self.__fileSystem.check()
        self._checked = self.__fileSystem.checked

    # XXX: this can take in a Timer
    def copy(self, geometry):
        return self.__fileSystem.copy(geometry)

    # XXX: this can take in a Timer
    def resize(self, geometry):
        return self.__fileSystem.resize(geometry)

    def getResizeConstraint(self):
        return self.__fileSystem.get_resize_constraint()

# collect all filesystem types and store them in a hash
fileSystemType = {}
__type = _ped.file_system_type_get_next()
fileSystemType[__type.name] = __type

while True:
    try:
        __type = _ped.file_system_type_get_next(__type)
        fileSystemType[__type.name] = __type
    except:
        break
