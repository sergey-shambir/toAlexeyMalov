#include <app/App.h>
#include <iostream>
#include "appdelegate.h"

using namespace std;

int main(int argc, char *argv[])
{
    std::string pathToImage = "../../Data/maze/default.png";
    if (argc > 2) {
        cout << "Usage:\n"
                "  ./program <path-to-image-with-maze>\n"
                "or just\n"
                "  ./program"
             << endl;
    } else if (argc == 2) {
        pathToImage = argv[1];
    }

    AppOptions options;
    App app(new AppDelegate(pathToImage), options);

    app.exec(argc, argv);

    return 0;
}

