#pragma once

namespace GL {
class FrameBuffer;

class RenderBuffer
{
public:
    RenderBuffer();
    RenderBuffer(const RenderBuffer& other);
    RenderBuffer& operator =(const RenderBuffer& other);
    ~RenderBuffer();

    int type() const;

    void bind();

    static RenderBuffer createLinkedWith(FrameBuffer &buffer);

private:
    void ensureInitialized();

    unsigned m_id;
};

} // namespace GL
