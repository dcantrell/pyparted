/*
 * pydisk.h
 * pyparted docstrings for pydisk.c
 *
 * Copyright (C) 2007, 2008, 2009  Red Hat, Inc.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions of
 * the GNU General Public License v.2, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY expressed or implied, including the implied warranties of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.  You should have received a copy of the
 * GNU General Public License along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.  Any Red Hat trademarks that are incorporated in the
 * source code or documentation are not subject to the GNU General Public
 * License and may only be used or replicated with the express permission of
 * Red Hat, Inc.
 *
 * Red Hat Author(s): David Cantrell <dcantrell@redhat.com>
 *                    Chris Lumens <clumens@redhat.com>
 */

#ifndef DOCSTRINGS_PYDISK_H_INCLUDED
#define DOCSTRINGS_PYDISK_H_INCLUDED

#include <Python.h>

PyDoc_STRVAR(partition_destroy_doc,
"destroy(self) -> None\n\n"
"Destroys the Partition object.");

PyDoc_STRVAR(partition_is_active_doc,
"is_active(self) -> boolean\n\n"
"Return whether self is active or not.");

PyDoc_STRVAR(partition_set_flag_doc,
"set_flag(self, flag, state) -> boolean\n\n"
"Sets the state of the given flag on self .  Flags have different types of\n"
"different types of disk labels, and are not guaranteed to exist on all disk\n"
"label types.  If provided with an invalid flag for the disk's label,\n"
"_ped.PartitionException is raised.");

PyDoc_STRVAR(partition_get_flag_doc,
"get_flag(self, flag) -> integer\n\n"
"Return the state of the given flag on self.  There is no check for invalid\n"
"flag types, so these will always return 0.  It is therefore recommended to\n"
"call self.is_flag_available() first to make sure.");

PyDoc_STRVAR(partition_is_flag_available_doc,
"is_flag_available(self, flag) -> boolean\n\n"
"Return whether the given flag is valid for self.");

PyDoc_STRVAR(partition_set_system_doc,
"set_system(self, FileSystemType) -> boolean\n\n"
"Set the system type on self to FileSystemType.  On error,\n"
"_ped.PartitionException is raised.");

PyDoc_STRVAR(partition_set_name_doc,
"set_name(self, string) -> boolean\n\n"
"On disk labels that support it, this method sets the partition's name.\n"
"Before attempting this operation, DiskType.check_feature() can be used to\n"
"determine if it is even supported.  On error, _ped.PartitionException will\n"
"be raised.");

PyDoc_STRVAR(partition_get_name_doc,
"get_name(self) -> string\n\n"
"On disk labels that support it, this method returns the partition's name.  On\n"
"all other disk labels, _ped.PartitionException will be raised.  Before calling\n"
"this method, DiskType.check_feature() can be called to check for support.");

PyDoc_STRVAR(partition_is_busy_doc,
"is_busy(self) -> boolean\n\n"
"Return whether self is busy or not.  The most likely reason for a partition\n"
"to be busy is because it's mounted.  Additionally, extended partitions are\n"
"busy if any of their logical partitions are busy.");

PyDoc_STRVAR(partition_get_path_doc,
"get_path(self) -> string\n\n"
"Return a path that could be used for addressing self at an operating system\n"
"level.  For instance, on Linux this could return '/dev/sda' for a partition.\n"
"If an error occurs, _ped.PartitionException is raised.");

PyDoc_STRVAR(partition_reset_num_doc,
"reset_num(self) -> boolean\n\n"
"Reset the partition's number to value allowing it to be set correctly when\n"
"the partition is added to _ped.PartedDisk. The returned value means\n"
"success/failure");

PyDoc_STRVAR(disk_duplicate_doc,
"duplicate(self) -> Disk\n\n"
"Return a new Disk that is a copy of self.  This method raises\n"
"_ped.DiskException if there is an error making the copy.");

PyDoc_STRVAR(disk_destroy_doc,
"destroy(self) -> None\n\n"
"Destroy the Disk object.");

PyDoc_STRVAR(disk_commit_doc,
"commit(self) -> boolean\n\n"
"Write the in-memory changes to the disk's partition table and inform the\n"
"operating system of the changes.  This method is equivalent to calling:\n"
"\tself.disk_commit_to_dev()\n"
"\tself.disk_commit_to_os()\n"
"On error, _ped.DiskException is raised.");

PyDoc_STRVAR(disk_commit_to_dev_doc,
"commit_to_dev(self) -> boolean\n\n"
"Write the in-memory changes to the disk's partition table.  On error,\n"
"_ped.DiskException is raised.");

