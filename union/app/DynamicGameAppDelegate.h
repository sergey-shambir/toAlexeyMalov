#ifndef DYNAMICGAMEAPPDELEGATE_H
#define DYNAMICGAMEAPPDELEGATE_H

#include "IAppDelegate.h"
#include <functional>
#include "../math/math"

struct DynamicGameOpts
{
    unsigned width;
    unsigned height;
};

class DynamicGameAppDelegateData;
class DynamicGameAppDelegate : public IAppDelegate
{
public:
    DynamicGameAppDelegate(const DynamicGameOpts &opts);
    ~DynamicGameAppDelegate();

    void setCellState(unsigned x, unsigned y, bool winner);
    void setWinColor(const vec3 &color);
    void setLoseColor(const vec3 &color);
    void setDefaultColor(const vec3 &color);

    virtual void willRedisplay(const Window &window, float deltaTime);
    virtual void didResize(const Window &window);
    virtual void didInitialized(const Window &window);

private:
    void drawField();

    DynamicGameAppDelegateData *d;
};

#endif // DYNAMICGAMEAPPDELEGATE_H
