#ifndef EYECAMERAINPUTCONTROLLER_H
#define EYECAMERAINPUTCONTROLLER_H

#include "IInputController.h"
#include "IFirstPersonCamera.h"

#include <set>

class EyeCameraInputController : public IInputController
{
public:
    EyeCameraInputController(GL::IFirstPersonCamera &reference, float cameraSpeed);

    void pressedKey(const KeyboardEvent &);
    void releasedKey(const KeyboardEvent &);
    void movedMouse(const MouseMotionEvent &);
    void pressedMouse(const MouseButtonEvent &);
    void releasedMouse(const MouseButtonEvent &);

private:
    void safeWarpMouse();

    GL::IFirstPersonCamera &m_cameraRef;
    float m_cameraSpeed;

    /**
     * Pairs of coordinates that emitted by warping mouse to center,
     * thay should be ignored
     */
    std::multiset<std::pair<int, int> > m_blacklistedMoves;
};

#endif // EYECAMERAINPUTCONTROLLER_H
