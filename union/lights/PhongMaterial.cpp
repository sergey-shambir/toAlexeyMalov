#include "PhongMaterial.h"

#include <GL/glew.h> // to install material in fixed pipeline
#include <assimp/types.h> // to convert aiColor3D in vec4

namespace GL {

PhongMaterial::PhongMaterial()
    : m_ambient(0, 0, 0, 1)
    , m_diffuse(0, 0, 0, 1)
    , m_specular(0, 0, 0, 1)
    , m_shininess(30.0f)
{
}

PhongMaterial::~PhongMaterial()
{
}

const vec4 &PhongMaterial::ambient() const
{
    return m_ambient;
}

const vec4 &PhongMaterial::diffuse() const
{
    return m_diffuse;
}

const vec4 &PhongMaterial::specular() const
{
    return m_specular;
}

float PhongMaterial::shininess() const
{
    return m_shininess;
}

void PhongMaterial::setAmbient(const vec3 &values)
{
    m_ambient = vec4(values, 1.0);
}

void PhongMaterial::setAmbient(const vec4 &values)
{
    m_ambient = values;
}

void PhongMaterial::setAmbient(const aiColor3D &color)
{
    m_ambient.x = color.r;
    m_ambient.y = color.g;
    m_ambient.z = color.b;
}

void PhongMaterial::setDiffuse(const vec3 &values)
{
    m_diffuse = vec4(values, 1.0);
}

void PhongMaterial::setDiffuse(const vec4 &values)
{
    m_diffuse = values;
}

void PhongMaterial::setDiffuse(const aiColor3D &color)
{
    m_diffuse.x = color.r;
    m_diffuse.y = color.g;
    m_diffuse.z = color.b;
}

void PhongMaterial::setSpecular(const vec3 &values)
{
    m_diffuse = vec4(values, 1.0);
}

void PhongMaterial::setSpecular(const vec4 &values)
{
    m_specular = values;
}

void PhongMaterial::setSpecular(const aiColor3D &color)
{
    m_specular.x = color.r;
    m_specular.y = color.g;
    m_specular.z = color.b;
}

void PhongMaterial::setShininess(float value)
{
    m_shininess = value;
}

void PhongMaterial::install() const
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_shininess);
}

} // namespace GL
