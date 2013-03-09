#include "MouseMotionEvent.h"

MouseMotionEvent::MouseMotionEvent(const Window &window, int x, int y, int dx, int dy)
    : m_window(window)
    , m_x(x)
    , m_y(y)
    , m_dx(dx)
    , m_dy(dy)
{
}
