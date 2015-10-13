#ifndef UNION_APP_H
#define UNION_APP_H

#include "IAppDelegate.h"
#include "AppOptions.h"

class DataOfApp;

class App
{
public:
    /**
     * @brief Constructs app with given delegate
     * @param delegate - cannot be null, constructed app takes ownership
     * on this pointer
     * @param options - window size, title, framerate, etc.
     */
    App(IAppDelegate *delegate, AppOptions const& options = AppOptions());
    ~App();

    /**
     * Initializes OpenGL (SDL+GLEW) and DevIL
     * Starts main events loop.
     */
    void exec(int argc, char *argv[]);

#if !defined(UNION_LIBRARY)
protected:
#endif

    void unlockRedisplay();

private:
    bool initLibs(int argc, char *argv[]);
    void initGLContext();
    void initGLEW();
    void initUpdateTimer();
    void mainLoop();

    void postRedisplayByInput();

    DataOfApp *d;
};

#endif // UNION_APP_H
