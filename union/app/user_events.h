#ifndef USER_EVENTS_H
#define USER_EVENTS_H

#include <SDL/SDL.h>

/**
 * Integer codes for SDL user events
 */
namespace UserEvents {
const int EVENT_REDISPLAY = 1001;
}

SDL_Event createUserEvent(int code, void *data1 = nullptr, void* data2 = nullptr);

#endif // USER_EVENTS_H
