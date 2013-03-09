#pragma once

#include "Light.h"

namespace GL {

class OmniLight : public abstract::Light
{
public:
    OmniLight(vec3 const& position);
    ~OmniLight();

    vec4 const& position() const;

    void setPosition(vec3 const& position);
    void setQuadraticAttenuation(float value);
    void setLinearAttenuation(float value);
    void setConstantAttenuation(float value);

    void render(GLenum light);

private:
    vec4 m_position;
    float m_linearAttenuation;
    float m_quadraticAttenuation;
    float m_constantAttenuation;
};

} // namespace glplus
