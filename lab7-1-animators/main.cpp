#include "AppDelegate.h"
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
    AppOptions options;
    options.title = "Animated scene with 3ds objects";
    options.windowSize = std::pair<int, int>(1200, 800);
    App app(new AppDelegate(), options);

    app.exec(argc, argv);

    return 0;
}
