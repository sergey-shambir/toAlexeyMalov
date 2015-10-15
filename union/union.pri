include(union-flags.pri)

DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD

win32 {
    LIBS *= -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lassimp
} else {
    LIBS *= -lSDL2 -lSDL2_image -lassimp
}

LIBS *= -lunion

unix:!macx {
    QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN\',-z,origin'
}

CONFIG += precompile_header
PRECOMPILED_HEADER = stdafx.h
