#ifndef STATIC3DSCENEAPPDELEGATE_H
#define STATIC3DSCENEAPPDELEGATE_H

#include "../app/IAppDelegate.h"
#include "../math/vec3.h"
#include <string>

class DataOfStatic3DSceneAppDelegate;
namespace GL { class IFirstPersonCamera; }

/**
 * 3D, lights and blending
 */
struct Static3DSceneAppDelegateOptions
{
    double zNear;
    double zFar;
    double cameraSpeed;
    std::string cameraType; // "fly", "walk", "rotate"

    Static3DSceneAppDelegateOptions()
        : zNear(0.05)
        , zFar(20.0)
        , cameraSpeed(10.0)
        , cameraType("fly") {}
};

class Static3DSceneAppDelegate : public IAppDelegate
{
public:
    Static3DSceneAppDelegate(const Static3DSceneAppDelegateOptions &options = Static3DSceneAppDelegateOptions());
    ~Static3DSceneAppDelegate();

    void willRedisplay(const Window &window, float deltaTime);

    /**
     * @brief Changes projection matrix and camera parameters,
     * does not call redisplaying
     */
    void didResize(const Window &window);

    /**
     * @brief Initializes viewport, camera, projection matrix,
     * 3D, blending, smoothing and lighing
     */
    void didInitialized(const Window &window);

    /**
     * @brief Sends event to camera controller
     */
    void pressedKey(const KeyboardEvent &ev);

    /**
     * @brief Sends event to camera controller
     */
    void pressedMouse(const MouseButtonEvent &ev);

    /**
     * @brief Sends event to camera controller
     */
    void releasedKey(const KeyboardEvent &ev);

    /**
     * @brief Sends event to camera controller
     */
    void releasedMouse(const MouseButtonEvent &ev);

    /**
     * @brief Sends event to camera controller
     */
    void movedMouse(const MouseMotionEvent &ev);

protected:
    const vec3d &getEyePosition() const;

    GL::IFirstPersonCamera *camera() const;

private:
    void createCamera(const Window &window);

    DataOfStatic3DSceneAppDelegate *d;
};

#endif // STATIC3DSCENEAPPDELEGATE_H
