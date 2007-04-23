%{!?python_sitearch: %define python_sitearch %(%{__python} -c "from distutils.sysconfig import get_python_lib; print get_python_lib(1)")}

Summary: Python module for GNU parted
Name:    pyparted
Version: 1.8.6
Release: 2%{?dist}
License: GPL
Group:   System Environment/Libraries
URL:     http://people.redhat.com/dcantrel/%{name}

Source0: http://people.redhat.com/dcantrel/%{name}/%{name}-%{version}.tar.bz2

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
BuildRequires: python-devel, parted-devel >= 1.8.6
Requires: parted >= 1.8.6

%description
Python modules for the parted library.  It is used for manipulation
partition tables.

%prep
%setup -q

%build
CFLAGS="%{optflags}" CC="%{__cc}" %{__make} %{?_smp_mflags}

%install
%{__make} install DESTDIR=%{buildroot}

%clean
%{__rm} -rf %{buildroot}

%files
%defattr(-,root,root,-)
%doc README COPYING AUTHORS ChangeLog
%{python_sitearch}/partedmodule.so

%changelog
* Mon Apr 23 2007 David Cantrell <dcantrell@redhat.com> - 1.8.6-2
- Ensure build env CFLAGS are included (#226337)

* Fri Apr 19 2007 David Cantrell <dcantrell@redhat.com> - 1.8.6-1
- Merge review (#226337)

* Tue Mar 20 2007 David Cantrell <dcantrell@redhat.com> - 1.8.5-4
- Rebuild for GNU parted-1.8.6

* Tue Mar 20 2007 David Cantrell <dcantrell@redhat.com> - 1.8.5-3
- Rebuild for GNU parted-1.8.5

* Mon Mar 19 2007 David Cantrell <dcantrell@redhat.com> - 1.8.5-2
- Rebuild for GNU parted-1.8.4

* Thu Feb 08 2007 David Cantrell <dcantrell@redhat.com> - 1.8.5-1
- Define and use python_sitearch rather than python_sitelib

* Thu Feb 08 2007 David Cantrell <dcantrell@redhat.com> - 1.8.4-1
- Use preferred BuildRoot (package review)
- Define and use python_sitelib macro (package review)

* Fri Jan 12 2007 David Cantrell <dcantrell@redhat.com> - 1.8.3-1
- Required parted-1.8.2 or higher

* Wed Jan 10 2007 Jeremy Katz <katzj@redhat.com> - 1.8.2-1
- use PyObject_DEL instead of PyMem_DEL

* Thu Dec  7 2006 Jeremy Katz <katzj@redhat.com> - 1.8.1-3
- rebuild for python 2.5

* Tue Dec 05 2006 David Cantrell <dcantrell@redhat.com> - 1.8.1-2
- Rebuild for GNU parted-1.8.1

* Thu Nov 30 2006 David Cantrell <dcantrell@redhat.com> - 1.8.1-1
- Determine Python version to use in %%build so the source RPM is more
  easily moved between distribution releases.

* Fri Nov 17 2006 David Cantrell <dcantrell@redhat.com> - 1.8.0-1
- Bump version to 1.8.0 and require parted >= 1.8.0
- Remove python-abi Requires line since rpm handles that automatically

* Wed Aug 30 2006 David Cantrell <dcantrell@redhat.com> - 1.7.3-1
- Include parted/constraint.h in required header files

* Wed Aug 30 2006 David Cantrell <dcantrell@redhat.com> - 1.7.2-2
- Require parted-1.7.1 or higher

* Tue Jul 25 2006 David Cantrell <dcantrell@redhat.com> - 1.7.2-1
- Add HPSERVICE, PALO, PREP, and MSFT_RESERVED to partition types list

* Wed Jul 12 2006 Jesse Keating <jkeating@redhat.com> - 1.7.1-1.1
- rebuild

* Sun May 28 2006 David Cantrell <dcantrell@redhat.com> - 1.7.1-1
- Bump version to 1.7.1 and require parted >= 1.7.1

* Fri May 19 2006 David Cantrell <dcantrell@redhat.com> - 1.7.0-1
- Bump version to 1.7.0 and require parted >= 1.7.0

* Fri Dec 09 2005 Jesse Keating <jkeating@redhat.com>
- rebuilt

* Fri Nov 11 2005 Peter Jones <pjones@redhat.com> - 1.6.10-1
- rebuild for new parted.
- add debugging options for make so debuginfo isn't useless

* Wed Nov  9 2005 Jeremy Katz <katzj@redhat.com> - 1.6.9-5
- rebuild for new parted

* Wed Aug 31 2005 Chris Lumens <clumens@redhat.com> 1.6.9-4
- Rebuilt for new parted library.

* Wed Mar 16 2005 Chris Lumens <clumens@redhat.com> 1.6.9-3
- Updated for gcc4 and python2.4.  Fixed build warnings.

* Tue Dec 14 2004 Jeremy Katz <katzj@redhat.com> - 1.6.9-2
- add support for sx8 devices

* Mon Nov  8 2004 Jeremy Katz <katzj@redhat.com> - 1.6.8-3
- rebuild for python 2.4

* Mon Oct 11 2004 Warren Togami <wtogami@redhat.com> - 1.6.8-2
- #135100 req python-abi (Robert Scheck)

* Tue Aug 17 2004 Jeremy Katz <katzj@redhat.com> - 1.6.8-1
- update for new parted ABI
  - device -> heads, sectors, cylinders now refer to the bios geometry
- require parted >= 1.6.12

* Thu Jul 22 2004 Jeremy Katz <katzj@redhat.com> - 1.6.7-3
- build on ppc64 again

* Thu May 13 2004 Jeremy Katz <katzj@redhat.com> - 1.6.7-1
- fix build for newer versions of gcc (fix from Jeff Law)

* Tue Mar 16 2004 Jeremy Katz <katzj@redhat.com> 1.6.6-2
- fix PARTITION_PROTECTED definition (#118451)

* Fri Mar 12 2004 Jeremy Katz <katzj@redhat.com>
- Initial build split out into separate source from the parted package.
- Don't build on ppc64 right now due to parted build problems (#118183)
