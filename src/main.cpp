#include "coretimer.h"
#include "timer.h"
#include "window.h"
#include "graphics.h"
#include "input.h"
#include "map.h"
#include "player.h"
#include "input_parser.h"

bool running = false;

struct CmdArguments {
    bool fullscreen;
    int width;
    int height;
    std::string mapFile;
    bool novsync;
};

CmdArguments parseCmdOpts(const InputParser& inputParser) {
    const int width = std::stoi(inputParser.getOption("-width", "640"));
    const int height = std::stoi(inputParser.getOption("-height", "360"));
    const std::string mapFile = inputParser.getOption("-map", "assets/maps/small.txt");
    const bool fullscreen = inputParser.optionExists("-fullscreen");
    const bool novsync = inputParser.optionExists("-novsync");
    
    logger::info("Using w: %d, h: %d, fullscreen: %d, vsync: %d\n", 
                 width, 
                 height,
                 fullscreen,
                 !novsync
    );
    return CmdArguments{fullscreen, width, height, mapFile, novsync};
}

void processEvents(Input& input) {
    SDL_Event e;

    if (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
        }

        input.pollEvent(&e);
    }
}

void update(float dt, Input& input, Player& player, Map& map) {
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

void draw(Graphics& graphics, Map& map) {
    map.draw(graphics);

    graphics.present();
}

int main(int argc, char **argv) {
    InputParser inputParser(argc, argv);
    const auto args = parseCmdOpts(inputParser);

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    Window window(args.width, args.height, args.fullscreen);
    Graphics graphics(window, !args.novsync);
    Input input;
    Player player(7, 14, 66.0f);
    Map map(&player, args.mapFile);
    CoreTimer timer;
    Timer renderTimer;

    running = true;
    while (running) {
        processEvents(input);

        timer.step();
        update(timer.getDelta(), input, player, map);

        renderTimer.start();
        draw(graphics, map);
        float renderTime = renderTimer.end();

        renderTimer.print(1000, "render ms: %f", renderTime);

        timer.sleep(1);
    }

    SDL_Quit();
    return EXIT_SUCCESS;
}
