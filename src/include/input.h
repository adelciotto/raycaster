#ifndef INPUT_H
#define INPUT_H

#include "common.h"

enum KeyCodes {
    keyUp = SDL_SCANCODE_UP,
    keyDown = SDL_SCANCODE_DOWN,
    keyLeft = SDL_SCANCODE_LEFT,
    keyRight = SDL_SCANCODE_RIGHT,
    keyEsc = SDL_SCANCODE_ESCAPE
};

class Input {
    const uint8_t *keyStates;
    std::unique_ptr<uint8_t[]> previousKeyStates;

public:
    Input();

    bool isKeyDown(KeyCodes key) const;
    bool isKeyUp(KeyCodes key) const;
    bool isKeyPressed(KeyCodes key) const;
    void pollEvent(SDL_Event *e);
    void update();
};

#endif // INPUT_H
