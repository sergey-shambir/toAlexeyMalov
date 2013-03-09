
#include <stdafx.h>
#include "D21AppDelegate.h"
#include <app/user_events.h>

////////////////////////////////////////////////////////////////////////////////

enum class State {
    Undefined,
    Win,
    Lose
};

bool man_who_achieves_1_1_wins = false;
static unsigned frame_interval = 50;
static unsigned field_width = 8;
static unsigned field_height = 7;

////////////////////////////////////////////////////////////////////////////////

static void postRedisplay()
{
    SDL_Event event = createUserEvent(UserEvents::EVENT_REDISPLAY);
    SDL_PushEvent(&event);
}

class Field
{
public:
    unsigned width;
    unsigned height;
    std::vector<State> field;
    DynamicGameAppDelegate &delegate;

    Field(unsigned width, unsigned height, DynamicGameAppDelegate &delegate)
        : width(width), height(height), delegate(delegate)
    {
        field.resize(width * height, State::Undefined);
    }

    State get(unsigned x, unsigned y) const
    {
        return field[index(x, y)];
    }

    void set(unsigned x, unsigned y, State state)
    {
        field[index(x, y)] = state;
        if (state == State::Win)
            delegate.setCellState(x, y, true);
        else if (state == State::Lose)
            delegate.setCellState(x, y, false);
    }

    unsigned index(unsigned x, unsigned y) const
    {
        return x + y * width;
    }
};

class D21Resolver
{
public:
    SDL_TimerID timerId;
    DynamicGameAppDelegate &delegate;
    Field field;

    D21Resolver(const DynamicGameOpts &opts, D21AppDelegate &delegate)
        : delegate(delegate)
        , field(opts.width, opts.height, delegate)
    {
    }

    bool resolve()
    {
        for (unsigned x = 0; x < field.width; ++x)
            for (unsigned y = 0; y < field.height; ++y) {
                if (field.get(x, y) == State::Undefined) {
                    if (x == 0 && y == 0) {
                        field.set(0, 0, man_who_achieves_1_1_wins ? State::Lose : State::Win);
                    } else if (x == 0) {
                        field.set(x, y, State::Lose);
                        for (unsigned newy = 0; newy < y; ++newy)
                            if (field.get(x, newy) == State::Lose)
                                field.set(x, y, State::Win);
                    } else if (y == 0) {
                        field.set(x, y, State::Lose);
                        for (unsigned newx = 0; newx < x; ++newx)
                            if (field.get(newx, y) == State::Lose)
                                field.set(x, y, State::Win);
                    } else {
                        field.set(x, y, State::Lose);
                        for (unsigned newy = 0; newy < y; ++newy)
                            if (field.get(x, newy) == State::Lose)
                                field.set(x, y, State::Win);
                        for (unsigned newx = 0; newx < x; ++newx)
                            if (field.get(newx, y) == State::Lose)
                                field.set(x, y, State::Win);
                    }
                    return true;
                }
            }

        return false;
    }
};

Uint32 timerFunc(Uint32 interval, void* resolverPtr)
{
    D21Resolver &resolver = *reinterpret_cast<D21Resolver*>(resolverPtr);
    if (!resolver.resolve())
        SDL_RemoveTimer(resolver.timerId);
    else
        postRedisplay();

    return interval;
}

static DynamicGameOpts d21Options()
{
    DynamicGameOpts opts;
    opts.width = field_width;
    opts.height = field_height;
    return opts;
}

D21AppDelegate::D21AppDelegate()
    : DynamicGameAppDelegate(d21Options())
    , d(new D21Resolver(d21Options(), *this))
{
}

D21AppDelegate::~D21AppDelegate()
{
    delete d;
}

void D21AppDelegate::didInitialized(const Window &window)
{
    DynamicGameAppDelegate::didInitialized(window);
//    d->resolve();
    d->timerId = SDL_AddTimer(frame_interval, timerFunc, d);
}
