# nvidia-system-monitor
<div style="text-align: center;">
    <img src="icon.png" alt="icon" width="256" height="256"/>
</div>

Task Manager for Linux for Nvidia graphics cards

[![Codacy Badge](https://app.codacy.com/project/badge/Grade/5a91f69b64a7459eb4aa788172595771)](https://www.codacy.com/manual/congard/nvidia-system-monitor-qt?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=congard/nvidia-system-monitor-qt&amp;utm_campaign=Badge_Grade)

## Dependencies
1. Qt 5.11+
   * Arch: [`qt5-base`](https://www.archlinux.org/packages/extra/x86_64/qt5-base/)
   * Debian: [`qtdeclarative5-dev`](https://packages.debian.org/en/sid/qtdeclarative5-dev)
   * Ubuntu: [`qtdeclarative5-dev`](https://packages.ubuntu.com/focal/qtdeclarative5-dev)
2. `nvidia-smi`
3. `which`
4. `cmake` (make dependence)

## Building
**Note:** after a lot of refactoring you may be bumped into compilation errors because of missing headers.
<br>If so, create new issue or write me email with compilation error: [dbcongard@gmail.com](mailto:dbcongard@gmail.com)

### ArchLinux
You can install `nvidia-system-monitor-qt` directly from [AUR](https://aur.archlinux.org/packages/nvidia-system-monitor-qt/)
<br>To launch enter `qnvsm` or just click on `.desktop` file

#### From source
The only difference from **Other** commands is that it will try to load the icon located in 
`/usr/share/icons/hicolor/512x512/apps/nvidia-system-monitor-qt.png`

```bash
mkdir build
cmake -DCMAKE_BUILD_TYPE=Release -B build -DDistributive=Arch -G "Unix Makefiles"
cmake --build build --target qnvsm -- -j 2
```

### Other
```bash
cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" -B cmake-build-release
cd cmake-build-release
make -j4
```
To launch type `cmake-build-release/qnvsm`

The option -j describes the number of parallel processes for the build. In this case make will try to use 4 cores for the build.

If you want to use an IDE for Linux you can try CLion or QtCreator.

## Config
Here example of simple config located in `~/.config/nvidia-system-monitor/config`:
```
# time in ms
updateDelay 500
graphLength 120000

#           gpu id  red  green  blue
gpuColor    0       0    0      255
gpuColor    1       0    255    0
gpuColor    2       255  0      0
```

## Screenshots
[Open SCREENSHOTS.md](SCREENSHOTS.md)

## Donate
<b>Payeer:</b> P36281059

Or you can do it even without finance. Just open [this](http://fainbory.com/8aWY) link

Thank you.
