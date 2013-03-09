#ifndef IDLEAPPDELEGATE_H
#define IDLEAPPDELEGATE_H

#include "IAppDelegate.h"

class IdleAppDelegate : public IAppDelegate
{
public:
    void willRedisplay(const Window &, float )  {}
    void didResize(const Window &)              {}
    void didInitialized(const Window &)         {}
};

#endif // IDLEAPPDELEGATE_H
