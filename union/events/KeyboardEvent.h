#ifndef KEYBOARDEVENT_H
#define KEYBOARDEVENT_H

#include <SDL/SDL.h>

class Window;

class KeyboardEvent
{
public:
    const Window &m_window; /**< Window where event occured */
    SDLKey m_key;           /**< SDL virtual keysym */
    SDLMod m_modifiers;     /**< current key modifiers */

    KeyboardEvent(const Window &window, SDLKey key, SDLMod modifiers);
};

#endif // KEYBOARDEVENT_H
