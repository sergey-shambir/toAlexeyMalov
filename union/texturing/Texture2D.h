#pragma once
#include <memory>

struct SDL_Surface;

namespace GL {
class FrameBuffer;

class Texture2D
{
    unsigned m_id;

public:
    Texture2D();
    Texture2D(const Texture2D &other);
    Texture2D& operator =(const Texture2D &other);
    ~Texture2D();

    void bind() const;
    void bindAnyway() const; //! performs unbinding if texture is null

    bool isNull() const;

    /**
     * @brief Use carefully or do not use
     * @return OpenGL id of texture resource or 0
     */
    unsigned resourceId() const;

    static Texture2D createFromImageByPath(const std::string &path);
    static Texture2D createFromDevILBinded();
    static Texture2D createLinkedWith(FrameBuffer &buffer);

private:
    static void bindId(unsigned id);
    static void useParameter(int parameter, int value);

    static unsigned createFromSDLSurface(const SDL_Surface &surface);
};

} // namespace GL
