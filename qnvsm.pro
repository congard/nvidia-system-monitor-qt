TEMPLATE = app

QT += widgets

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/processes.cpp \
    src/worker.cpp \
    src/utils.cpp \
    src/utilization.cpp

HEADERS += \
    src/mainwindow.h \
    src/processes.h \
    src/worker.h \
    src/utils.h \
    src/constants.h \
    src/settings.h \
    src/utilization.h

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build

DESTDIR = build

DISTFILES += \
    .gitignore \
    DONATE.md \
    LICENSE \
    README.md \
    SCREENSHOTS.md
