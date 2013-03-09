#pragma once

#include "IFirstPersonCamera.h"
#include <math/mat4.h>

/**
  Данный класс камеры работает одновременно с OpenGL и OpenAL, устанавливая положение
  как зрителя, так и слушателя
 */

namespace GL {

class EyeCamera : public IFirstPersonCamera
{
public:
    EyeCamera(int viewportWidth, int viewportHeight);
    ~EyeCamera();

    void install() const;

    void loadLookAtRH(vec3d const& eye,
                      vec3d const& at,
                      vec3d const& up);

    void setViewport(int width, int height);
    void getViewport(int &width, int &height);

    void movedMouse(int dx, int dy);

    /**
       \brief Decides if camera should perform onMotion() method or just ignore it
     */
    void setTrackingEnabled(bool enabled);

    bool isTrackingEnabled() const;

    vec3d speed() const;
    void setSpeed(const vec3d &value);

    void move(float deltaTime);

    const vec3d &positionOfEye() const;

    bool capturesMouse() const { return true; }

private:
    std::pair<int, int> m_viewport;
    vec3d m_eye;   /**< Eye position */
    vec3d m_front; /**< Front direction */
    vec3d m_up;    /**< Up direction */
    vec3d m_speed; /**< Speed, parrots per second, front/left/up */

    bool m_trackingEnabled;
    bool m_firstMovePassed;
};

} // namespace GL