PyDoc_STRVAR(disk_commit_to_os_doc,
"commit_to_os(self) -> boolean\n\n"
"Inform the operating system that disk's partition table layout has changed.\n"
"What exactly this means depends on the operating system.  On error, a\n"
"_ped.DiskException is raised.");

PyDoc_STRVAR(disk_check_doc,
"check(self) -> boolean\n\n"
"Perform a basic sanity check on the partition table.  This check does not\n"
"depend on the type of disk.  If there is an error performing the check,\n"
"_ped.DiskException is raised.");

PyDoc_STRVAR(disk_print_doc,
"print(self) -> None\n\n"
"Print a summary of the partitions on self.");

PyDoc_STRVAR(disk_get_primary_partition_count_doc,
"get_primary_partition_count(self) -> integer\n\n"
"Return the number of primary partitions on self.");

PyDoc_STRVAR(disk_get_last_partition_num_doc,
"get_last_partition_num(self) -> integer\n\n"
"Return the highest in-use partition number on self.");

PyDoc_STRVAR(disk_get_max_primary_partition_count_doc,
"get_max_primary_partition_count(self) -> integer\n\n"
"Get the maximum number of primary partitions supported by the disk label.");

PyDoc_STRVAR(disk_get_max_supported_partition_count_doc,
"get_max_supported_partition_count(self) -> integer\n\n"
"Get the highest supported partition number of this disk.");

PyDoc_STRVAR(disk_get_partition_alignment_doc,
"get_partition_alignment(self) -> Alignment\n\n"
"Get the alignment needed for partition boundaries on this disk.\n"
"The returned alignment describes the alignment for the start sector\n"
"of the partition, for all disklabel types which require alignment,\n"
"except Sun disklabels, the end sector must be aligned too.\n"
"To get the end sector alignment decrease the PedAlignment offset by 1.\n");

PyDoc_STRVAR(disk_max_partition_length_doc,
"max_partition_length(self) -> long\n\n"
"This returns the maximum length for a partition the label on this disk\n"
"can represent. This does not necessarily mean that there is enough\n"
"freespace to create such a partition.\n"
"If this information is not available 0 is returned");

PyDoc_STRVAR(disk_max_partition_start_sector_doc,
"max_partition_start_sector(self) -> long\n\n"
"This returns the maximum partition start sector the label on this disk\n"
"can represent.\n"
"If this information is not available 0 is returned");

PyDoc_STRVAR(disk_set_flag_doc,
"set_flag(self, flag, state) -> boolean\n\n"
"Sets the state of the given flag on self .\n"
"If provided with an invalid flag for the disk's label,\n"
"a PartedException is raised.");

PyDoc_STRVAR(disk_get_flag_doc,
"get_flag(self, flag) -> boolean\n\n"
"Return the state of the given flag on self.  There is no check for invalid\n"
"flag types, so these will always return 0.  It is therefore recommended to\n"
"call self.is_flag_available() first to make sure.");

PyDoc_STRVAR(disk_is_flag_available_doc,
"is_flag_available(self, flag) -> boolean\n\n"
"Return whether the given flag is valid for self.");

PyDoc_STRVAR(disk_add_partition_doc,
"add_partition(self, Partition, Constraint) -> boolean\n\n"
"Adds the new partition Partition to self.  This operation may modify the\n"
"partition's geometry, subject to Constraint.  Having a strict Constraint\n"
"will likely cause this operation to fail, raising a _ped.PartitionException\n"
"in the process.");

PyDoc_STRVAR(disk_remove_partition_doc,
"remove_partition(self, Partition) -> boolean\n\n"
"Remove Partition from self.  If Partition is an extended partition, it must\n"
"not contain any logical partitions.  The Partition object itself is not\n"
"destroyed.  The caller must use Partition.destroy() or self.delete_partition().\n"
"For all error cases, _ped.PartitionException will be raised.");

PyDoc_STRVAR(disk_delete_partition_doc,
"delete_partition(self, Partition) -> boolean\n\n"
"Remove Partition from self and destroy the Partition object afterwards.  This\n"
"is equivalent to calling:\n"
"\tself.remove_partition(Partition)\n"
"\tPartition.destroy()\n"
"For all error cases, _ped.PartitionException will be raised.");

PyDoc_STRVAR(disk_delete_all_doc,
"disk_delete_all(self) -> boolean\n\n"
"Remove and destroy all partitions on self, raising _ped.PartitionException on\n"
"any error case.");

