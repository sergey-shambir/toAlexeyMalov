ROOT = $$PWD
include(../union/union.pri)

TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

HEADERS += \
    D21AppDelegate.h

SOURCES += \
    D21AppDelegate.cpp

HEADERS += \
    stdafx.h

