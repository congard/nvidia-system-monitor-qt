Name:           qnvsm
Version:        1.5
Release:        1%{?dist}
Summary:        Task Manager for Linux for Nvidia graphics cards
Group:          Applications/System
License:        MIT
URL:            https://github.com/congard/nvidia-system-monitor-qt

BuildRequires:  cmake glibc-common qt5-qtbase-devel
Requires:       xorg-x11-drv-nvidia-cuda qt5-qtbase

%description
Task Manager for Linux for Nvidia graphics cards

%prep
%if "%{build_source}" == "github"
mkdir -p %{_sourcedir}
cd %{_sourcedir}
wget https://github.com/congard/nvidia-system-monitor-qt/archive/refs/tags/v%{version}.tar.gz
%endif

cd %{_builddir}
tar -xvzf %{_sourcedir}/v%{version}.tar.gz

%build
cd %{_builddir}/nvidia-system-monitor-qt-%{version}

mkdir -p build
cmake -DCMAKE_BUILD_TYPE=Release -DIconPath=%{_datadir}/icons/hicolor/512x512/apps/qnvsm.png -DVersionPrefix="%{?release}" -B build -G "Unix Makefiles"
cmake --build build --target %{name} -- -j $((($(getconf _NPROCESSORS_ONLN) / 2)))

%install
mkdir -p $RPM_BUILD_ROOT/%{_bindir}
mkdir -p $RPM_BUILD_ROOT/%{_datadir}/applications $RPM_BUILD_ROOT/%{_datadir}/icons/hicolor/512x512/apps

cd %{_builddir}/nvidia-system-monitor-qt-%{version}

cp build/%{name} $RPM_BUILD_ROOT/%{_bindir}
cp package/rpm/qnvsm.desktop $RPM_BUILD_ROOT/%{_datadir}/applications/qnvsm.desktop
cp icon.png $RPM_BUILD_ROOT/%{_datadir}/icons/hicolor/512x512/apps/qnvsm.png

%files
%{_bindir}/%{name}
%{_datadir}/applications/qnvsm.desktop
%{_datadir}/icons/hicolor/512x512/apps/qnvsm.png

%clean
rm -rf %{_builddir}
rm -rf %{_buildrootdir}

%changelog
* Wed Feb 23 2022 Daniel <dbcongard@gmail.com>
- qnvsm rpm package has been created
