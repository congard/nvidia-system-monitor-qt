from file import *

package = "qnvsm"
version = "1.3"
revision = "1"
architecture = "amd64"
maintainer = "Daniel Kongarden <dbcongard@gmail.com>"
description = "Task Manager for Linux for Nvidia graphics cards"
dependencies = "nvidia-smi, qtdeclarative5-dev"

files = {
    "build/qnvsm": file("/usr/bin/qnvsm", "executable", 755),
    "../../icon.png": file("/usr/share/icons/hicolor/512x512/apps/qnvsm.png", "icon", 644),
    "qnvsm.desktop": file("/usr/share/applications/qnvsm.desktop", permission=644)
}

# qt = "/mnt/memory0/Development/Qt/linux/6.0.2/gcc_64"
qt = "" # means "system"
