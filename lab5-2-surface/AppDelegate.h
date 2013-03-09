#ifndef APPDELEGATE_H
#define APPDELEGATE_H

#include <union-all.h>
#include <vector>

class AppDelegate : public Static3DSceneAppDelegate
{
public:
    AppDelegate();

    void willRedisplay(const Window &window, float deltaTime);
    void didInitialized(const Window &window);

private:
    void drawSurface();

    std::vector<vec3d> m_verticies;
    std::vector<vec3d> m_normales;
    std::vector<unsigned> m_indicies;
    GL::OmniLight m_lamp0;
    GL::OmniLight m_lamp1;
    GL::PhongMaterial m_surfaceMaterial;
};

#endif // APPDELEGATE_H
