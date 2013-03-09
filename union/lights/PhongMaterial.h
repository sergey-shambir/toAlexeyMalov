#pragma once
#include "../math/vec4.h"

struct aiColor3D;

namespace GL {

/**
   \class GL::PhongMaterial
   Материал для модели освещения фонга,
   компонента Emission отсутствует (т.к. мало кому нужна)
 */

class PhongMaterial
{
public:
    PhongMaterial();
    ~PhongMaterial();

    const vec4 &ambient() const;
    const vec4 &diffuse() const;
    const vec4 &specular() const;
    float shininess() const;

    void setAmbient(const vec3 &values);
    void setAmbient(const vec4 &values);
    void setAmbient(const aiColor3D& color);

    void setDiffuse(const vec3 &values);
    void setDiffuse(const vec4 &values);
    void setDiffuse(const aiColor3D& color);

    void setSpecular(const vec3 &values);
    void setSpecular(const vec4 &values);
    void setSpecular(const aiColor3D& color);

    void setShininess(float value);

    //! Передаёт параметры материала в текущее состояние OpenGL
    virtual void install() const;

private:
    vec4 m_ambient;
    vec4 m_diffuse;
    vec4 m_specular;
    float m_shininess;
};

} // namespace GL
