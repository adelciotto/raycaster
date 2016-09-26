#include "input.h"

const KeyCode KeyUp = SDL_SCANCODE_UP;
const KeyCode KeyDown = SDL_SCANCODE_DOWN;
const KeyCode KeyLeft = SDL_SCANCODE_LEFT;
const KeyCode KeyRight = SDL_SCANCODE_RIGHT;
const KeyCode KeyEsc = SDL_SCANCODE_ESCAPE;
const int numKeys = 256;

static const uint8_t *keyStates;
static uint8_t *previousKeyStates = NULL;

void Input_Init() {
    keyStates = SDL_GetKeyboardState(NULL);
    previousKeyStates = malloc(sizeof(uint8_t) * numKeys);

    if (previousKeyStates == NULL) {
        fprintf(stderr, "Error: Malloc fail in Input_Init()\n");
        exit(EXIT_FAILURE);
    }
}

void Input_Destroy() {
    free(previousKeyStates);
}

bool Input_IsKeyDown(KeyCode code) {
    return keyStates[code];
}

bool Input_IsKeyUp(KeyCode code) {
    return !keyStates[code];
}

bool Input_IsKeyPressed(KeyCode code) {
    return !previousKeyStates[code] && keyStates[code];
}

void Input_PollEvent(SDL_Event *e) {
    keyStates = SDL_GetKeyboardState(NULL);

    switch (e->type) { } 
}

void Input_Update() {
    memcpy(previousKeyStates, keyStates, sizeof(uint8_t) * numKeys);
}
