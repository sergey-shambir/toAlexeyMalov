#ifndef IAPPDELEGATE_H
#define IAPPDELEGATE_H

#include "../camera/IInputController.h"
class Window;

class IAppDelegate : public IInputController
{
public:
    virtual ~IAppDelegate() {}

    //////////////////////////////// REQUIRED //////////////////////////////////

    /**
     * Called when need redisplay
     */
    virtual void willRedisplay(Window const& window, float deltaTime) = 0;

    /**
     * Called right after window have been resized
     */
    virtual void didResize(Window const& window) = 0;

    /**
     * Called when OpenGL and audio was initialized
     * @note You should create glViewport manually
     */
    virtual void didInitialized(Window const& window) = 0;

    //////////////////////////////// OPTIONAL //////////////////////////////////

    /**
     * @return True if redisplay needed
     */
    virtual bool didBecomeActive(Window const&)                 { return false; }
    virtual void didBecomeInactive(Window const&)               { }
    virtual void willQuit(Window const&)                        { }
};

#endif // IAPPDELEGATE_H
