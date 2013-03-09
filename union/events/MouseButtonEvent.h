#ifndef MOUSEBUTTONEVENT_H
#define MOUSEBUTTONEVENT_H

class Window;

enum class MouseButton
{
    LEFT,
    MIDDLE,
    RIGHT,
    EXTRA
};

class MouseButtonEvent
{
public:
    const Window &m_window; /**< Window where event occured */
    MouseButton m_button;	/**< The mouse button index */
    int m_x;	/**< The X/Y coordinates of the mouse */
    int m_y;

    MouseButtonEvent(const Window &window, MouseButton button, int x, int y);
};

#endif // MOUSEBUTTONEVENT_H
