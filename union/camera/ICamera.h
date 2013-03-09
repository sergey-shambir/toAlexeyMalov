#pragma once
#include <math/vec3.h>

namespace GL {

class ICamera
{
public:
    /**
       \brief Sets current OpenGL model-view matrix
     */
    virtual void install() const = 0;

    virtual ~ICamera() { }

    virtual void loadLookAtRH(vec3d const& eye,
                              vec3d const& at,
                              vec3d const& up) = 0;
};

} // namespace glplus
