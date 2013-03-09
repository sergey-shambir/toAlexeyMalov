#pragma once

#include "IFirstPersonCamera.h"

/**
  Данный класс камеры работает одновременно с OpenGL и OpenAL, устанавливая положение
  как зрителя, так и слушателя
 */

namespace GL {

class IFirstPersonCameraDelegate
{
public:
    virtual vec3d fixPosition(const vec3d &position) = 0;
};

class FirstPersonCamera : public IFirstPersonCamera
{
public:
    FirstPersonCamera(int viewportWidth, int viewportHeight);

    void install() const;
    void loadLookAtRH(const vec3d &eye,
                      const vec3d &at,
                      const vec3d &up);

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

    /** weak pointer, does not take ownership */
    void setDelegate(IFirstPersonCameraDelegate *delegate);

    bool capturesMouse() const { return true; }

private:
    std::pair<int, int> m_viewport;
    vec3d m_eye;   /**< Eye position */
    vec3d m_front; /**< Front direction */
    std::pair<float, float> m_speed; /**< Speed, parrots per second, front/left */

    bool m_trackingEnabled;
    bool m_firstMovePassed;

    IFirstPersonCameraDelegate *m_delegate;
};

} // namespace GL
