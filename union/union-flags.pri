
# warnings
linux-clang: QMAKE_CXXFLAGS += -Wdocumentation

win32 {
    INCLUDEPATH += $$PWD/../win32-mingw/include
    LIBS *= -L"$$PWD/../win32-mingw/lib"
    LIBS *= -lopengl32 -lGLEW32 -lGLU32 -lOpenAL32 -lDevIL
    *debug* {
        LIBS *= -L"$$PWD/../win32-mingw/debug"
    } else {
        LIBS *= -L"$$PWD/../win32-mingw/release"
    }
} else {
    LIBS *= -lGL -lGLEW -lGLU -lopenal -lIL
    INCLUDEPATH += /usr/local/include
    *debug* {
        LIBS *= -L"$$PWD/../unix-gcc/debug"
    } else {
        LIBS *= -L"$$PWD/../unix-gcc/release"
    }
}

QMAKE_CXXFLAGS += -std=c++0x -Wall -Wextra

defineReplace(redirectOutput) {
    target_dir = $$PWD
    win32 {
        *debug* {
            target_dir = ../$$relative_path($$PWD/../win32-mingw/debug/$$1, $$OUT_PWD)
        } else {
            target_dir = ../$$relative_path($$PWD/../win32-mingw/release/$$1, $$OUT_PWD)
        }
    } else {
        *debug* {
            target_dir = $$relative_path($$PWD/../unix-gcc/debug/$$1, $$OUT_PWD)
        } else {
            target_dir = $$relative_path($$PWD/../unix-gcc/release/$$1, $$OUT_PWD)
        }
    }
    return($$target_dir)
}
