#include "App.h"
#include "Window.h"
#include "user_events.h"

#include "../events/Events.h"
#include "../helpers/ResourceHeap.h"

#include <assert.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <IL/il.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <stdio.h>

static const char WORKDIR[] = "../../toAlexeyMalov/Data/";

////////////////////////////////////////////////////////////////////////////////

namespace {
unsigned redisplayTimerCallback(const unsigned interval, void *)
{
    SDL_Event event = createUserEvent(UserEvents::EVENT_REDISPLAY);
    SDL_PushEvent(&event);
    return interval;
}

unsigned unlockTimerCallback(const unsigned interval, void *thisPtr)
{
    App *app = reinterpret_cast<App*>(thisPtr);
    app->unlockRedisplay();
    return interval;
}
} // anonymous namespace

////////////////////////////////////////////////////////////////////////////////

class DataOfApp
{
public:
    IAppDelegate *delegate;
    AppOptions options;
    SDL_TimerID updateTimerId;
    bool redisplayRequestedWhileLocked;
    unsigned systemTimeMilliseconds;
    Window window;
    GL::ResourceHeap appOpenGLHeap;

    void destroyTimer() {
        SDL_RemoveTimer(updateTimerId);
        updateTimerId = 0;
    }

    /**
     * @brief updateTime
     */
    unsigned updateTime() {
        unsigned ticks = SDL_GetTicks();
        unsigned diff = ticks - systemTimeMilliseconds;
        systemTimeMilliseconds = ticks;
        return diff;
    }

    DataOfApp(IAppDelegate *vDelegate, AppOptions vOptions)
        : delegate(vDelegate)
        , options(vOptions)
        , updateTimerId(0)
        , redisplayRequestedWhileLocked(false)
        , systemTimeMilliseconds(0) {
    }

    ~DataOfApp() {
        destroyTimer();
        delete delegate;
    }
};

////////////////////////////////////////////////////////////////////////////////

App::App(IAppDelegate *delegate, const AppOptions &options)
    : d(new DataOfApp(delegate, options))
{
    assert(delegate);
    assert(options.framerate != 0 && "Framerate cannot be 0");
    assert(options.framerate < 1000 && "Framerate should be less that 1000");
}

App::~App()
{
    delete d;
}

void App::exec(int argc, char *argv[])
{
#if defined(_WIN32)
#else
    if (0 != chdir(WORKDIR)) {
        printf("Cannot goto directory %s, where placed all data files\n", WORKDIR);
        exit(1);
    }
#endif

    if (initLibs(argc, argv))
    {
        initGLContext();
        initGLEW();
        initUpdateTimer();
        mainLoop();
    }
}

void App::unlockRedisplay()
{
    d->destroyTimer();
    if (d->redisplayRequestedWhileLocked) {
        d->redisplayRequestedWhileLocked = false;
        SDL_Event ev = createUserEvent(UserEvents::EVENT_REDISPLAY);
        SDL_PushEvent(&ev);
    }
}

