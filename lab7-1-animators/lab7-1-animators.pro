ROOT = $$PWD;
include(../union/union.pri)

TEMPLATE = app
CONFIG -= qt

INSTALLS += run.sh

SOURCES += main.cpp \
    AppDelegate.cpp

HEADERS += \
    stdafx.h \
    AppDelegate.h

OTHER_FILES += \
    run.sh \
    GLSL/pipeline.vert \
    GLSL/pipeline.frag

