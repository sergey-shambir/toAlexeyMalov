ROOT = $$PWD;
include(../union/union.pri)

TEMPLATE = app
CONFIG -= qt
TARGET = $$redirectOutput($$TARGET)

SOURCES += main.cpp \
    AppDelegate.cpp

HEADERS += \
    stdafx.h \
    AppDelegate.h

