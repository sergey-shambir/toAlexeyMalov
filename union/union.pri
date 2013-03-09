include(union-flags.pri)

DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD
LIBS *= -L../union -lunion
LIBS *= -lSDL -lGL -lGLEW -lGLU -lopenal -lalut

CONFIG += precompile_header
PRECOMPILED_HEADER = stdafx.h
