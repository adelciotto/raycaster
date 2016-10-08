#include "coretimer.h"
#include "profile_timer.h"
#include "window.h"
#include "graphics.h"
#include "input.h"
#include "map.h"
#include "player.h"
#include "input_parser.h"

static bool running = false;

namespace {
    struct Settings {
        bool fullscreen;
        int width;
        int height;
        std::string mapFile;
        bool novsync;
    };
};

static Settings parseCmdOpts(const InputParser& inputParser) {
    const int width = std::stoi(inputParser.getOption("-width", "1280"));
    const int height = std::stoi(inputParser.getOption("-height", "720"));
    const std::string mapFile = inputParser.getOption("-map", "assets/maps/small.txt");
    const bool fullscreen = inputParser.optionExists("-fullscreen");
    const bool novsync = inputParser.optionExists("-novsync");
    
    logger::info("Using w: %d, h: %d, fullscreen: %d, vsync: %d\n", 
                 width, 
                 height,
                 fullscreen,
                 !novsync
    );
    return Settings{fullscreen, width, height, mapFile, novsync};
}

static void processEvents(Input& input, CoreTimer& time) {
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

static void update(float dt, Input& input, Player& player, Map& map) {
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

static void draw(Graphics& graphics, Map& map) {
    map.draw(graphics);
}

static void printStats(Graphics& graphics, Player& player, float fps, ProfileTimer& frameTimer) {
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
}

int main(int argc, char **argv) {
    InputParser inputParser(argc, argv);
    const auto args = parseCmdOpts(inputParser);

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    Window window(args.width, args.height, args.fullscreen);
    Graphics graphics(window, !args.novsync);
    Input input;
    Player player(22, 11.5, 66.0f);
    Map map(&player, args.mapFile);
    CoreTimer time;
    ProfileTimer frameTimer("frametime");

    running = true;
    while (running) {
        time.start();

        processEvents(input, time);
        time.step();
        float delta = time.getDelta();

        if (!time.isPaused()) {
            frameTimer.start();
            update(delta, input, player, map);

            draw(graphics, map);
            frameTimer.end();

            printStats(graphics, player, time.getFPS(), frameTimer);
        }

        graphics.present();

        time.sleep(1);
        // time.delay(); Use to manually cap the framerate to 60fps.
    }

    SDL_Quit();
    return EXIT_SUCCESS;
}
