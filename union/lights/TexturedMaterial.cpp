#include "TexturedMaterial.h"
#include "../helpers/ResourceHeap.h"

namespace GL {

TexturedMaterial::TexturedMaterial()
{
}

TexturedMaterial::~TexturedMaterial()
{
}

void TexturedMaterial::setDiffuseTexture(const Texture2D &texture)
{
    m_diffuseTexture = texture;
}

const Texture2D& TexturedMaterial::diffuseTexture() const
{
    return m_diffuseTexture;
}

void TexturedMaterial::install() const
{
    PhongMaterial::install();
    m_diffuseTexture.bindAnyway();
}

} // namespace GL
