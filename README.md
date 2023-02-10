# nvidia-system-monitor

<div style="text-align: center;">
    <img src="icon.png" alt="icon" width="256" height="256"/>
</div>

Task Manager for Linux for Nvidia graphics cards

[![Codacy Badge](https://app.codacy.com/project/badge/Grade/5a91f69b64a7459eb4aa788172595771)](https://www.codacy.com/manual/congard/nvidia-system-monitor-qt?utm_source=github.com&utm_medium=referral&utm_content=congard/nvidia-system-monitor-qt&utm_campaign=Badge_Grade)

![preview](pics/preview.gif)

## Packages

* [Flatpak](https://flathub.org/apps/details/io.github.congard.qnvsm)
* RPM: see [releases](https://github.com/congard/nvidia-system-monitor-qt/releases)
* [AUR](https://aur.archlinux.org/packages/nvidia-system-monitor-qt/): `nvidia-system-monitor-qt`

## Dependencies

1.  Qt 5.11+
    * Fedora: `qt5-qtbase`, `qt5-qtbase-devel`
    * Arch: [`qt5-base`](https://www.archlinux.org/packages/extra/x86_64/qt5-base/)
    * Debian: [`qtdeclarative5-dev`](https://packages.debian.org/en/sid/qtdeclarative5-dev)
    * Ubuntu: [`qtdeclarative5-dev`](https://packages.ubuntu.com/focal/qtdeclarative5-dev)
2.  `nvidia-smi`
3.  `which`
4.  `cmake` (make dependence)

## Building

### Fedora

You can install rpm package from [releases](https://github.com/congard/nvidia-system-monitor-qt/releases)
<br>To launch type `qnvsm` or just click on the `.desktop` file

### ArchLinux

You can install `nvidia-system-monitor-qt` directly from [AUR](https://aur.archlinux.org/packages/nvidia-system-monitor-qt/)
<br>To launch type `qnvsm` or just click on the `.desktop` file

### Other

```bash
mkdir build
cmake -DCMAKE_BUILD_TYPE=Release -B build -G "Unix Makefiles"
cmake --build build --target qnvsm -- -j 4
```

You can specify icon path by passing CMake argument `IconPath`, for example:
```bash
mkdir build
cmake -DCMAKE_BUILD_TYPE=Release -DIconPath=/usr/share/icons/hicolor/512x512/apps/nvidia-system-monitor-qt.png -B build -G "Unix Makefiles"
cmake --build build --target qnvsm -- -j 4
```

To launch type `build/qnvsm`

The option -j describes the number of parallel processes for the build. In this case make will try to use 4 cores for the build.

## Donate

<b>Payeer:</b> P36281059
