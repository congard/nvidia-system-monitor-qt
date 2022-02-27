#!/bin/bash

PACKAGE_ROOT='qnvsm'
DIST="$(lsb_release -i -s)$(lsb_release -r -s)"

icon="/usr/share/icons/hicolor/512x512/apps/qnvsm.png"
version_prefix="$DIST"
threads=$((($(getconf _NPROCESSORS_ONLN) / 2)))

if [ $threads -eq 0 ]; then
    threads=1
fi

cmake ../../ -DCMAKE_BUILD_TYPE=Release -DIconPath="$icon" -DVersionPrefix="$version_prefix" -B build -G "Unix Makefiles"
cmake --build build --target qnvsm -- -j $threads

mkdir -p $PACKAGE_ROOT/usr/bin
mkdir -p $PACKAGE_ROOT/usr/share/icons/hicolor/512x512/apps
mkdir -p $PACKAGE_ROOT/usr/share/applications

cp build/qnvsm $PACKAGE_ROOT/usr/bin
cp ../../icon.png $PACKAGE_ROOT/usr/share/icons/hicolor/512x512/apps/qnvsm.png
cp qnvsm.desktop $PACKAGE_ROOT/usr/share/applications

dpkg-deb --build $PACKAGE_ROOT

mv qnvsm.deb "qnvsm_$DIST.deb"
