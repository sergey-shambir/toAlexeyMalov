#pragma once
#include "PhongMaterial.h"
#include "../texturing/Texture2D.h"

namespace GL {

class Texture2D;

class TexturedMaterial : public PhongMaterial
{
public:
    TexturedMaterial();
    ~TexturedMaterial();

    void setDiffuseTexture(const Texture2D& diffuseTexture);
    const Texture2D& diffuseTexture() const;

    void install() const;

private:
    Texture2D m_diffuseTexture;
};

} // namespace GL
