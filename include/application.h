#ifndef APPLICATION_H
#define APPLICATION_H

#include "window.h"
#include "graphics.h"
#include "input.h"
#include "player.h"
#include "map.h"
#include "coretimer.h"

class InputParser;

namespace {
    struct Settings {
        bool fullscreen;
        int width;
        int height;
        std::string mapFile;
        bool vsync;
    };
};

// This is a single instance only class that encapsulates all
// the creation / deletion of all the main required resources for
// this program. It also contains the game loop.
class Application {
public:
    Application(int argc, char **argv);

private:
    static bool instantiated;

    Window window; 
    Graphics graphics;
    Input input;
    Player player;
    Map map;
    CoreTimer time;
    bool running;

    Settings parseSettings();
    void create(const Settings& settings);
    void processEvents();
    void update(double dt);
    void draw();
    void loop();
};

#endif // APPLICATION_H

