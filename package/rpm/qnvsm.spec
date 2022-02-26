Name:           qnvsm
Version:        1.5
Release:        1%{?dist}
Summary:        Task Manager for Linux for Nvidia graphics cards
Group:          Applications/System
License:        MIT
URL:            https://github.com/congard/nvidia-system-monitor-qt

%if "%{build_source}" == "github"
Source0:        https://github.com/congard/nvidia-system-monitor-qt/archive/refs/tags/%{version}.tar.gz
%else
# for test build purposes
Source0:        %{name}-%{version}.tar.gz
%endif

BuildRequires:  cmake glibc-common qt5-qtbase-devel
Requires:       xorg-x11-drv-nvidia-cuda qt5-qtbase

%description
Task Manager for Linux for Nvidia graphics cards

%prep
%autosetup

%build
mkdir build
cmake -DCMAKE_BUILD_TYPE=Release -DIconPath=%{_datadir}/icons/hicolor/512x512/apps/qnvsm.png -DVersionPrefix="%{?release}" -B build -G "Unix Makefiles"
cmake --build build --target %{name} -- -j $((($(getconf _NPROCESSORS_ONLN) / 2)))

%install
mkdir -p $RPM_BUILD_ROOT/%{_bindir}
mkdir -p $RPM_BUILD_ROOT/%{_datadir}/applications $RPM_BUILD_ROOT/%{_datadir}/icons/hicolor/512x512/apps

cp build/%{name} $RPM_BUILD_ROOT/%{_bindir}
cp package/rpm/qnvsm.desktop $RPM_BUILD_ROOT/%{_datadir}/applications/qnvsm.desktop
cp icon.png $RPM_BUILD_ROOT/%{_datadir}/icons/hicolor/512x512/apps/qnvsm.png

%files
%{_bindir}/%{name}
%{_datadir}/applications/qnvsm.desktop
%{_datadir}/icons/hicolor/512x512/apps/qnvsm.png

%changelog
* Wed Feb 23 2022 Daniel <dbcongard@gmail.com>
- qnvsm rpm package has been created
