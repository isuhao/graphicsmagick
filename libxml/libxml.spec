# Note that this is NOT a relocatable package
%define ver      2.2.3
%define prefix   /usr

Summary: libXML library
Name: libxml2
Version: %ver
Release: 1
Copyright: LGPL
Group: Development/Libraries
Source: ftp://xmlsoft.org/libxml2-%{ver}.tar.gz
BuildRoot: /var/tmp/libxml2-%{PACKAGE_VERSION}-root

URL: http://xmlsoft.org/
Docdir: %{prefix}/doc

%description
This library allows you to manipulate XML files.

%package devel
Summary: Libraries, includes, etc to develop libxml applications
Group: Development/Libraries
Requires: libxml2 = %{version}

%description devel
Libraries, include files, etc you can use to develop libxml applications.


%changelog

* Thu Sep 23 1999 Daniel Veillard <Daniel.Veillard@w3.org>

- corrected the spec file alpha stuff
- switched to version 1.7.1
- Added validation, XPath, nanohttp, removed memory leaks
- Renamed CHAR to xmlChar

* Wed Jun  2 1999 Daniel Veillard <Daniel.Veillard@w3.org>

- Switched to version 1.1: SAX extensions, better entities support, lots of
  bug fixes.

* Sun Oct  4 1998 Daniel Veillard <Daniel.Veillard@w3.org>

- Added xml-config to the package

* Thu Sep 24 1998 Michael Fulbright <msf@redhat.com>

- Built release 0.30

%prep
%setup

%build
# Needed for snapshot releases.
if [ ! -f configure ]; then
%ifarch alpha
  CFLAGS="$RPM_OPT_FLAGS" ./autogen.sh --host=alpha-redhat-linux --prefix=%prefix --sysconfdir="/etc"
%else
  CFLAGS="$RPM_OPT_FLAGS" ./autogen.sh --prefix=%prefix --sysconfdir="/etc"
%endif
else
%ifarch alpha
  CFLAGS="$RPM_OPT_FLAGS" ./configure --host=alpha-redhat-linux --prefix=%prefix --sysconfdir="/etc"
%else
  CFLAGS="$RPM_OPT_FLAGS" ./configure --prefix=%prefix --sysconfdir="/etc"
%endif
fi

if [ "$SMP" != "" ]; then
  (make "MAKE=make -k -j $SMP"; exit 0)
  make
else
  make
fi

%install
rm -rf $RPM_BUILD_ROOT

install -d $RPM_BUILD_ROOT/usr/man/man1
install -d $RPM_BUILD_ROOT/usr/man/man4
make prefix=$RPM_BUILD_ROOT%{prefix} install

%clean
rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-, root, root)

%doc AUTHORS ChangeLog NEWS README COPYING COPYING.LIB TODO
%doc /usr/man/man1/xmllint.1*
%doc /usr/man/man4/libxml.4*

%{prefix}/lib/lib*.so.*
%{prefix}/bin/xmllint

%files devel
%defattr(-, root, root)

%{prefix}/lib/lib*.so
%{prefix}/lib/*a
%{prefix}/lib/*.sh
%{prefix}/include/*
%{prefix}/bin/xml-config
