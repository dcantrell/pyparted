#
# Python bindings for libparted (built on top of the _ped Python module).
#
# Copyright (C) 2009-2020 Red Hat, Inc.
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
# Red Hat Author(s): Chris Lumens <clumens@redhat.com>
#                    David Cantrell <dcantrell@redhat.com>
#

import sys

if sys.version_info.major >= 3 and sys.version_info.minor >= 3:
    from collections.abc import Sequence
else:
    from collections import Sequence

class CachedList(Sequence):
    """CachedList()

       Provides an immutable list that is constructed from a function that
       could take a while to run.  This is basically the same concept as
       memoization, except that the function does not take any parameters
       and therefore there is nothing to use as a memo.

       The constructor function is provided to __init__, must not take any
       parameters, and must return a list.  The invalidate() method indicates
       that the list is no longer valid and should be reconstucted by
       calling the function again.  It is up to client code to call invalidate.
       The rest of the procedure is handled by this class.

       In all ways, this should appear to be just like a list."""
    def __init__(self, lstFn):
        """Construct a new CachedList.  The lstFn is a function that takes
           no parameters and returns a list.  It will be called lazily - the
           list is not constructed until the first access, which could be
           quite a while after this method is called."""
        self._invalid = True
        self._lst = []
        self._lstFn = lstFn

    def __rebuildList(self):
        if self._invalid:
            self._lst = self._lstFn()
            self._invalid = False

    def __contains__(self, value):
        self.__rebuildList()
        return self._lst.__contains__(value)

    def __getitem__(self, index):
        self.__rebuildList()
        return self._lst.__getitem__(index)

    def __iter__(self):
        self.__rebuildList()
        return self._lst.__iter__()

    def __len__(self):
        self.__rebuildList()
        return len(self._lst)

    def __repr__(self):
        self.__rebuildList()
        return repr(self._lst)

    def __str__(self):
        self.__rebuildList()
        return str(self._lst)

    def __hash__(self):
        return hash(str(self))

    def count(self, value):
        self.__rebuildList()
        return self._lst.count(value)

    def index(self, value, *args, **kwargs):
        self.__rebuildList()
        return self._lst.index(value, *args, **kwargs)

    def invalidate(self):
        """Indicate that the list is no longer valid, due to some external
           changes.  The next access to the list will result in the provided
           list construction function being called to build a new list."""
        self._invalid = True
