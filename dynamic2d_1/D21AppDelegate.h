#pragma once
#include <stdafx.h>

class D21Resolver;

class D21AppDelegate : public DynamicGameAppDelegate
{
public:
    D21AppDelegate();
    ~D21AppDelegate();

    void didInitialized(const Window &window);

private:
    D21Resolver *d;
};
