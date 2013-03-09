#ifndef APPDELEGATE_H
#define APPDELEGATE_H

#include <union-all.h>

class DataOfAppDelegate;

class AppDelegate : public Static3DSceneAppDelegate
{
public:
    AppDelegate();
    ~AppDelegate();

    void willRedisplay(const Window &window, float deltaTime);
    void didResize(const Window &window);
    void didInitialized(const Window &window);

private:
    DataOfAppDelegate *d;
};

#endif // APPDELEGATE_H
