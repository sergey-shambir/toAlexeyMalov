#include "VertexBuffer.h"
#include "../helpers/ResourceHeap.h"

#include <GL/glew.h>
#include <stdexcept>

namespace GL {

static unsigned bufferConstructor()
{
    if (!GLEW_VERSION_1_5)
    {
        throw std::runtime_error("ERROR: hardware requirement missed - OpenGL >= 1.5,"
                                 " update drivers or try run on another graphics card");
    }

    unsigned bufferId(0);
    glGenBuffers(1, &bufferId);
    return bufferId;
}

static void bufferDestructor(unsigned bufferId)
{
    glDeleteBuffers(1, &bufferId);
}

VertexBuffer::VertexBuffer()
    : m_id(0)
    , m_type(Type_Attributes)
    , m_usageHint(GL_STATIC_DRAW)
    , m_bytesCount(0)
{
}

VertexBuffer::VertexBuffer(const VertexBuffer &other)
    : m_id(other.m_id)
    , m_type(other.m_type)
    , m_usageHint(other.m_usageHint)
    , m_bytesCount(0)
{
    ResourceHeap::instance().retain(m_id, ResourceHeap::VertexBuffer);
}

VertexBuffer &VertexBuffer::operator =(const VertexBuffer &other)
{
    ResourceHeap::instance().retain(m_id, ResourceHeap::VertexBuffer);
    m_id = other.m_id;
    m_type = other.m_type;
    m_usageHint = other.m_usageHint;
    m_bytesCount = other.m_bytesCount;
    return *this;
}

VertexBuffer::~VertexBuffer()
{
    ResourceHeap::instance().release(m_id, ResourceHeap::VertexBuffer);
}

void VertexBuffer::ensureInitializated()
{
    if (m_id == 0) {
        m_id = ResourceHeap::instance().create(bufferConstructor,
                                               bufferDestructor,
                                               ResourceHeap::VertexBuffer);
    }
}

void VertexBuffer::setType(VertexBuffer::Type type)
{
    m_type = type;
}

void VertexBuffer::setUsageHint(int usageHint)
{
    m_usageHint = usageHint;
}

void VertexBuffer::init(const void *data, unsigned bytesCount)
{
    ensureInitializated();
    m_bytesCount = bytesCount;

    GLenum target = (m_type == Type_Attributes)
            ? GL_ARRAY_BUFFER
            : GL_ELEMENT_ARRAY_BUFFER;
    glBindBuffer(target, m_id);
    glBufferData(target, bytesCount, data, m_usageHint);
}

void VertexBuffer::bind() const
{
    GLenum target = (m_type == Type_Attributes)
            ? GL_ARRAY_BUFFER
            : GL_ELEMENT_ARRAY_BUFFER;
    glBindBuffer(target, m_id);
}

void VertexBuffer::unbind() const
{
    GLenum target = (m_type == Type_Attributes)
            ? GL_ARRAY_BUFFER
            : GL_ELEMENT_ARRAY_BUFFER;
    glBindBuffer(target, 0);
}

void VertexBuffer::unbind(Type type)
{
    GLenum target = (type == Type_Attributes)
            ? GL_ARRAY_BUFFER
            : GL_ELEMENT_ARRAY_BUFFER;
    glBindBuffer(target, 0);
}

unsigned VertexBuffer::bytesCount() const
{
    return m_bytesCount;
}

} // namespace GL
