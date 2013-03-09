#pragma once
#include <union-all.h>

class DataOfAppDelegate;

class AppDelegate : public Static3DSceneAppDelegate
{
public:
    AppDelegate();
    ~AppDelegate();

    void didInitialized(const Window &window);
    void willRedisplay(const Window &window, float deltaTime);

private:
    void loadModel0();
    void loadModel1();
    void loadModel2();

    DataOfAppDelegate* d;
};
