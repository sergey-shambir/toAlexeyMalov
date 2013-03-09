#pragma once

namespace GL {

class FrameBuffer
{
public:
    FrameBuffer();
    FrameBuffer(const FrameBuffer &other);
    FrameBuffer &operator =(const FrameBuffer &other);
    ~FrameBuffer();

    void setWidth(unsigned width);
    void setHeight(unsigned height);

    unsigned width() const;
    unsigned height() const;
    int type() const;

    void enableDepthBuffer(bool enabled);

    void checkStatus() const; //! Throws std::runtime error if incomplete
    void initAndBind();
    void bind() const;
    static void unbind();

    void setupViewportAndProjection();

private:
    inline void ensureInitialized();

    unsigned m_id;
    unsigned m_width;
    unsigned m_height;

    bool m_useDepthBuffer;
    unsigned m_depthBuffer;
};

} // namespace GL
