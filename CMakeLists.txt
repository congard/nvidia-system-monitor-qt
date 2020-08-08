# 3.10, but tested only under 3.15+
# should work with previous versions too,
# if you use one, just edit next line
cmake_minimum_required(VERSION 3.10)
project(qnvsm)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 14)

set(Version 1.2)

find_package(Qt5Core)
find_package(Qt5Widgets)

add_definitions(-DQNVSM_VERSION="${Version}")

if (Distributive STREQUAL "Arch")
    add_definitions(-DICON_PATH="/usr/share/icons/hicolor/512x512/apps/nvidia-system-monitor-qt.png")
endif()

include_directories(src)

add_executable(qnvsm
        src/constants.h
        src/Settings.h
        src/main.cpp
        src/MainWindow.cpp src/MainWindow.h
        src/core/Utils.cpp src/core/Utils.h
        src/core/Worker.cpp src/core/Worker.h
        src/core/WorkerThread.cpp src/core/WorkerThread.h
        src/processes/ProcessInfo.cpp src/processes/ProcessInfo.h
        src/processes/ProcessesWorker.cpp src/processes/ProcessesWorker.h
        src/processes/ProcessesView.cpp src/processes/ProcessesView.h
        src/utilization/Point.h src/utilization/Point.cpp
        src/utilization/UtilizationData.h src/utilization/memory/MemoryData.h
        src/utilization/UtilizationWorker.cpp src/utilization/UtilizationWorker.h
        src/utilization/UtilizationWidget.cpp src/utilization/UtilizationWidget.h
        src/utilization/Graph.cpp src/utilization/Graph.h
        src/utilization/gpu/GPUUtilizationWorker.cpp src/utilization/gpu/GPUUtilizationWorker.h
        src/utilization/gpu/GPUUtilizationWidget.cpp src/utilization/gpu/GPUUtilizationWidget.h
        src/utilization/memory/MemoryUtilizationWorker.cpp src/utilization/memory/MemoryUtilizationWorker.h
        src/utilization/memory/MemoryUtilizationWidget.cpp src/utilization/memory/MemoryUtilizationWidget.h)

target_link_libraries(qnvsm ${Qt5Core_LIBRARIES} ${Qt5Widgets_LIBRARIES})