PyDoc_STRVAR(disk_set_partition_geom_doc,
"set_partition_geom(self, Partition, Constraint, start_sector, end_sector) ->\n"
"                  boolean\n\n"
"Change the location of Partition by setting a new Geometry on it, subject to\n"
"the restrictions of Constraint.  This operation can fail for many reasons,\n"
"all of which result in a _ped.PartitionException.  One of the most likely\n"
"failure cases is that the new location overlaps with an existing partition.\n"
"On error, Partition will be unchanged.  On success, the contents of the\n"
"partition will still not be changed - the file system itself will still\n"
"need to be resized.");

PyDoc_STRVAR(disk_maximize_partition_doc,
"maximize_partition(self, Partition, Constraint) -> boolean\n\n"
"Grow the Partition to the largest possibly size, subject to the restrictions\n"
"of Constraint.  Raise _ped.PartitionException on error.");

PyDoc_STRVAR(disk_get_max_partition_geometry_doc,
"get_max_partition_geometry(self, Partition, Constraint) -> Geometry\n\n"
"Return the maximum Geometry that Partition can be grown to, subject to the\n"
"restrictions of Constraint.  Raise _ped.PartitionException on error.");

PyDoc_STRVAR(disk_minimize_extended_partition_doc,
"minimize_extended_partition(self) -> boolean\n\n"
"Reduce the size of an extended partition on self to the minimum while still\n"
"including all logical partitions.  If there are no logical partitions, the\n"
"extended partition will be deleted.  If the extended partition cannot be\n"
"shrunk, a _ped.PartitionException will be raised.");

PyDoc_STRVAR(disk_next_partition_doc,
"next_partition(self, Partition) -> Partition\n\n"
"Return the next partition on self after Partition.  If Partition is None,\n"
"return the first partition.  If Partition is an extended partition, return\n"
"the first logical partition inside it.  If Partition is the last partition,\n"
"raise IndexError.  Repeatedly calling this method has the effect of\n"
"performing a depth-first traversal on self.");

PyDoc_STRVAR(disk_get_partition_doc,
"get_partition(self, num) -> Partition\n\n"
"Return the Partition given by num, or raise _ped.PartitionException if no\n"
"partition with that index exists.");

PyDoc_STRVAR(disk_get_partition_by_sector_doc,
"get_partition_by_sector(self, sector) -> Partition\n\n"
"Return the Partition containing sector, or raise _ped.PartitionException\n"
"otherwise.  If sector exists within a logical partition, the logical\n"
"partition is returned.");

PyDoc_STRVAR(disk_extended_partition_doc,
"extended_partition(self) -> Partition\n\n"
"If an extended partition exists on self, return it.  Otherwise, raise\n"
"_ped.PartitionException");

PyDoc_STRVAR(disk_type_check_feature_doc,
"check_feature(self, DiskTypeFeature) -> boolean\n\n"
"Return whether or not self supports a particular partition table feature.\n"
"DiskTypeFeatures are given by the _ped.DISK_TYPE_* constants.");

PyDoc_STRVAR(_ped_Partition_doc,
"A _ped.Partition object describes a single partition on a disk.  Operations\n"
"on Partition objects are limited to getting and setting flags, names, and\n"
"paths.  All other operations you may wish to do involving partitions are\n"
"done through a _ped.Disk or _ped.FileSystem object.  These objects all exist\n"
"as attributes of a Partition, though.\n\n"
"Valid flags for Partitions are given by the _ped.PARTITION_* constants,\n"
"though not all flags are valid for every disk label type.\n\n"
"For most errors involving a Partition object, _ped.PartitionException will\n"
"be raised.");

PyDoc_STRVAR(_ped_Disk_doc,
"A _ped.Disk object represents a disk label, or partition table, on a single\n"
"_ped.Device.  Since parted supports a variety of platforms, it must also\n"
"support a variety of disk labels, not all of which may support the same set\n"
"of features.  For instance, DOS disk labels support extended partitions while\n"
"other systems do not.  The Disk object therefore includes a DiskType\n"
"reference to enumerate supported features.  However, all other Disk operations\n"
"are supported on all disk label types.\n\n"
"Operations on Disk objects include creating, deleting, moving, and resizing\n"
"partitions in various ways.  Creating filesystems within these partitions is\n"
"left up to the FileSystem objects.\n\n"
"For most errors involving a Disk object, _ped.PartitionException will be\n"
"raised.  Some operations can also raise _ped.IOException or IndexError.");

PyDoc_STRVAR(_ped_DiskType_doc,
"A _ped.DiskType object is a simple object that gives a partition table a\n"
"name and describes features it supports.  A reference to one of these\n"
"objects is stored inside a _ped.Disk object.");

#endif /* PYDISK_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4
 */
