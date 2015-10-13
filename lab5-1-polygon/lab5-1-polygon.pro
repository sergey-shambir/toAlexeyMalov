ROOT = $$PWD
include(../union/union.pri)

TEMPLATE = app
CONFIG -= qt
TARGET = $$redirectOutput($$TARGET)

HEADERS += \
    stdafx.h \
    AppDelegate.h

SOURCES += \
    main.cpp \
    AppDelegate.cpp
