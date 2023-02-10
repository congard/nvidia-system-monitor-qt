# 3.10, but tested only under 3.15+
# should work with previous versions too,
# if you use one, just edit next line
cmake_minimum_required(VERSION 3.10)
project(qnvsm)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)

set(Version 1.5)

if (NOT VersionPrefix)
    set(VersionPrefix "(compiled from source)")
endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

if (NOT IconPath)
    set(IconPath "")
endif()

add_definitions(-DICON_PATH="${IconPath}")

set(QNVSM_Version "${Version} ${VersionPrefix}")
configure_file(src/resources/MainWindow_about.html src/resources/MainWindow_about.html.out)

file(READ ${CMAKE_CURRENT_BINARY_DIR}/src/resources/MainWindow_about.html.out Resources_MainWindow_About)
file(READ src/resources/MainWindow_help.html Resources_MainWindow_Help)
configure_file(src/resources/MainWindowResources.h.in src/resources/MainWindowResources.h)

file(READ src/resources/QSplitter.qss Resources_QSplitter_Stylesheet)
configure_file(src/resources/QSplitterResources.h.in src/resources/QSplitterResources.h)

include_directories(src)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/src)

set(PROJECT_SOURCES
        src/main.cpp
        src/MainWindow.cpp src/MainWindow.h
        src/SettingsDialog.cpp src/SettingsDialog.h
        src/SelectColorButton.cpp src/SelectColorButton.h
        src/resources/MainWindowResources.h.in src/resources/MainWindowResources.h
        src/resources/QSplitterResources.h.in src/resources/QSplitterResources.h
        src/core/Utils.cpp src/core/Utils.h
        src/core/Worker.cpp src/core/Worker.h
        src/core/WorkerThread.cpp src/core/WorkerThread.h
        src/core/SettingsManager.cpp src/core/SettingsManager.h
        src/processes/ProcessInfo.cpp src/processes/ProcessInfo.h
        src/processes/ProcessesView.cpp src/processes/ProcessesView.h
        src/core/Point.h
        src/utilization/CircleWidget.cpp src/utilization/CircleWidget.h
        src/utilization/UtilizationData.h
        src/utilization/UtilizationWidget.cpp src/utilization/UtilizationWidget.h
        src/utilization/UtilizationContainer.cpp src/utilization/UtilizationContainer.h
        src/utilization/gpu/GPUUtilizationWidget.cpp src/utilization/gpu/GPUUtilizationWidget.h
        src/utilization/memory/MemoryUtilizationWidget.cpp src/utilization/memory/MemoryUtilizationWidget.h
        src/utilization/gpu/GPUUtilizationContainer.cpp src/utilization/gpu/GPUUtilizationContainer.h
        src/utilization/memory/MemoryUtilizationContainer.cpp src/utilization/memory/MemoryUtilizationContainer.h
        src/core/InfoProvider.cpp src/core/InfoProvider.h
        src/utilization/Grid.h
        src/core/PCHIPInterpolator.cpp src/core/PCHIPInterpolator.h
        src/utilization/FlowLayout.cpp src/utilization/FlowLayout.h
        src/utilization/GPUDetailsWidget.cpp src/utilization/GPUDetailsWidget.h
        src/utilization/gpu/GPUUtDetailsWidget.cpp src/utilization/gpu/GPUUtDetailsWidget.h
        src/utilization/memory/GPUMemDetailsWidget.cpp src/utilization/memory/GPUMemDetailsWidget.h)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(qnvsm ${PROJECT_SOURCES})
else()
    add_executable(qnvsm ${PROJECT_SOURCES})
endif()

target_link_libraries(qnvsm PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if (FLATPAK_BUILD)
    target_compile_definitions(qnvsm PRIVATE FLATPAK_BUILD)

    install(TARGETS qnvsm DESTINATION bin)
    install(FILES icon.png DESTINATION share/icons/hicolor/512x512/apps RENAME io.github.congard.qnvsm.png)
    install(FILES package/flatpak/io.github.congard.qnvsm.desktop DESTINATION share/applications)
    install(FILES package/flatpak/io.github.congard.qnvsm.metainfo.xml DESTINATION share/metainfo)
endif()
