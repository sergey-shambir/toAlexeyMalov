#pragma once
#include "IFirstPersonCamera.h"
#include "../math/mat4.h"

namespace GL {

class OverviewAndRotateCamera : public IFirstPersonCamera
{
public:
    OverviewAndRotateCamera(int viewportWidth, int viewportHeight);
    ~OverviewAndRotateCamera();

    void install() const;

    void loadLookAtRH(vec3d const& eye,
                      vec3d const& at,
                      vec3d const& up);

    void setViewport(int width, int height);
    void getViewport(int &width, int &height);

    void movedMouse(int dx, int dy);

    void setTrackingEnabled(bool enabled);
    bool isTrackingEnabled() const;

    vec3d speed() const;
    void setSpeed(const vec3d &value);

    void move(float deltaTime);

    const vec3d &positionOfEye() const;

    bool capturesMouse() const { return false; }

private:
    std::pair<int, int> m_viewport;

    vec3d m_eye;   /**< Eye position */
    vec3d m_front; /**< Front direction */
    vec3d m_up;    /**< Up direction */

    bool m_trackingEnabled;
    bool m_firstMovePassed;
};

} // namespace GL
