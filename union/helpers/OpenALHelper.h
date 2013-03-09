#pragma once
#include "../math/vec4.h"

class OpenALHelper
{
    OpenALHelper();
public:
    static void setListenerCoordinates(vec3 const& position,
                                       vec3 const& frontDirection,
                                       vec3 const& upDirection);
    static void setListenerCoordinates(vec3d const& position,
                                       vec3d const& frontDirection,
                                       vec3d const& upDirection);
    static void setListenerSpeed(vec3 const& velocity);
    static void setListenerSpeed(vec3d const& velocity);
};
