#ifndef PLAYER_H
#define PLAYER_H

#include "mathutils/vec2.h"

enum Direction { 
    forwards = 1,
    backwards = -1,
    left = 1,
    right = -1,
    none = 0
};

struct Player {
    mathutils::Vec2 position;
    mathutils::Vec2 direction;
    mathutils::Vec2 cameraPlane;
    mathutils::Vec2 velocity;
    double rotation;

    Player();
    Player(double startX, double startY, double fov);

    void move(Direction direction);
    void rotate(Direction direction);
    void update(double delta);
    bool isMovingForwards() const;
    bool isMovingBackwards() const;
};

#endif
