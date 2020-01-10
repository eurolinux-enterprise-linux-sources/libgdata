Name:           libgdata
Version:        0.17.8
Release:        1%{?dist}
Summary:        Library for the GData protocol

License:        LGPLv2+
URL:            https://wiki.gnome.org/Projects/libgdata
Source0:        https://download.gnome.org/sources/%{name}/0.17/%{name}-%{version}.tar.xz

BuildRequires:  gcr-devel
BuildRequires:  glib2-devel
BuildRequires:  gnome-online-accounts-devel
BuildRequires:  gobject-introspection-devel
BuildRequires:  gtk-doc
%if ! 0%{?rhel}
BuildRequires:  uhttpmock-devel
%endif
BuildRequires:  intltool
BuildRequires:  json-glib-devel
BuildRequires:  liboauth-devel
BuildRequires:  libsoup-devel
BuildRequires:  libxml2-devel
BuildRequires:  vala-devel
BuildRequires:  vala

%if 0%{?fedora}
Obsoletes:      compat-libgdata19 < 0.17.1
%endif

%description
libgdata is a GLib-based library for accessing online service APIs using the
GData protocol --- most notably, Google's services. It provides APIs to access
the common Google services, and has full asynchronous support.

%package        devel
Summary:        Development files for %{name}
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description    devel
The %{name}-devel package contains libraries and header files for
developing applications that use %{name}.

%prep
%setup -q

%build
%configure \
%if 0%{?rhel}
  --disable-always-build-tests \
%endif
  --disable-silent-rules \
  --disable-static
make %{?_smp_mflags} CFLAGS="$CFLAGS -fno-strict-aliasing"

%install
%make_install
find $RPM_BUILD_ROOT -name '*.la' -exec rm -f {} ';'

%find_lang gdata


%check
# Only the general test can be run without network access
# Actually, the general test doesn't work either without gconf
#cd gdata/tests
#./general

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files -f gdata.lang
%license COPYING
%doc NEWS README AUTHORS
%{_libdir}/libgdata.so.22*
%dir %{_libdir}/girepository-1.0
%{_libdir}/girepository-1.0/GData-0.0.typelib

