#
# Python bindings for libparted (built on top of the _ped Python module).
#
# Copyright The pyparted Project Authors
# SPDX-License-Identifier: GPL-2.0-or-later
#

import locale
import functools


def localeC(fn):
    # setlocale is not thread-safe, and anaconda (at least) may call this from
    # another thread.  This is just a luxury to have untranslated tracebacks,
    # so it's not worth tracebacking itself.
    def _setlocale(l):
        try:
            locale.setlocale(locale.LC_MESSAGES, l)
        # pylint: disable=bare-except
        except:
            pass

    @functools.wraps(fn)
    def new(*args, **kwds):
        oldlocale = locale.getlocale(locale.LC_MESSAGES)
        _setlocale("C")
        try:
            ret = fn(*args, **kwds)
        finally:
            _setlocale(oldlocale)
        return ret

    return new
