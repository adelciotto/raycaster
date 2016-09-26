#include "timer.h"
#include "window.h"
#include "graphics.h"
#include "input.h"
#include "raycaster.h"

const int defaultWindowWidth = 640;
const int defaultWindowHeight = 480;

bool running = false;

struct arguments {
    bool fullscreen;
    int width;
    int height;
};

struct arguments parseCmdOpts(int argc, char *argv[]) {
    struct arguments args = { false, defaultWindowWidth, defaultWindowHeight };
    
    if (argc == 4) {
        args.width = Util_ParseInt(argv[1]);
        args.height = Util_ParseInt(argv[2]);
        args.fullscreen = strcmp(argv[3], "true") == 0 ? true : false;
    } 
    
    printf("Using w: %d, h: %d, fullscreen: %d\n", args.width, args.height, args.fullscreen);
    return args;
}

void input() {
    SDL_Event e;

    if (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
        }

        Input_PollEvent(&e);
    }

    if (Input_IsKeyPressed(KeyEsc)) {
        running = false;
    }

    Input_Update();
}

void update(float dt) {

}

void draw(Graphics *graphics) {
    Graphics_Clear(graphics, 0xFFFFFFFF);

    Graphics_DrawVertLine(graphics, 320/2, 0, graphics->bufferHeight, 0xFF0000FF);

    Graphics_Present(graphics);
}

void cleanup(Window *window, Graphics *graphics) {
    Input_Destroy();
    Graphics_Destroy(graphics);
    Window_Destroy(window);
    SDL_Quit();
}

int main(int argc, char **argv) {
    struct arguments args = parseCmdOpts(argc, argv);

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    Window window;
    Graphics graphics;
    Timer timer;

    Window_Init(&window, args.width, args.height, args.fullscreen);
    Graphics_Init(&graphics, &window);
    Input_Init();
    Raycaster_Init(66.0f);
    Timer_Init(&timer);

    running = true;
    while (running) {
        input();

        Timer_Step(&timer);
        update(timer.delta);

        Graphics_Clear(&graphics, 0xCCCCCCFF);
        for (int x = 0; x < graphics.bufferWidth; x++) {
            float cameraX = 2 * x / (float)graphics.bufferWidth - 1;
            float perpWallDist = Raycaster_CastRay(cameraX);
            /*printf("%.2f\n", perpWallDist);*/

            int h = graphics.bufferHeight;
            int lineHeight = (int)(h / perpWallDist);
            int drawStart = -lineHeight / 2 + h / 2;
            if (drawStart < 0) drawStart = 0;
            int drawEnd = lineHeight / 2 + h / 2;
            if (drawEnd >= h) drawEnd = h - 1;

            Graphics_DrawVertLine(&graphics, x, drawStart, drawEnd, 0xFF0000FF);
        }

        Graphics_Present(&graphics);
        /*draw(&graphics);*/

        Timer_Sleep(1);
    }

    cleanup(&window, &graphics);
    return EXIT_SUCCESS;
}