%files devel
%{_includedir}/*
%{_libdir}/libgdata.so
%{_libdir}/pkgconfig/%{name}.pc
%{_datadir}/gtk-doc/
%dir %{_datadir}/gir-1.0
%{_datadir}/gir-1.0/GData-0.0.gir
%{_datadir}/vala/

%changelog
* Mon Apr 24 2017 Debarshi Ray <rishi@fedoraproject.org> - 0.17.8-1
- Update to 0.17.8
- Resolves: #1387002

* Wed Mar 08 2017 Debarshi Ray <rishi@fedoraproject.org> - 0.17.7-1
- Update to 0.17.7
- Resolves: #1387002, #1427087

* Wed Sep 21 2016 Kalev Lember <klember@redhat.com> - 0.17.6-1
- Update to 0.17.6
- Resolves: #1387002

* Mon May 04 2015 Debarshi Ray <rishi@fedoraproject.org> - 0.17.1-1
- Update to 0.17.1
- Resolves: #1174599

* Mon Mar 23 2015 Richard Hughes <rhughes@redhat.com> - 0.16.1-1
- Update to 0.16.1
- Resolves: #1174599

* Fri Jan 24 2014 Daniel Mach <dmach@redhat.com> - 0.13.3-4
- Mass rebuild 2014-01-24

* Fri Dec 27 2013 Daniel Mach <dmach@redhat.com> - 0.13.3-3
- Mass rebuild 2013-12-27

* Tue Jul 16 2013 Matthias Clasen <mclasen@redhat.com> - 0.13.3-2
- Disable strict aliasing, since the code is not strict-aliasing-clean

* Tue Feb 26 2013 Kalev Lember <kalevlember@gmail.com> - 0.13.3-1
- Update to 0.13.3
- Add vala bindings

* Tue Feb 19 2013 Bastien Nocera <bnocera@redhat.com> 0.13.2-3
- Co-own the gtk-doc directory (#604382)

* Thu Feb 14 2013 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.13.2-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_19_Mass_Rebuild

* Tue Oct 02 2012 Kalev Lember <kalevlember@gmail.com> - 0.13.2-1
- Update to 0.13.2

* Sun Aug 12 2012 Debarshi Ray <rishi@fedoraproject.org> - 0.13.1-2
- Add BuildRequires: gcr-devel gnome-online-accounts-devel

* Tue Jul 31 2012 Richard Hughes <hughsient@gmail.com> - 0.13.1-1
- Update to 0.13.1

* Thu Jul 19 2012 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.13.0-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_18_Mass_Rebuild

* Fri May 18 2012 Richard Hughes <hughsient@gmail.com> - 0.13.0-1
- Update to 0.13.0

* Tue Apr 17 2012 Kalev Lember <kalevlember@gmail.com> - 0.12.0-1
- Update to 0.12.0

* Thu Mar 15 2012 Matthias Clasen <mclasen@redhat.com> - 0.11.1-1
- Update to 0.11.1

* Tue Jan 17 2012 Dan Horák <dan[at]danny.cz> - 0.11.0-3
- update BR

* Fri Jan 13 2012 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.11.0-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_17_Mass_Rebuild

* Tue Dec 20 2011 Matthias Clasen <mclasen@redhat.com> - 0.11.0-1
- Update to 0.11.0

* Mon Sep 19 2011 Matthias Clasen <mclasen@redhat.com> - 0.10.1-1
- Update to 0.10.1

* Mon Jul 04 2011 Matthew Barnes <mbarnes@redhat.com> 0.9.1-1
- Update to 0.9.1

* Mon Jun 13 2011 Bastien Nocera <bnocera@redhat.com> 0.9.0-1
- Update to 0.9.0

* Fri May 20 2011 Bastien Nocera <bnocera@redhat.com> 0.8.1-1
- Update to 0.8.1

* Mon Feb 07 2011 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.8.0-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_15_Mass_Rebuild

* Mon Jan 17 2011 Bastien Nocera <bnocera@redhat.com> 0.8.0-1
- Update to 0.8.0

* Mon Oct 18 2010 Bastien Nocera <bnocera@redhat.com> 0.7.0-1
- Update to 0.7.0

* Wed Sep 29 2010 jkeating - 0.6.4-6
- Rebuilt for gcc bug 634757

* Wed Sep 22 2010 Matthias Clasen <mclasen@redhat.com> - 0.6.4-5
- Rebuild with newer gobject-introspection
- Disable tests

* Thu Jul 15 2010 Colin Walters <walters@verbum.org> - 0.6.4-4
- Rebuild with new gobject-introspection
- Drop gir-repository-devel

* Mon Jul 12 2010 Colin Walters <walters@verbum.org> - 0.6.4-2
- Rebuild against new gobject-introspection

* Thu Apr 08 2010 Bastien Nocera <bnocera@redhat.com> 0.6.4-1
- Update to 0.6.4

* Wed Feb 17 2010 Bastien Nocera <bnocera@redhat.com> 0.6.1-2
- Rebuild to update F-13 tag

* Wed Feb 17 2010 Bastien Nocera <bnocera@redhat.com> 0.6.1-1
- Update to 0.6.1

* Mon Feb 15 2010 Bastien Nocera <bnocera@redhat.com> 0.6.0-1
- Update to 0.6.0
- Add introspection support

* Sun Nov 22 2009 Bastien Nocera <bnocera@redhat.com> 0.5.1-1
- Update to 0.5.1
- Fixes queries with non-ASCII characters

* Tue Sep 22 2009 Bastien Nocera <bnocera@redhat.com> 0.5.0-1
- Update to 0.5.0

* Tue Aug 11 2009 Bastien Nocera <bnocera@redhat.com> 0.4.0-3
- Fix source URL

* Fri Jul 24 2009 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.4.0-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_12_Mass_Rebuild

* Mon Jul 20 2009 Bastien Nocera <bnocera@redhat.com> 0.4.0-1
- Update to 0.4.0

* Tue May 26 2009 Bastien Nocera <bnocera@redhat.com> 0.3.0-1
- Update to 0.3.0

* Sat Apr 25 2009 Bastien Nocera <bnocera@redhat.com> 0.2.0-1
- Update to 0.2.0

* Mon Apr 06 2009 - Bastien Nocera <bnocera@redhat.com> - 0.1.1-2
- Add check, snippet from Jason Tibbitts

* Wed Apr 01 2009 - Bastien Nocera <bnocera@redhat.com> - 0.1.1-1
- Update to 0.1.1

* Wed Apr 01 2009 - Bastien Nocera <bnocera@redhat.com> - 0.1.0-1
- First package

