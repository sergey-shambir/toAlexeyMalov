#pragma once

#include "../math/mat4.h"

class OpenGLHelper
{
    OpenGLHelper() = delete;
public:
    static void enable3D(bool enabled);
    static void enableLights(bool enabled);
    static void enable2DTextures(bool enabled);
    static void enableBlendAndSmooth(bool enabled);

    static void toggleLights();
    static void toggle2DTextures();
    static void toggleWiredView();

    static void increaseLineWidth();
    static void decreaseLineWidth();
    static void dumpIfError();

    static void setProjectionMatrix(const mat4d &value);
    static void setModelViewMatrix(const mat4d& value);
    static void readModelViewMatrix(mat4d& out);

    static void renderTestCube();
};
