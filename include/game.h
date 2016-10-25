#ifndef GAME_H
#define GAME_H

#include "window.h"
#include "graphics.h"
#include "input.h"

class Game {
public:
    Game(const std::string& settingsPath);
    virtual ~Game() { }

protected:
    struct Settings {
        bool fullscreen;
        int width;
        int height;
        std::string mapfile;
        bool vsync;
    };

    virtual void load() = 0;
    virtual void create(const Settings& settings) = 0;
    virtual void handleInput() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;

    Window window;
    Graphics graphics;
    Input input;

private:
    void init(const Settings& settings);
    Settings parseSettings(const std::string& settingsPath);
    void loop();

    bool isRunning = false;
    bool isPaused = false;
};

#endif // GAME_H
