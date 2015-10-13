#include "AppDelegate.h"
#include <GL/glew.h>

#include <SDL2/SDL_mouse.h>

static const float CAMERA_SPEED = 5.0f;

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
    // и массиве цветов
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(SimpleVertex), &vertices[1].color);

    // Разрешаем использование массива координат вершин и цветов
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, faces);

    // Выключаем использовнием массива цветов
    glDisableClientState(GL_COLOR_ARRAY);
    // Выключаем использование массива координат вершин
    glDisableClientState(GL_VERTEX_ARRAY);
}

typedef Static3DSceneAppDelegate super;

class DataOfAppDelegate
{
public:
};

AppDelegate::AppDelegate()
    : d(nullptr)
{
}

AppDelegate::~AppDelegate()
{
    delete d;
}

void AppDelegate::willRedisplay(const Window &window, float deltaTime)
{
    super::willRedisplay(window, deltaTime);

    drawCube();
}

void AppDelegate::didResize(const Window &window)
{
    super::didResize(window);
}

void AppDelegate::didInitialized(const Window &window)
{
    super::didInitialized(window);
    OpenGLHelper::enableLights(false);
    OpenGLHelper::enable2DTextures(false);
}
