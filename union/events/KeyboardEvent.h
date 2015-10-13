#ifndef KEYBOARDEVENT_H
#define KEYBOARDEVENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>

class Window;

class KeyboardEvent
{
public:
    const Window &m_window; /**< Window where event occured */
    SDL_Keycode m_key;           /**< SDL virtual keysym */
    Uint16 m_modifiers;     /**< current key modifiers */

    KeyboardEvent(const Window &window, SDL_Keycode key, Uint16 modifiers);
};

#endif // KEYBOARDEVENT_H
