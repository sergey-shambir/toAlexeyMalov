#pragma once

#include <SDL2/SDL.h>

/**
 * Integer codes for SDL user events
 */
namespace UserEvents {
const int EVENT_REDISPLAY = 1001;
}

SDL_Event createUserEvent(int code, void *data1 = nullptr, void* data2 = nullptr);
