#ifndef APPOPTIONS_H
#define APPOPTIONS_H

#include <string>

class AppOptions
{
public:
    /**
     * True if you want to use timer to emit redisplay events, turns of
     * redisplaying on input events
     */
    bool useTimer;

    bool fullScreen;

    /**
     * Framerate determines FPS when using timer
     * or minimal pause between redisplay events otherwise
     */
    unsigned framerate;

    /**
     * Width and height
     */
    std::pair<unsigned, unsigned> windowSize;

    std::string title;

    /**
     * Constructs with default options, shared for all my works
     */
    AppOptions()
        : useTimer(true)
        , fullScreen(false)
        , framerate(40)
        , windowSize(800, 600)
        , title("Sample application | Сергей Шамбир")
    {
    }
};

#endif // APPOPTIONS_H
