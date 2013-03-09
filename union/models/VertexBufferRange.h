#pragma once

namespace GL {

class VertexBufferRange
{
public:
    unsigned m_start;
    unsigned m_length;

    VertexBufferRange()
        : m_start(0)
        , m_length(0)
    {
    }

    VertexBufferRange(unsigned start, unsigned length)
        : m_start(start)
        , m_length(length)
    {
    }

    void init(unsigned start, unsigned length)
    {
        m_start = start;
        m_length = length;
    }
};

} // namespace GL
