# 3.10, but tested only under 3.15+
# should work with previous versions too,
# if you use one, just edit next line
cmake_minimum_required(VERSION 3.10)
project(qnvsm)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 14)

find_package(Qt5Core)
find_package(Qt5Widgets)

if (Distributive STREQUAL "Arch")
    add_definitions(-DICON_PATH="/usr/share/icons/hicolor/512x512/apps/nvidia-system-monitor-qt.png")
endif()

include_directories(src)

add_executable(qnvsm
        src/constants.h
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        src/processes.cpp
        src/processes.h
        src/settings.h
        src/utilization.cpp
        src/utilization.h
        src/utils.cpp
        src/utils.h
        src/worker.cpp
        src/worker.h)

target_link_libraries(qnvsm ${Qt5Core_LIBRARIES} ${Qt5Widgets_LIBRARIES})