#include "coretimer.h"
#include "timer.h"
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

static void processEvents(Input& input) {
    SDL_Event e;

    if (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
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

static void printStats(Graphics& graphics, float fps, float frameTime) {
    int nextY = graphics.drawString("debug stats", 10, 10, 0xFFFFFFFF, true, 0xFF00FFFF);

    // Print the FPS.
    const std::string fpsText = utils::stringFormat("fps: %.2f", fps);
    nextY = graphics.drawString(fpsText, 10, nextY, 0xFFFFFFFF, true, 0xFF00FFFF);

    // Print the frametime.
    const std::string ftText = utils::stringFormat("frametime: %.2f", frameTime);
    nextY = graphics.drawString(ftText, 10, nextY, 0xFFFFFFFF, true, 0xFF00FFFF);
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
    Timer frameTimer;

    running = true;
    while (running) {
        processEvents(input);

        time.step();
        frameTimer.start();
        update(time.getDelta(), input, player, map);

        draw(graphics, map);
        float frameTime = frameTimer.end();

        printStats(graphics, time.getFPS(), frameTime);
        graphics.present();

        time.sleep(1);
    }

    SDL_Quit();
    return EXIT_SUCCESS;
}
