#include "MouseButtonEvent.h"

MouseButtonEvent::MouseButtonEvent(const Window &window, MouseButton button, int x, int y)
    : m_window(window)
    , m_button(button)
    , m_x(x)
    , m_y(y)
{
}
