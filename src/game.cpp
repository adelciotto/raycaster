#include <chrono>

#include "game.h"
#include "json_parser.h"

using namespace std::chrono_literals;

namespace {
    constexpr std::chrono::nanoseconds timestep(16ms);
}

Game::Game(const std::string& settingsPath) {
    auto settings = parseSettings(settingsPath);    
    init(settings);

    loop();
}

Game::Settings Game::parseSettings(const std::string& settingsPath) {
    JsonParser jsonParser(settingsPath);

    const int width = jsonParser.get<int>("width", 640);
    const int height = jsonParser.get<int>("height", 360);
    const std::string mapFile = jsonParser.get<std::string>("map", "assets/maps/small.json");
    const bool fullscreen = jsonParser.get<bool>("fullscreen", false);
    const bool vsync = jsonParser.get<bool>("vsync", true);
    
    logger::info("Using w: %d, h: %d, fullscreen: %d, vsync: %d\n", 
                 width, 
                 height,
                 fullscreen,
                 vsync
    );

    return Settings{fullscreen, width, height, mapFile, vsync};
}

void Game::init(const Settings& settings) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    // For now just wait until all resource threads have finished.
    load();

    window.create(settings.width, settings.height, settings.fullscreen);
    graphics.create(window, settings.vsync);
    create(settings);
    
    isRunning = true;
}

void Game::loop() {
    using clock = std::chrono::high_resolution_clock;

    std::chrono::nanoseconds lag(0ns);
    auto previousTime = clock::now();

    while (isRunning) {
        auto deltaTime = clock::now() - previousTime;
        previousTime = clock::now();
        lag += std::chrono::duration_cast<std::chrono::nanoseconds>(deltaTime);

        handleInput();

        while (lag >= timestep) {
            lag -= timestep;

            // TODO: set current state
            update();
        }

        // TODO: interpolate state
        auto alpha = (double) lag.count() / timestep.count();

        draw();
    }
}
