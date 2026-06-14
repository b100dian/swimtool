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
Requires:       libgbinder-radio >= 1.5.0
Requires:       libgbinder

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

%files
%{_bindir}/swimtool
