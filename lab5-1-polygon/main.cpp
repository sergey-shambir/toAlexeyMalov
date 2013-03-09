#include <app/App.h>
#include "AppDelegate.h"

using namespace std;

int main(int argc, char *argv[])
{
    AppOptions options;
//    options.useTimer = false;
    App app(new AppDelegate(), options);

    app.exec(argc, argv);

    return 0;
}

