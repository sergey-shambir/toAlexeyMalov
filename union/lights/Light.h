#pragma once

#include "../math/math"

#include <GL/glew.h>

namespace GL {
namespace abstract {

class Light
{
public:
    ~Light();

    vec4 const& ambient() const;
    vec4 const& diffuse() const;
    vec4 const& specular() const;

    void setAmbient(vec4 const& ambient);
    void setDiffuse(vec4 const& diffuse);
    void setSpecular(vec4 const& specular);

    /**
       \brief sends light parameters to OpenGL, does not call glEnable(light)
     */
    virtual void render(GLenum light) = 0;

protected:
    Light();
    void installColors(GLenum light);

private:
    vec4 m_ambient;
    vec4 m_diffuse;
    vec4 m_specular;
};

} // namespace base
} // namespace GLPlus
