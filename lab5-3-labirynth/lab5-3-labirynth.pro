ROOT = $$PWD
include(../union/union.pri)

TEMPLATE = app
CONFIG -= qt
TARGET = $$redirectOutput($$TARGET)

SOURCES += main.cpp \
    appdelegate.cpp \
    maze.cpp \
    mazeloader.cpp

HEADERS += \
    appdelegate.h \
    stdafx.h \
    maze.h \
    mazeloader.h

