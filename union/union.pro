#-------------------------------------------------
#
#  Union - small library for OpenGL applications
#
#-------------------------------------------------

include(union-flags.pri)
LIBS *= -lSDL2 -lSDL2_image -lassimp

QT       -= core gui

TARGET = $$redirectOutput(union)
TEMPLATE = lib

DEFINES += UNION_LIBRARY

SOURCES += \
    app/App.cpp \
    app/Window.cpp \
    app/user_events.cpp \
    events/KeyboardEvent.cpp \
    events/MouseButtonEvent.cpp \
    events/MouseMotionEvent.cpp \
    app/SingleViewAppDelegate.cpp \
    camera/EyeCamera.cpp \
    camera/EyeCameraInputController.cpp \
    helpers/OpenGLHelper.cpp \
    app/YobaAppDelegate.cpp \
    delegates/Static3DSceneAppDelegate.cpp \
    lights/PhongMaterial.cpp \
    lights/Light.cpp \
    lights/OmniLight.cpp \
    camera/firstpersoncamera.cpp \
    shading/Shader.cpp \
    models/ModelInterface.cpp \
    animators/AnimatorInterface.cpp \
    models/VertexBuffer.cpp \
    helpers/ResourceHeap.cpp \
    models/BoundingBox.cpp \
    models/RenderContext.cpp \
    lights/TexturedMaterial.cpp \
    shading/Program.cpp \
    texturing/Texture2D.cpp \
    shading/ProgramBridge.cpp \
    models/StaticModel.cpp \
    models/StaticMesh.cpp \
    camera/OverviewAndRotateCamera.cpp \
    helpers/OpenALHelper.cpp \
    texturing/FrameBuffer.cpp \
    texturing/RenderBuffer.cpp

HEADERS +=\
    math/vec4.h \
    math/vec3.h \
    math/vec2.h \
    math/rectangle.h \
    math/quat.h \
    math/math \
    math/mat4.h \
    math/mat3.h \
    math/geometry.h \
    app/App.h \
    app/IAppDelegate.h \
    app/Window.h \
    app/AppOptions.h \
    app/user_events.h \
    app/IdleAppDelegate.h \
    events/KeyboardEvent.h \
    events/Events.h \
    events/MouseButtonEvent.h \
    events/MouseMotionEvent.h \
    union-all.h \
    app/SingleViewAppDelegate.h \
    camera/EyeCamera.h \
    camera/ICamera.h \
    camera/IInputController.h \
    camera/EyeCameraInputController.h \
    helpers/OpenGLHelper.h \
    app/YobaAppDelegate.h \
    delegates/Static3DSceneAppDelegate.h \
    lights/PhongMaterial.h \
    lights/Light.h \
    lights/OmniLight.h \
    camera/firstpersoncamera.h \
    shading/Shader.h \
    models/ModelInterface.h \
    animators/AnimatorInterface.h \
    models/VertexBuffer.h \
    helpers/ResourceHeap.h \
    models/VertexBufferRange.h \
    models/BoundingBox.h \
    models/RenderContext.h \
    lights/TexturedMaterial.h \
    texturing/Texture2D.h \
    camera/IFirstPersonCamera.h \
    shading/Program.h \
    shading/ProgramBridge.h \
    models/StaticModel.h \
    models/StaticMesh.h \
    camera/OverviewAndRotateCamera.h \
    helpers/OpenALHelper.h \
    texturing/FrameBuffer.h \
    texturing/RenderBuffer.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE39900D2
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = union.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

HEADERS += \
    app/DynamicGameAppDelegate.h

SOURCES += \
    app/DynamicGameAppDelegate.cpp
