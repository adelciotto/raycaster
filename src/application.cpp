#include <fstream>
#include "application.h"
#include "json_parser.h"
#include "profile_timer.h"
#include "picojson.h"

bool Application::instantiated = false;

Application::Application(int argc, char **argv) 
    : running(false) {
    assert(!instantiated);
    instantiated = true;

    const Settings settings = parseSettings(); 

    create(settings);
    loop();
}

Settings Application::parseSettings() {
    JsonParser jsonParser("settings.json");

    const int width = (int)jsonParser.get<double>("width", 640);
    const int height = (int)jsonParser.get<double>("height", 360);
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

void Application::create(const Settings& settings) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    window.create(settings.width, settings.height, settings.fullscreen);
    graphics.create(window, settings.vsync);
    map.create(&player, settings.mapFile);
    running = true;
}

void Application::processEvents() {
    SDL_Event e;

    if (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
        }

        if (e.type == SDL_WINDOWEVENT) {
            if (e.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
                time.pause();
                logger::info("Game loop paused");
            }
            if (e.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
                time.resume();
                logger::info("Game loop resumed");
            }
        }

        input.pollEvent(&e);
    }
}

void Application::update(double dt) {
    if (input.isKeyPressed(KeyCodes::keyEsc)) {
        running = false;
    }

    if (input.isKeyDown(KeyCodes::keyUp)) {
        player.move(Direction::forwards);
    } else if (input.isKeyDown(KeyCodes::keyDown)) {
        player.move(Direction::backwards);
    } else {
        player.move(Direction::none);
    }

    if (input.isKeyDown(KeyCodes::keyLeft)) {
        player.rotate(Direction::left);
    } else if (input.isKeyDown(KeyCodes::keyRight)) {
        player.rotate(Direction::right);
    } else {
        player.rotate(Direction::none);
    }
    input.update();

    map.update(dt);
}

void Application::draw() {
    map.draw(graphics);
}

void Application::loop() {
    ProfileTimer frameTimer("frametime");
    auto printStats = [&](double fps) -> void {
        // Print the FPS.
        std::string text = utils::stringFormat("fps: %.2f", fps);
        int nextY = graphics.drawString(text, 10, 10, 0xFFFFFFFF, true, 0xFF00FFFF);

        // Print the frametime.
        nextY = frameTimer.printDelta(graphics, 10, nextY, 0xFFFFFFFF, 0xFF00FFFF);

        // Print the player position.
        text = utils::stringFormat("position: %s", player.position.toString().c_str());
        nextY = graphics.drawString(text, 10, nextY, 0xFFFFFFFF, true, 0x00FF00FF);

        // Print the player position.
        text = utils::stringFormat("direction: %s", player.direction.toString().c_str());
        graphics.drawString(text, 10, nextY, 0xFFFFFFFF, true, 0x00FF00FF);
    };

    while (running) {
        time.start();

        processEvents();

        // Right now this is a simple game loop that uses a variable time-step.
        // It's used to maintain consistent game speeds across varying framerates.
        // It is not the best solution, but for now lets keep it simple.
        // VSync is on by default to keep the framerate locked to the vertical refresh rate
        // of the users monitor. When vsync is not available, CoreTimer::delay can be used to
        // manually cap it.
        time.step();
        double delta = time.getDelta();

        if (!time.isPaused()) {
            frameTimer.start();
            update(delta);
            draw();
            frameTimer.end();

            printStats(time.getFPS());
            graphics.present();
        }

        time.sleep(1);
        // time.delay(); Use to manually cap the framerate to 60fps.
    }

    SDL_Quit();
}
