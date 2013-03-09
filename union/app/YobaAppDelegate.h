#ifndef YOBAAPPDELEGATE_H
#define YOBAAPPDELEGATE_H

#include "SingleViewAppDelegate.h"

class Simple3DAppDelegate : public SingleViewAppDelegate
{
public:
    Simple3DAppDelegate();

    virtual void willRedisplay(const Window &window, float deltaTime);
    virtual void didResize(const Window &window);
    virtual void didInitialized(const Window &window);
};

#endif // YOBAAPPDELEGATE_H
