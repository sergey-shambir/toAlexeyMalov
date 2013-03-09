#include "Static3DSceneAppDelegate.h"
#include "../camera/EyeCamera.h"
#include "../camera/firstpersoncamera.h"
#include "../camera/EyeCameraInputController.h"
#include "../camera/OverviewAndRotateCamera.h"
#include "../helpers/OpenGLHelper.h"
#include "../app/Window.h"
#include "../events/KeyboardEvent.h"

#include <GL/glew.h>
#include <memory>
#include <stdexcept>

class DataOfStatic3DSceneAppDelegate
{
public:
    Static3DSceneAppDelegateOptions m_options;
    std::unique_ptr<GL::IFirstPersonCamera> m_camera;
    std::unique_ptr<EyeCameraInputController> m_cameraCtl;
};

Static3DSceneAppDelegate::Static3DSceneAppDelegate(const Static3DSceneAppDelegateOptions &options)
    : d(new DataOfStatic3DSceneAppDelegate)
{
    d->m_options = options;
}

Static3DSceneAppDelegate::~Static3DSceneAppDelegate()
{
    delete d;
}

void Static3DSceneAppDelegate::didInitialized(const Window &window)
{
    glViewport(0, 0, window.width(), window.height());
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    createCamera(window);
    d->m_cameraCtl.reset(new EyeCameraInputController(*(d->m_camera), d->m_options.cameraSpeed));

    d->m_camera->loadLookAtRH(vec3d(4, 4, 0.5),
                              vec3d(0, 0, 0),
                              vec3d(0, 0, 1));

    mat4d proj;
    proj.loadPerspective(90, window.width() / window.height(), d->m_options.zNear, d->m_options.zFar);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(proj);
    glMatrixMode(GL_MODELVIEW);

    OpenGLHelper::enable3D(true);
    OpenGLHelper::enableBlendAndSmooth(true);
    OpenGLHelper::enableLights(true);
    OpenGLHelper::dumpIfError();
}

void Static3DSceneAppDelegate::didResize(const Window &window)
{
    glViewport(0, 0, window.width(), window.height());
    d->m_camera->setViewport(window.width(), window.height());

    mat4d proj;
    proj.loadPerspective(90, window.width() / window.height(), 0.05, 20);
    glMatrixMode(GL_PROJECTION);
}

void Static3DSceneAppDelegate::willRedisplay(const Window &window, float deltaTime)
{
    (void)window;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    d->m_camera->move(deltaTime);
    d->m_camera->install();
}

void Static3DSceneAppDelegate::pressedKey(const KeyboardEvent &ev)
{
    if (ev.m_key == SDLK_v) {
        OpenGLHelper::toggleWiredView();
        return;
    }

    if (ev.m_key == SDLK_t) {
        OpenGLHelper::toggle2DTextures();
        return;
    }

    if (ev.m_key == SDLK_l) {
        OpenGLHelper::toggleLights();
        return;
    }

    if (ev.m_key == SDLK_EQUALS) {
        OpenGLHelper::increaseLineWidth();
        return;
    }

    if (ev.m_key == SDLK_MINUS) {
        OpenGLHelper::decreaseLineWidth();
        return;
    }

    d->m_cameraCtl->pressedKey(ev);
}

void Static3DSceneAppDelegate::pressedMouse(const MouseButtonEvent &ev)
{
    d->m_cameraCtl->pressedMouse(ev);
}

void Static3DSceneAppDelegate::releasedKey(const KeyboardEvent &ev)
{
    d->m_cameraCtl->releasedKey(ev);
}

void Static3DSceneAppDelegate::releasedMouse(const MouseButtonEvent &ev)
{
    d->m_cameraCtl->releasedMouse(ev);
}

void Static3DSceneAppDelegate::movedMouse(const MouseMotionEvent &ev)
{
    d->m_cameraCtl->movedMouse(ev);
}

const vec3d &Static3DSceneAppDelegate::getEyePosition() const
{
    return d->m_camera->positionOfEye();
}

GL::IFirstPersonCamera *Static3DSceneAppDelegate::camera() const
{
    return d->m_camera.get();
}

void Static3DSceneAppDelegate::createCamera(const Window &window)
{
    const std::string &type = d->m_options.cameraType;

    if (type == "fly")
        d->m_camera.reset(new GL::EyeCamera(window.width(), window.height()));

    else if (type == "walk")
        d->m_camera.reset(new GL::FirstPersonCamera(window.width(), window.height()));

    else if (type == "rotate")
        d->m_camera.reset(new GL::OverviewAndRotateCamera(window.width(), window.height()));

    else {
        throw std::runtime_error("Invalid camera type: " + type);
    }
}
