#include "FrameBuffer.h"
#include "../helpers/ResourceHeap.h"
#include "../math/mat4.h"
#include "../helpers/OpenGLHelper.h"

#include <GL/glew.h>
#include <stdexcept>

/**
  \paragraph Creating, binding, deleting frame buffer
             glGenFramebuffersEXT, glDeleteFramebuffersEXT, glBindFramebufferEXT
  \paragraph Creating, binding, deleting render buffer
             glGenRenderbuffersEXT, glBindRenderbufferEXT, glDeleteRenderbuffersEXT
  \paragraph Attach texture or render buffer to frame buffer
             glFramebufferTextureEXT, glFramebufferRenderbufferEXT
 */

double FIELD_OF_VIEW = 60.0;
double Z_NEAR = 0.05;
double Z_FAR = 100;

namespace GL {

static unsigned bufferConstructor()
{
    if (!GLEW_EXT_framebuffer_object)
    {
        throw std::runtime_error("ERROR: hardware requirement missed - GLEW_EXT_framebuffer_object,"
                                 " update drivers or try run on another graphics card");
    }

    unsigned bufferId(0);
    glGenFramebuffersEXT(1, &bufferId);
    return bufferId;
}

static void bufferDestructor(unsigned bufferId)
{
    glDeleteFramebuffersEXT(1, &bufferId);
}

static unsigned depthBufferConstructor()
{
    if (!GLEW_EXT_framebuffer_object)
    {
        throw std::runtime_error("ERROR: hardware requirement missed - GLEW_EXT_framebuffer_object,"
                                 " update drivers or try run on another graphics card");
    }

    unsigned id(0);
    glGenRenderbuffersEXT(1, &id);
    return id;
}

static void depthBufferDestructor(unsigned id)
{
    glDeleteRenderbuffersEXT(1, &id);
}

FrameBuffer::FrameBuffer() :
    m_id(0),
    m_width(0),
    m_height(0),
    m_useDepthBuffer(false),
    m_depthBuffer(0)
{
}

FrameBuffer::FrameBuffer(const FrameBuffer &other)
    : m_id(other.m_id)
{
    ResourceHeap::instance().retain(m_id, ResourceHeap::FrameBufferExt);
}

FrameBuffer &FrameBuffer::operator =(const FrameBuffer &other)
{
    ResourceHeap::instance().retain(m_id, ResourceHeap::FrameBufferExt);
    m_id = other.m_id;
    return *this;
}

FrameBuffer::~FrameBuffer()
{
    ResourceHeap::instance().release(m_id, ResourceHeap::FrameBufferExt);
}

void FrameBuffer::setWidth(unsigned width)
{
    m_width = width;
}

void FrameBuffer::setHeight(unsigned height)
{
    m_height = height;
}

unsigned FrameBuffer::width() const
{
    return m_width;
}

unsigned FrameBuffer::height() const
{
    return m_height;
}

int FrameBuffer::type() const
{
    return GL_FRAMEBUFFER_EXT;
}

void FrameBuffer::enableDepthBuffer(bool enabled)
{
    m_useDepthBuffer = enabled;
}

void FrameBuffer::checkStatus() const
{
    GLenum status = glCheckFramebufferStatusEXT(type());
    if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
        throw std::runtime_error("Buffer is incomplete");
}

void FrameBuffer::initAndBind()
{
    ensureInitialized();
    bind();
}

void FrameBuffer::bind() const
{
    glBindFramebufferEXT(GL_FRAMEBUFFER, m_id);
}

void FrameBuffer::unbind()
{
    glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::ensureInitialized()
{
    if (m_id == 0) {
        m_id = ResourceHeap::instance().create(bufferConstructor,
                                               bufferDestructor,
                                               ResourceHeap::FrameBufferExt);
    }
    if (m_useDepthBuffer && m_depthBuffer == 0) {
        m_depthBuffer = ResourceHeap::instance().create(
                    depthBufferConstructor,
                    depthBufferDestructor,
                    ResourceHeap::RenderBufferExt
                    );
    }
}

void FrameBuffer::setupViewportAndProjection()
{
    glViewport(0, 0, m_width, m_height);
    double aspect = double(m_width) / double(m_height);

    mat4d proj;
    proj.loadPerspective(FIELD_OF_VIEW, aspect, Z_NEAR, Z_FAR);
    OpenGLHelper::setProjectionMatrix(proj);
}

} // namespace GL
