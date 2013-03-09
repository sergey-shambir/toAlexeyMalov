#include "OpenGLHelper.h"
#include <GL/glew.h>
#include <stdio.h>

static bool isModelViewMatrixActive()
{
    GLint mode;
    glGetIntegerv(GL_MATRIX_MODE, &mode);
    return (mode == GL_MODELVIEW);
}

void OpenGLHelper::enable3D(bool enabled)
{
    if (enabled) {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
}

void OpenGLHelper::enableLights(bool enabled)
{
    if (enabled) {
        glEnable(GL_LIGHTING);
        glShadeModel(GL_SMOOTH);
    } else {
        glDisable(GL_LIGHTING);
    }
}

void OpenGLHelper::enable2DTextures(bool enabled)
{
    if (enabled) {
        glEnable(GL_TEXTURE_2D);
    } else {
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void OpenGLHelper::enableBlendAndSmooth(bool enabled)
{
    if (enabled) {
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glLineWidth(3);
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        glLineWidth(1);
        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_BLEND);
    }
}

void OpenGLHelper::toggleLights()
{
    bool shouldEnable = !glIsEnabled(GL_LIGHTING);
    enableLights(shouldEnable);
}

void OpenGLHelper::toggle2DTextures()
{
    bool shouldEnable = !glIsEnabled(GL_TEXTURE_2D);
    enable2DTextures(shouldEnable);
}

void OpenGLHelper::toggleWiredView()
{
    GLint param;
    glGetIntegerv(GL_POLYGON_MODE, &param);

    if (GL_FILL == param) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void OpenGLHelper::increaseLineWidth()
{
    // FIXME: not implemented
}

void OpenGLHelper::decreaseLineWidth()
{
    // FIXME: not implemented
}

void OpenGLHelper::dumpIfError()
{
    GLenum error = glGetError();
    if (error == GL_NO_ERROR)
        return;

    switch (error)
    {
        case GL_INVALID_ENUM:
            puts("OGL ERROR: Invalid enum value passed");
            break;

        case GL_INVALID_VALUE:
            puts("OGL ERROR: Invalid value passed");
            break;

        case GL_INVALID_OPERATION:
            puts("OGL ERROR: Operation cannot be called in such place");
            break;

        case GL_STACK_OVERFLOW:
            puts("OGL ERROR: Stack overflow");
            break;

        case GL_STACK_UNDERFLOW:
            puts("OGL ERROR: Stack underflow");
            break;

        default:
            puts("Unknown OpenGL error");
            break;
    }
    fflush(stdout);
}

void OpenGLHelper::setProjectionMatrix(const mat4d &value)
{
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(value);
    glMatrixMode(GL_MODELVIEW);
}

void OpenGLHelper::setModelViewMatrix(const mat4d &value)
{
    if (!isModelViewMatrixActive()) {
        puts("ERROR: Cannot set model-view matrix - mode is not GL_MODELVIEW");
        fflush(stdout);
    } else {
        glLoadMatrixd(value);
    }
}

void OpenGLHelper::readModelViewMatrix(mat4d &out)
{
    double* dest = &(out.a00);
    glGetDoublev(GL_MODELVIEW_MATRIX, dest);
}

// Структура - трехмерный вектор
struct Vec3
{
    GLfloat x, y, z;
};

// Структура - четырехкомпонентный цвет
struct Color4
{
    GLubyte r, g, b, a;
};

// Структура - вершина
struct SimpleVertex
{
    Vec3 pos;
    Color4 color;
};

static void drawCube()
{
    /*

       Y
       |
       |
       |
       +---X
      /
     /
    Z
       3----2
      /    /|
     /    / |
    7----6  |
    |  0 |  1
    |    | /
    |    |/
    4----5
    */
    // Массив координат вершин
    static const SimpleVertex vertices[8] =
    {
        {{-1, -1, -1}, {255, 0, 0, 255}},		// 0
        {{+1, -1, -1}, {255, 255, 0, 255}},		// 1
        {{+1, +1, -1}, {0, 255, 0, 255}},		// 2
        {{-1, +1, -1}, {0, 0, 0, 255}},			// 3
        {{-1, -1, +1}, {255, 0, 255, 255}},		// 4
        {{+1, -1, +1}, {255, 255, 255, 255}},	// 5
        {{+1, +1, +1}, {0, 255, 255, 255}},		// 6
        {{-1, +1, +1}, {0, 0, 255, 255}},		// 7
    };

//    vec3 normals[8] =
//    {
//        vec3(+1, +1, +1),   // 0
//        vec3(-1, +1, +1),	// 1
//        vec3(-1, -1, +1),   // 2
//        vec3(+1, -1, +1),   // 3
//        vec3(+1, +1, -1),   // 4
//        vec3(-1, +1, -1),	// 5
//        vec3(-1, -1, -1),	// 6
//        vec3(+1, -1, -1),	// 7
//    };
//    for (auto& n : normals)
//        n = (-n).normalized();

    // Массив граней, а точнее, индексов составляющих их вершин.
    // Индексы вершин граней перечисляются в порядке их обхода
    // против часовой стрелки (если смотреть на грань снаружи)
    static const unsigned char faces[6][4] =
    {
        {4, 7, 3, 0},	// грань x<0
        {5, 1, 2, 6},	// грань x>0
        {4, 0, 1, 5},	// грань y<0
        {7, 6, 2, 3},	// грань y>0
        {0, 3, 2, 1},	// грань z<0
        {4, 5, 6, 7},	// грань z>0
    };

    // Передаем информацию о массиве вершин
    glVertexPointer(3, GL_FLOAT, sizeof(SimpleVertex), &vertices[0].pos);
//    glNormalPointer(GL_FLOAT, 0, normals);
    // и массиве цветов
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(SimpleVertex), &vertices[1].color);

    // Разрешаем использование массива координат вершин и цветов
    glEnableClientState(GL_VERTEX_ARRAY);
//    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, faces);

    // Выключаем использовнием массива цветов
    glDisableClientState(GL_COLOR_ARRAY);
//    glDisableClientState(GL_NORMAL_ARRAY);
    // Выключаем использование массива координат вершин
    glDisableClientState(GL_VERTEX_ARRAY);
}

void OpenGLHelper::renderTestCube()
{
    drawCube();
}
