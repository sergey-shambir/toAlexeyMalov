#pragma once
#include "ICamera.h"

namespace GL {

class IFirstPersonCamera: public ICamera
{
public:
    virtual void setViewport(int width, int height) = 0;
    virtual void getViewport(int &width, int &height) = 0;
    virtual void movedMouse(int dx, int dy) = 0;

    /**
       \brief Decides if camera should perform onMotion() method or just ignore it
     */
    virtual void setTrackingEnabled(bool enabled) = 0;
    virtual bool isTrackingEnabled() const = 0;

    virtual vec3d speed() const = 0;
    virtual void setSpeed(const vec3d &value) = 0;

    virtual void move(float deltaTime) = 0;
    virtual const vec3d &positionOfEye() const = 0;

    virtual bool capturesMouse() const = 0;
};

} // namespace GL
