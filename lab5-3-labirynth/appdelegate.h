#pragma once

#include <union-all.h>
#include <vector>

class DataOfAppDelegate;

class AppDelegate : public Static3DSceneAppDelegate
{
public:
    AppDelegate(const std::string &imagePath);
    ~AppDelegate();

    void willRedisplay(const Window &window, float deltaTime);
    void didInitialized(const Window &window);
    void willQuit(const Window &window);

private:
    DataOfAppDelegate *d;
};
