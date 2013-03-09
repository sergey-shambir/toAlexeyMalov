#include "RenderBuffer.h"
#include "FrameBuffer.h"
#include "../helpers/ResourceHeap.h"
#include "GL/glew.h"

namespace GL {

static unsigned renderBufferCtor()
{
    unsigned id = 0;
    glGenRenderbuffersEXT(1, &id);
    return id;
}

static void renderBufferDtor(unsigned value)
{
    glDeleteRenderbuffersEXT(1, &value);
}

RenderBuffer::RenderBuffer() :
    m_id(0)
{
}

RenderBuffer::RenderBuffer(const RenderBuffer &other)
    : m_id(other.m_id)
{
    ResourceHeap::instance().retain(m_id, ResourceHeap::RenderBufferExt);
}

RenderBuffer &RenderBuffer::operator =(const RenderBuffer &other)
{
    m_id = other.m_id;
    ResourceHeap::instance().retain(m_id, ResourceHeap::RenderBufferExt);
    return *this;
}

RenderBuffer::~RenderBuffer()
{
    ResourceHeap::instance().release(m_id, ResourceHeap::RenderBufferExt);
}

int RenderBuffer::type() const
{
    return GL_RENDERBUFFER_EXT;
}

void RenderBuffer::bind()
{
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_id);
}

RenderBuffer RenderBuffer::createLinkedWith(FrameBuffer &buffer)
{
    RenderBuffer ret;
    ret.ensureInitialized();
    ret.bind();

    glRenderbufferStorageEXT(
                ret.type(),
                GL_DEPTH_COMPONENT,
                buffer.width(),
                buffer.height()
                );
    glFramebufferRenderbufferEXT(
                buffer.type(),
                GL_DEPTH_ATTACHMENT_EXT,
                ret.type(),
                ret.m_id
                );

    return ret;
}

void RenderBuffer::ensureInitialized()
{
    if (m_id == 0) {
        m_id = ResourceHeap::instance().create(renderBufferCtor,
                                               renderBufferDtor,
                                               ResourceHeap::RenderBufferExt);
    }
}

} // namespace GL
