#ifndef EYECAMERAINPUTCONTROLLER_H
#define EYECAMERAINPUTCONTROLLER_H

#include "IInputController.h"
#include "IFirstPersonCamera.h"

#include <set>

class Window;

class EyeCameraInputController : public IInputController
{
public:
    EyeCameraInputController(const Window &window, GL::IFirstPersonCamera &reference, float cameraSpeed);

    void pressedKey(const KeyboardEvent &);
    void releasedKey(const KeyboardEvent &);
    void movedMouse(const MouseMotionEvent &);
    void pressedMouse(const MouseButtonEvent &);
    void releasedMouse(const MouseButtonEvent &);

private:
    void safeWarpMouse();
    void updateCameraSpeed();

    const Window &m_window;
    GL::IFirstPersonCamera &m_cameraRef;
    float m_cameraSpeed;
    struct MoveState
    {
        bool keyW = false;
        bool keyA = false;
        bool keyS = false;
        bool keyD = false;
        bool left = false;
        bool right = false;
        bool forward = false;
        bool backward = false;
    } m_moveState;

    /**
     * Pairs of coordinates that emitted by warping mouse to center,
     * thay should be ignored
     */
    std::multiset<std::pair<int, int> > m_blacklistedMoves;
};

#endif // EYECAMERAINPUTCONTROLLER_H
