#!/usr/bin/python3

import sys

from pocketlint import PocketLintConfig, PocketLinter

class PypartedLintConfig(PocketLintConfig):
    @property
    def extraArgs(self):
        return ["--extension-pkg-whitelist", "_ped"]

    @property
    def pylintPlugins(self):
        retval = super(PypartedLintConfig, self).pylintPlugins
        retval.remove("pocketlint.checkers.eintr")
        return retval

if __name__ == "__main__":
    conf = PypartedLintConfig()
    linter = PocketLinter(conf)
    rc = linter.run()
    sys.exit(rc)
