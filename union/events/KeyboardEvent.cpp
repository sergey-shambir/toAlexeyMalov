#include "KeyboardEvent.h"

KeyboardEvent::KeyboardEvent(const Window &window, SDL_Keycode key, Uint16 modifiers)
    : m_window(window)
    , m_key(key)
    , m_modifiers(modifiers)
{
}
