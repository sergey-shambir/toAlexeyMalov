#include "AppDelegate.h"

int main(int argc, char* argv[])
{
    AppOptions options;
    options.title = "Lab9: image filters";
    options.useTimer = false;
    options.windowSize = std::pair<unsigned, unsigned>(1600u, 1000u);

    auto ad = new Lab9FilterAppDelegate();
    App app(ad, options);
    app.exec(argc, argv);

    return 0;
}
