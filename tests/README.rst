Testing pyparted
================

Pyparted test suite inherits from the standard :class:`unittest.TestCase` class.
To execute it from inside the source directory run the command::

    make test

It is also possible to generate test coverage reports using the Python coverage
tool. To do that execute::

    make coverage

To execute the Pylint code analysis tool run::

    make check

Running Pylint requires Python3 due to usage of pocket-lint.


Test Suite Architecture
------------------------

Pyparted test suite relies on several base classes listed below. All test
cases inherit from them.

- :class:`unittest.TestCase` - the standard unit test class in Python.
  Used only for base classes described below and for test cases which
  don't need access to filesystems or disks;


- :class:`~tests.baseclass.RequiresDeviceNode` - base class for any test case
  that requires a temporary device node;

- :class:`~tests.baseclass.RequiresDevice` - base class for any test case that
  requires a _ped.Device or parted.Device object first;

- :class:`~tests.baseclass.RequiresFileSystem` - base class for any test case
  that requires a filesystem on a device;

- :class:`~tests.baseclass.RequiresDeviceAlignment`- base class for certain
  alignment tests that require a _ped.Device;

- :class:`~tests.baseclass.RequiresLabeledDevice` - base class for any test
  case that requires a labeled device;

- :class:`~tests.baseclass.RequiresDisk` - base class for any test case that
  requires a _ped.Disk or parted.Disk;

- :class:`~tests.baseclass.RequiresMount` - base class for any test case that
  requires a filesystem made and mounted;

- :class:`~tests.baseclass.RequiresPartition` - base class for any test case
  that requires a _ped.Partition;

- :class:`~tests.baseclass.RequiresDiskTypes` - base class for any test case
  that requires a hash table of all _ped.DiskType objects available;

- :class:`~tests.baseclass.BuildList` - base class for any test case that
  requires a list being built via successive calls of some function. The
  function must raise IndexError when there's no more output to add to the
  return list. This class is most useful for all those _get_next methods.
  It is used in combination with other base classes;


Test scenarios are defined inside the `runTest` method of each test class using
multiple assert statements.
