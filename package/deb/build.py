from config import *
from os.path import join
from shutil import copyfile
import os

package_root = "{}_{}-{}_{}".format(package, version, revision, architecture)
debian_dir = join(package_root, "DEBIAN")

os.makedirs(package_root, exist_ok=True)
os.makedirs(debian_dir, exist_ok=True)

print("# Creating a control file")

control_file = open(join(debian_dir, "control"), "w")
control_file.write("""Package: {}
Version: {}
Architecture: {}
Maintainer: {}
Description: {}
Depends: {}
""".format(package, version, architecture, maintainer, description, dependencies))
control_file.flush()
control_file.close()

print("# Creating a desktop file")

for src in files:
    dst = files[src]

    if dst.has_description():
        desc = dst.get_description()

        if desc == "executable":
            executable_dst = dst.get_dir()
        elif desc == "icon":
            icon_dst = dst.get_dir()

desktop_file_in = open("qnvsm.desktop.in", "r")
desktop_in = desktop_file_in.read()
desktop_file_in.close()

desktop_file = open("qnvsm.desktop", "w")
desktop = desktop_file.write(desktop_in.format(executable=executable_dst, icon=icon_dst))
desktop_file.flush()
desktop_file.close()

print("# Building")

if qt:
    cmake_prefix = "-DCMAKE_PREFIX_PATH='{}'".format(qt)
else:
    cmake_prefix = ""

version_prefix = "{}_deb".format(package_root)

os.system("""cmake ../../ {prefix} -DCMAKE_BUILD_TYPE=Release -DIconPath="{icon}" -DVersionPrefix="{version_prefix}" -B build -G "Unix Makefiles"
cmake --build build --target qnvsm -- -j 4""".format(prefix=cmake_prefix, icon=icon_dst, version_prefix=version_prefix))

print("# Copying files")

for src in files:
    dst_file = files[src]
    dst_file_dir = dst_file.get_dir() # destination in the system
    dst = package_root + dst_file_dir

    os.makedirs(os.path.dirname(dst), exist_ok=True)

    copyfile(src, dst)

    if (dst_file.has_permission()):
        os.chmod(dst, dst_file.get_permission())

print("# Building package")

os.system("dpkg-deb --build --root-owner-group {}".format(package_root))

print("# Package {}.deb was successfully built".format(package_root))
