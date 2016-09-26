#ifndef INPUT_H

#include "common.h"

typedef int KeyCode;

extern const KeyCode KeyUp;
extern const KeyCode KeyDown;
extern const KeyCode KeyLeft;
extern const KeyCode KeyRight;
extern const KeyCode KeyEsc;

void Input_Init();
void Input_Destroy();
bool Input_IsKeyDown(KeyCode code);
bool Input_IsKeyUp(KeyCode code);
bool Input_IsKeyPressed(KeyCode code);
void Input_PollEvent(SDL_Event *e);
void Input_Update();

#endif // INPUT_H
