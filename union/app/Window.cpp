#include "Window.h"

#include <string>
#include <assert.h>
#include <SDL/SDL.h>
#include <GL/glew.h>

static void getFullscreenSize(uint16_t &width, uint16_t &height)
{
    const SDL_VideoInfo* info = SDL_GetVideoInfo();
    width = info->current_w;
    height = info->current_h;
}

class DataWindow
{
public:
    std::string title;

    SDL_Surface *screen;
    WindowAction onDisplay;
    WindowAction onResize;
    uint16_t width;
    uint16_t height;
    bool isFullscren;
    uint16_t fullscreenWidth;
    uint16_t fullscreenHeight;
    uint16_t windowedWidth;
    uint16_t windowedHeight;

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
}

void Window::setTitle(const char *title)
{
    d->title = title;
    SDL_WM_SetCaption(title, nullptr);
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

void Window::redisplay()
{
    d->onDisplay(*this);
}

void Window::resize(uint16_t width, uint16_t height)
{
    d->width = width;
    d->height = height;
    d->screen = SDL_SetVideoMode(width, height, 32, SDL_OPENGL);

    d->onResize(*this);
}

void Window::toggleFullscreen(bool enterFullscreen)
{
    if (d->isFullscren == enterFullscreen)
        return;
    d->isFullscren = enterFullscreen;

    if (!d->fullscreenWidth)
        getFullscreenSize(d->fullscreenWidth, d->fullscreenHeight);

    uint32_t videoFlags = SDL_OPENGL | SDL_DOUBLEBUF;
    if (enterFullscreen)
    {
        d->windowedWidth = d->width;
        d->windowedHeight = d->height;
        d->width = d->fullscreenWidth;
        d->height = d->fullscreenHeight;
        videoFlags = videoFlags | SDL_FULLSCREEN;
    }
    else
    {
        d->width = d->windowedWidth;
        d->height = d->windowedHeight;
    }

    d->screen = SDL_SetVideoMode(d->width, d->height, 32, videoFlags);
    d->onResize(*this);
}
