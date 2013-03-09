ROOT = $$PWD
include(../union/union.pri)

TEMPLATE = app
CONFIG -= qt

SOURCES += main.cpp \
    AppDelegate.cpp

HEADERS += \
    AppDelegate.h \
    stdafx.h

