#ifndef APPDELEGATE_H
#define APPDELEGATE_H

#include <stdafx.h>

class Lab9FilterAppData;

class Lab9FilterAppDelegate : public Static3DSceneAppDelegate
{
public:
    Lab9FilterAppDelegate();
    ~Lab9FilterAppDelegate();

    void willRedisplay(const Window &window, float deltaTime);
    void didInitialized(const Window &window);

private:
    void loadModel1();

    void setupViewportAndProjection(const Window &window);
    void drawObjectToTexture();

    Lab9FilterAppData *d;
};

#endif // APPDELEGATE_H
