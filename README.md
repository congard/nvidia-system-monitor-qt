# nvidia-system-monitor
<center>
    <img src="icon.png" alt="icon" width="256" height="256"/>
</center>

Task Manager for Linux for Nvidia graphics cards

# Dependencies
Qt is required for work. If you are an ArchLinux user, you can install from [here](https://wiki.archlinux.org/index.php/qt#Installation)
<br>Also in the system must be available `nvidia-smi`

# Building
```
qmake qnvsm.pro
make
```
To launch type `build/qnvsm`

# Config
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

# Screenshots
[Open SCREENSHOTS.md](SCREENSHOTS.md)

# Donate
[Open DONATE.md](DONATE.md)
