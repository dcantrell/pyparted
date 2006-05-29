Summary: python module for parted
Name: pyparted
Version: 1.7.1
Release: 1
License: GPL
Group: System Environment/Libraries
Source0: %{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root
Requires: parted >= 1.7.0
BuildRequires: parted-devel >= 1.7.1, python-devel
Requires: python-abi = %(%{__python} -c "import sys; print sys.version[:3]")

%description
Python modules for the parted library.  It is used for manipulation
partition tables.

%prep
%setup -q

%build
export CFLAGS="-fPIC -O2 -g2"
if [ ! -x configure ]; then
	./autogen.sh --with-python-version=2.4
fi
%configure --with-python-version=2.4
make

%install
rm -rf $RPM_BUILD_ROOT
%makeinstall

%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
%doc README COPYING
%dir %{_libdir}/python?.?/site-packages/
%{_libdir}/python?.?/site-packages/*.so


%changelog
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
