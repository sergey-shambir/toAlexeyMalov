#include "Window.h"

#include <string>
#include <assert.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>

static void getFullscreenSize(uint16_t &width, uint16_t &height)
{
    SDL_DisplayMode mode;
    SDL_GetDesktopDisplayMode(0, &mode);
    width = mode.w;
    height = mode.h;
}

class DataWindow
{
public:
    std::string title;

    SDL_Surface *screen = nullptr;
    SDL_Window *window = nullptr;
    SDL_GLContext context = nullptr;
    WindowAction onDisplay;
    WindowAction onResize;
    uint16_t width = 0;
    uint16_t height = 0;
    bool isFullscren = false;
    uint16_t fullscreenWidth = 0;
    uint16_t fullscreenHeight = 0;
    uint16_t windowedWidth = 0;
    uint16_t windowedHeight = 0;

    DataWindow();
    ~DataWindow();
};

DataWindow::DataWindow()
    : windowedWidth(640)
    , windowedHeight(480)
{
}

DataWindow::~DataWindow()
{
}

Window::Window()
    : d(new DataWindow())
{
    resetHandlers();
}

Window::~Window()
{
    if (d->context)
    {
        SDL_GL_DeleteContext(d->context);
        d->context = nullptr;
    }
    if (d->window)
    {
        SDL_DestroyWindow(d->window);
        d->window = nullptr;
    }
}

void Window::setTitle(const char *title)
{
    d->title = title;
    if (d->window)
    {
        SDL_SetWindowTitle(d->window, d->title.c_str());
    }
}

const char *Window::title() const
{
    return d->title.c_str();
}

uint16_t Window::width() const
{
    return d->width;
}

uint16_t Window::height() const
{
    return d->height;
}

void Window::resetHandlers()
{
    d->onDisplay = [] (const Window &) {};
    d->onResize = [] (const Window &) {};
}

void Window::onResize(const WindowAction &action)
{
    d->onResize = action;
}

void Window::onDisplay(const WindowAction &action)
{
    d->onDisplay = action;
}

void Window::warpMouse(int x, int y) const
{
    if (d->window) {
        SDL_WarpMouseInWindow(d->window, x, y);
    }
}

void Window::redisplay()
{
    d->onDisplay(*this);
}

void Window::swapBuffers()
{
    SDL_GL_SwapWindow(d->window);
}

void Window::createWindow()
{
    if (d->context)
    {
        SDL_GL_DeleteContext(d->context);
        d->context = nullptr;
    }
    if (d->window)
    {
        SDL_DestroyWindow(d->window);
        d->window = nullptr;
    }
    Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
    if (d->isFullscren)
    {
        flags |= SDL_WINDOW_FULLSCREEN;
    }
    d->window = SDL_CreateWindow(d->title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, d->width, d->height, flags);
    d->context = SDL_GL_CreateContext(d->window);
}

void Window::resize(uint16_t width, uint16_t height)
{
    d->width = width;
    d->height = height;
    if (d->window)
    {
        SDL_SetWindowSize(d->window, d->width, d->height);
    }
    d->onResize(*this);
}

void Window::toggleFullscreen(bool enterFullscreen)
{
    if (d->isFullscren == enterFullscreen)
        return;
    d->isFullscren = enterFullscreen;

    if (!d->fullscreenWidth)
        getFullscreenSize(d->fullscreenWidth, d->fullscreenHeight);

    if (enterFullscreen)
    {
        d->windowedWidth = d->width;
        d->windowedHeight = d->height;
        d->width = d->fullscreenWidth;
        d->height = d->fullscreenHeight;
    }
    else
    {
        d->width = d->windowedWidth;
        d->height = d->windowedHeight;
    }

    if (d->window)
    {
        SDL_SetWindowFullscreen(d->window, SDL_WINDOW_FULLSCREEN);
    }
    d->onResize(*this);
}
