Name:           swimtool
Summary:        Sim Wake Information Monitor
Version:        0.1.0
Release:        1
License:        BSD
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  meson
BuildRequires:  ninja
BuildRequires: pkgconfig(libgbinder-radio)
BuildRequires: pkgconfig(libgbinder)
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(libglibutil)
BuildRequires: systemd

Requires:       libgbinder-radio >= 1.5.0
Requires:       libgbinder
Requires:       systemd

%description
Sim Wake Information Monitor utility.

%prep
%setup -q

%build
%meson
meson rewrite kwargs set project / version %{version}
%meson_build

%install
%meson_install

install -d %{buildroot}%{_unitdir}
install -d %{buildroot}%{_unitdir}/multi-user.target.wants
install -d %{buildroot}%{_unitdir}/ofono.service.wants
install -d %{buildroot}%{_libexecdir}
install -m 644 -D %{name}-disable-slot2.service %{buildroot}%{_unitdir}/%{name}-disable-slot2.service
install -m 755 -D %{name}-disable-slot2.sh %{buildroot}%{_libexecdir}/%{name}-disable-slot2
ln -sf ../%{name}-disable-slot2.service %{buildroot}%{_unitdir}/multi-user.target.wants/%{name}-disable-slot2.service
ln -sf ../%{name}-disable-slot2.service %{buildroot}%{_unitdir}/ofono.service.wants/%{name}-disable-slot2.service

%post
systemctl daemon-reload || :

%preun
systemctl daemon-reload || :

%files
%{_bindir}/%{name}
%{_unitdir}/%{name}-disable-slot2.service
%{_unitdir}/multi-user.target.wants/%{name}-disable-slot2.service
%{_unitdir}/ofono.service.wants/%{name}-disable-slot2.service
%{_libexecdir}/%{name}-disable-slot2
