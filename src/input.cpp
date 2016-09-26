#include "input.h"

const int numKeys = 256;

Input::Input() 
    : keyStates(nullptr),
      previousKeyStates(std::unique_ptr<uint8_t[]>(new uint8_t[numKeys])) { }

bool Input::isKeyDown(KeyCodes key) const {
    return keyStates[key];
}

bool Input::isKeyUp(KeyCodes key) const {
    return !keyStates[key];
}

bool Input::isKeyPressed(KeyCodes key) const {
    return !previousKeyStates[key] && keyStates[key];
}

void Input::pollEvent(SDL_Event *e) {
    keyStates = SDL_GetKeyboardState(NULL);

    switch (e->type) { } 
}

void Input::update() {
    std::memcpy(previousKeyStates.get(), keyStates, sizeof(uint8_t) * numKeys);
}
