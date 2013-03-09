#ifndef SINGLEVIEWAPPDELEGATE_H
#define SINGLEVIEWAPPDELEGATE_H

#include "IAppDelegate.h"

class SingleViewAppDelegate : public IAppDelegate
{
public:
    SingleViewAppDelegate();

    virtual void willRedisplay(const Window &window, float deltaTime);
    virtual void didResize(const Window &window);
    virtual void didInitialized(const Window &window);
};

#endif // SINGLEVIEWAPPDELEGATE_H
