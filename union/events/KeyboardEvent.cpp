#include "KeyboardEvent.h"

KeyboardEvent::KeyboardEvent(const Window &window, SDLKey key, SDLMod modifiers)
    : m_window(window)
    , m_key(key)
    , m_modifiers(modifiers)
{
}
