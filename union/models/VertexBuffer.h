#pragma once
#include "VertexBufferRange.h"

namespace GL {

class VertexBuffer
{
public:
    VertexBuffer();
    VertexBuffer(const VertexBuffer& other);
    VertexBuffer& operator =(const VertexBuffer& other);
    ~VertexBuffer();

    enum Type {
        Type_Attributes, // GL_ARRAY_BUFFER
        Type_Indicies // GL_ELEMENT_ARRAY_BUFFER
    };

    /**
     * @brief Sets type that affects next init() call
     * @param type [in] - buffer type, default is Type_Attributes
     */
    void setType(Type type);

    /**
     * @brief Sets usage hint, default is GL_STATIC_DRAW
     * @param usageHint - GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY,
     * GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW,
     * GL_DYNAMIC_READ, or GL_DYNAMIC_COPY
     */
    void setUsageHint(int usageHint);

    /**
     * @brief Sets buffer data with current usage hint and buffer type
     * @param data - pointer to data or null if no data needed on initialization
     * @param bytesCount - length of data in bytes
     */
    void init(const void *data, unsigned bytesCount);

    void bind() const;
    void unbind() const;
    static void unbind(Type type);

    unsigned bytesCount() const;

private:
    inline void ensureInitializated();

    unsigned m_id;
    Type     m_type;
    int      m_usageHint;
    unsigned m_bytesCount;
};

} // namespace GL
