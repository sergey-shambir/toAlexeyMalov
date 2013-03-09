#include "YobaAppDelegate.h"
#include "../helpers/OpenGLHelper.h"
#include "Window.h"
#include "../math/mat4.h"
#include <GL/glew.h>

typedef SingleViewAppDelegate super;

Simple3DAppDelegate::Simple3DAppDelegate()
{
}

void Simple3DAppDelegate::willRedisplay(const Window &window, float deltaTime)
{
    super::willRedisplay(window, deltaTime);
}

void Simple3DAppDelegate::didResize(const Window &window)
{
    super::didResize(window);
}

void Simple3DAppDelegate::didInitialized(const Window &window)
{
    super::didInitialized(window);

    {
    }
}