bool App::initLibs(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    if (SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_NOPARACHUTE))
    {
        puts(SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    d->updateTime();

    ilInit();
    return true;
}

void App::initGLContext()
{
    if (d->options.fullScreen)
        d->window.toggleFullscreen(true);
    else
        d->window.resize(d->options.windowSize.first, d->options.windowSize.second);
    d->window.setTitle(d->options.title.c_str());
    d->window.onResize([this](Window const& window) {
            d->delegate->didResize(window);
        });
    d->window.onDisplay([this](Window const& window) {
            float deltaTime = 0.001f * d->updateTime();
            d->delegate->willRedisplay(window, deltaTime);
        });
    d->window.createWindow();
}

void App::initGLEW()
{
    // Libraries initialization
    GLenum glewOk = glewInit();
    if (glewOk != GLEW_OK) {
        switch (glewOk) {
        case GLEW_ERROR_NO_GL_VERSION:
            puts("GLEW initialization error: missing GL versions");
            break;
        case GLEW_ERROR_GL_VERSION_10_ONLY:
            puts("GLEW initialization error: need at least OpenGL 1.1");
            break;
        case GLEW_ERROR_GLX_VERSION_11_ONLY:
            puts("GLEW initialization error: need at least GLX 1.2");
            break;
        }
        exit(1);
    }

    if (!GLEW_ARB_shader_objects)
    {
        puts("Video initialization error: Missing shaders support");
        exit(1);
    }
}

void App::initUpdateTimer()
{
    d->destroyTimer();

    if (d->options.useTimer) {
        unsigned interval = 1000 / d->options.framerate;
        d->updateTimerId = SDL_AddTimer(interval, redisplayTimerCallback, nullptr);
    }
}

void App::mainLoop()
{
    d->delegate->didInitialized(d->window);

    // first frame
    {
        d->window.redisplay();
        d->window.swapBuffers();
    }

    bool wantsQuit = false;
    SDL_Event event;
    while (!wantsQuit && SDL_WaitEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            wantsQuit =  true;
            break;

        case SDL_KEYDOWN: {
            SDL_Keysym sym = event.key.keysym;
            if (sym.sym == SDLK_F4 && ((sym.mod | KMOD_LALT) || (sym.mod | KMOD_RALT)))
            {
                wantsQuit = true;
                break;
            }

            d->delegate->pressedKey(KeyboardEvent(d->window, sym.sym, sym.mod));
            postRedisplayByInput();
        }
            break;

        case SDL_KEYUP: {
            SDL_Keysym sym = event.key.keysym;
            if (sym.sym == SDLK_ESCAPE) {
                wantsQuit = true;
            }
            else {
                d->delegate->releasedKey(KeyboardEvent(d->window, sym.sym, sym.mod));
                postRedisplayByInput();
            }
        }
            break;

        case SDL_MOUSEMOTION: {
            SDL_MouseMotionEvent ev = event.motion;
            d->delegate->movedMouse(MouseMotionEvent(d->window, ev.x, ev.y, ev.xrel, ev.yrel));
            postRedisplayByInput();
        }
            break;

        case SDL_WINDOWEVENT: {
            SDL_WindowEvent ev = event.window;
            if (ev.event == SDL_WINDOWEVENT_EXPOSED) {
                bool needRedisplay = d->delegate->didBecomeActive(d->window);
                if (needRedisplay)
                    postRedisplayByInput();
            } else if (ev.event == SDL_WINDOWEVENT_HIDDEN) {
                d->delegate->didBecomeInactive(d->window);
            }
        }
            break;

        case SDL_MOUSEBUTTONUP: {
            SDL_MouseButtonEvent ev = event.button;
            MouseButton button = MouseButton::EXTRA;
            switch (ev.button) {
            case SDL_BUTTON_LEFT:
                button = MouseButton::LEFT;
                break;
            case SDL_BUTTON_MIDDLE:
                button = MouseButton::MIDDLE;
                break;
            case SDL_BUTTON_RIGHT:
                button = MouseButton::RIGHT;
                break;
            }
            d->delegate->releasedMouse(MouseButtonEvent(d->window, button, ev.x, ev.y));
            postRedisplayByInput();
        }
            break;

        case SDL_MOUSEBUTTONDOWN: {
            SDL_MouseButtonEvent ev = event.button;
            MouseButton button = MouseButton::EXTRA;
            switch (ev.button) {
            case SDL_BUTTON_LEFT:
                button = MouseButton::LEFT;
                break;
            case SDL_BUTTON_MIDDLE:
                button = MouseButton::MIDDLE;
                break;
            case SDL_BUTTON_RIGHT:
                button = MouseButton::RIGHT;
                break;
            }
            d->delegate->pressedMouse(MouseButtonEvent(d->window, button, ev.x, ev.y));
            postRedisplayByInput();
        }
            break;

        case SDL_USEREVENT:
            if (event.user.code == UserEvents::EVENT_REDISPLAY) {
                d->window.redisplay();
                d->window.swapBuffers();
            }
            break;
        }
    }

    d->delegate->willQuit(d->window);
}

/**
 * Asks for redisplaying after input event.
 *
 * The minimum pause between redisplays is (1000 / Framerate) milliseconds.
 * If pause expired, redisplay will be posted immediately, otherwise it will
 * wait for expiration.
 */
void App::postRedisplayByInput()
{
    if (false == d->options.useTimer) {
        if (0 == d->updateTimerId) {
            SDL_Event ev = createUserEvent(UserEvents::EVENT_REDISPLAY);
            SDL_PushEvent(&ev);

            unsigned interval = 1000 / d->options.framerate;
            d->updateTimerId = SDL_AddTimer(interval, unlockTimerCallback, this);
        } else {
            d->redisplayRequestedWhileLocked = true;
        }
    }
}
