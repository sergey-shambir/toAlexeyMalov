#include "SingleViewAppDelegate.h"
#include "Window.h"
#include <GL/glew.h>

SingleViewAppDelegate::SingleViewAppDelegate()
{
}

void SingleViewAppDelegate::willRedisplay(const Window &, float deltaTime)
{
    (void)deltaTime;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SingleViewAppDelegate::didResize(const Window &)
{
}

void SingleViewAppDelegate::didInitialized(const Window &window)
{
    glViewport(0, 0, window.width(), window.height());
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}
