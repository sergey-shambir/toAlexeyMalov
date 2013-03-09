#include "OmniLight.h"

namespace GL {

OmniLight::OmniLight(const vec3 &position)
    : m_position(position, 1.0f)
    , m_linearAttenuation(0)
    , m_quadraticAttenuation(0)
    , m_constantAttenuation(1)
{
}

OmniLight::~OmniLight()
{
}

vec4 const& OmniLight::position() const
{
    return m_position;
}

void OmniLight::setPosition(const vec3 &position)
{
    m_position = vec4(position, 1.0f);
}

void OmniLight::setQuadraticAttenuation(float value)
{
    m_quadraticAttenuation = value;
}

void OmniLight::setLinearAttenuation(float value)
{
    m_linearAttenuation = value;
}

void OmniLight::setConstantAttenuation(float value)
{
    m_constantAttenuation = value;
}

void OmniLight::render(GLenum light)
{
    installColors(light);
    glLightfv(light, GL_POSITION, m_position);
    glLightf(light, GL_LINEAR_ATTENUATION, m_linearAttenuation);
    glLightf(light, GL_QUADRATIC_ATTENUATION, m_quadraticAttenuation);
    glLightf(light, GL_CONSTANT_ATTENUATION, m_constantAttenuation);
}

} // namespace glplus
