#ifndef IINPUTCONTROLLER_H
#define IINPUTCONTROLLER_H

class KeyboardEvent;
class MouseButtonEvent;
class MouseMotionEvent;

class IInputController
{
public:
    virtual ~IInputController() {}

    virtual void movedMouse(MouseMotionEvent const&)            { }
    virtual void pressedMouse(MouseButtonEvent const&)          { }
    virtual void releasedMouse(MouseButtonEvent const&)         { }
    virtual void pressedKey(KeyboardEvent const&)               { }
    virtual void releasedKey(KeyboardEvent const&)              { }
};

#endif // IINPUTCONTROLLER_H
