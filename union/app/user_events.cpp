#include "user_events.h"

SDL_Event createUserEvent(int code, void *data1, void *data2)
{
    SDL_Event event;
    event.type = SDL_USEREVENT;
    {
        SDL_UserEvent userEvent;
        userEvent.type = SDL_USEREVENT;
        userEvent.code = code;
        userEvent.data1 = data1;
        userEvent.data2 = data2;
        event.user = userEvent;
    }
    return event;
}
