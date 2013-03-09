
/**
 * Simple visualization made for kids.
 */

#include <stdafx.h>
#include "D21AppDelegate.h"

using namespace std;

int main(int argc, char* argv[])
{
    AppOptions options;
    options.title = "Math - Dynamic Programming";
    options.windowSize = std::pair<unsigned, unsigned>(1600u, 1000u);
    options.useTimer = true;
    options.fullScreen = true;

    auto ad = new D21AppDelegate();
    App app(ad, options);
    app.exec(argc, argv);

    return 0;
}

