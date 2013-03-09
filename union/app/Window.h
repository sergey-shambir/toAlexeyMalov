#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>
#include <functional>

class Window;
class DataWindow;
typedef std::function<void(Window const& win)> WindowAction;

class Window
{
public:
    Window();
    ~Window();

    /* свойства */
    const char *title() const;
    uint16_t width() const;
    uint16_t height() const;
    bool isFullscreen() const;

    /* поведение */
    void resetHandlers();
    void onResize(WindowAction const& action);
    void onDisplay(WindowAction const& action);

    /* команды */
    void setTitle(const char *title);
    void redisplay();
    //! При вызове выходит из полноэкранного режима
    void resize(uint16_t width, uint16_t height);
    void toggleFullscreen(bool enterFullscreen);

private:
    DataWindow *d;
};

#endif // WINDOW_H
