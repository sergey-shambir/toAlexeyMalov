include(union-flags.pri)

DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD

win32 {
    LIBS *= -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lassimp
    debug {
        LIBS *= -L$$OUT_PWD/../union/debug -lunion
    } else {
        LIBS *= -L$$OUT_PWD/../union/release -lunion
    }
} else {
    LIBS *= -lSDL2 -lSDL2_image -lassimp
    LIBS *= -L../union -lunion
}

CONFIG += precompile_header
PRECOMPILED_HEADER = stdafx.h
