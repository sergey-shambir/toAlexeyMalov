#ifndef MOUSEMOTIONEVENT_H
#define MOUSEMOTIONEVENT_H

class Window;

class MouseMotionEvent
{
public:
    const Window &m_window; /**< Window where event occured */
    int m_x;	/**< The X/Y coordinates of the mouse */
    int m_y;
    int m_dx;	/**< The relative motion in the X direction */
    int m_dy;	/**< The relative motion in the Y direction */

    MouseMotionEvent(const Window &window, int x, int y, int dx, int dy);
};

#endif // MOUSEMOTIONEVENT_H
