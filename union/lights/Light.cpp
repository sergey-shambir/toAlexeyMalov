#include "Light.h"
#include <GL/glew.h>

namespace GL {
namespace abstract {

Light::Light()
{
}

Light::~Light()
{
}

vec4 const& Light::ambient() const
{
    return m_ambient;
}

vec4 const& Light::diffuse() const
{
    return m_diffuse;
}

vec4 const& Light::specular() const
{
    return m_specular;
}

void Light::setAmbient(const vec4 &ambient)
{
    m_ambient = ambient;
}

void Light::setDiffuse(const vec4 &diffuse)
{
    m_diffuse = diffuse;
}

void Light::setSpecular(const vec4 &specular)
{
    m_specular = specular;
}

void Light::installColors(GLenum light)
{
    glLightfv(light, GL_AMBIENT, m_ambient);
    glLightfv(light, GL_DIFFUSE, m_diffuse);
    glLightfv(light, GL_SPECULAR, m_specular);
}

} // namespace base
} // namespace GLPlus
