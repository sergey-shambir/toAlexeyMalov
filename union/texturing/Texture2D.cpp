#include "Texture2D.h"
#include "FrameBuffer.h"
#include "../helpers/ResourceHeap.h"
#include <GL/glew.h>
#include <IL/il.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdexcept>

namespace GL {

static unsigned textureConstructor()
{
    unsigned id;
    glGenTextures(1, &id);
    return id;
}

static void textureDestructor(unsigned textureId)
{
    glDeleteTextures(1, &textureId);
}

Texture2D::Texture2D()
    : m_id(0)
{
}

Texture2D::Texture2D(const Texture2D &other)
    : m_id(other.m_id)
{
    ResourceHeap::instance().retain(other.m_id, ResourceHeap::Texture2D);
}

Texture2D &Texture2D::operator =(const Texture2D &other)
{
    m_id = other.m_id;
    ResourceHeap::instance().retain(other.m_id, ResourceHeap::Texture2D);
    return *this;
}

Texture2D::~Texture2D()
{
    ResourceHeap::instance().release(m_id, ResourceHeap::Texture2D);
}

void Texture2D::bind() const
{
    if (m_id == 0)
        throw std::runtime_error("Binding zero texture");
    bindId(m_id);
}

void Texture2D::bindAnyway() const
{
    bindId(m_id);
}

bool Texture2D::isNull() const
{
    return m_id == 0;
}

unsigned Texture2D::resourceId() const
{
    return m_id;
}

Texture2D Texture2D::createFromImageByPath(const std::string &path)
{
//    SDL_Surface *pSurface(IMG_Load(path.c_str()));

//    Texture2D ret(createFromSDLSurface(*pSurface));
//    SDL_FreeSurface(pSurface);
//    return ret;
    unsigned imageId = ilGenImage();
    ilBindImage(imageId);
    int success = ilLoadImage(path.c_str());
    if (success) {
        success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    }

    if (!success) {
        ilDeleteImage(imageId);

        {
            SDL_Surface *pSurface(IMG_Load(path.c_str()));
            if (!pSurface)
                throw std::runtime_error("Cannot load texture " + path);
            unsigned id = createFromSDLSurface(*pSurface);
            SDL_FreeSurface(pSurface);
            if (id == 0)
                throw std::runtime_error("Cannot load texture: SDL_image not "
                                         "expected this format" + path);
            {
                Texture2D ret;
                ret.m_id = id;
                return ret;
            }
        }
    }
    Texture2D ret(createFromDevILBinded());
    ilDeleteImage(imageId);
    return ret;
}

Texture2D Texture2D::createFromDevILBinded()
{
    Texture2D ret;
    ret.m_id = ResourceHeap::instance().create(textureConstructor,
                                               textureDestructor,
                                               ResourceHeap::Texture2D);
    ret.bind();
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    gluBuild2DMipmaps(GL_TEXTURE_2D, ilGetInteger(IL_IMAGE_BPP),
                      ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
                      ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
    return ret;
}

unsigned Texture2D::createFromSDLSurface(const SDL_Surface &surface)
{
    GLenum format = GL_RGB;
    switch (surface.format->BytesPerPixel)
    {
    case 4:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            format = GL_BGRA;
        else
            format = GL_RGBA;
        break;
    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            format = GL_BGR;
        else
            format = GL_RGB;
        break;
    default:
        return 0;
    }

    unsigned ret;
    ret = ResourceHeap::instance().create(textureConstructor,
                                          textureDestructor,
                                          ResourceHeap::Texture2D);

    bindId(ret);
    gluBuild2DMipmaps(GL_TEXTURE_2D, surface.format->BytesPerPixel,
                      surface.w, surface.h,
                      format, GL_UNSIGNED_BYTE, surface.pixels);
    return ret;
}

Texture2D Texture2D::createLinkedWith(FrameBuffer &buffer)
{
    Texture2D ret;
    ret.m_id = ResourceHeap::instance().create(textureConstructor,
                                               textureDestructor,
                                               ResourceHeap::Texture2D);
    ret.bind();

    // Set filtering parameters
    useParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    useParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (GLEW_EXT_texture_filter_anisotropic)
    {
        int maxAnisotropy = 0;
        glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
        useParameter(GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
    }

    // Link texture with framebuffer
    glTexImage2D(GL_TEXTURE_2D, /*level*/0,
                 GL_RGBA, buffer.width(), buffer.height(), /*border*/ 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, /*pixels from FB*/ nullptr);
    glGenerateMipmapEXT(GL_TEXTURE_2D);

    // Link framebuffer with texture
    glFramebufferTexture2DEXT(
                buffer.type(),
                GL_COLOR_ATTACHMENT0_EXT, // attachment point
                GL_TEXTURE_2D,
                ret.m_id,
                0 // initial mip-level
                );
    return ret;
}

void Texture2D::bindId(unsigned id)
{
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture2D::useParameter(int parameter, int value)
{
    glTexParameteri(GL_TEXTURE_2D, parameter, value);
}

} // namespace GL
