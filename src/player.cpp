#include "player.h"
#include "mathutils/mathutils.h"

namespace {
    const double defaultStartX = 22.0;
    const double defaultStartY = 11.5;
    const double defaultFov = 66.0;
    const double moveSpeed = 3.0;
    const double rotationSpeed = 2.0;
}

Player::Player()
    : position(defaultStartX, defaultStartY),
      direction(-1.0, 0.0),
      velocity(),
      rotation(0.0) {
    double fovRadians = mathutils::toRadians(defaultFov);
    double focalLength = tan(fovRadians / 2.0);

    cameraPlane.set(0, focalLength);
}

Player::Player(double startX, double startY, double fov)
    : position(startX, startY),
      direction(-1.0, 0.0),
      velocity(),
      rotation(0.0) {
    double fovRadians = mathutils::toRadians(fov);
    double focalLength = tan(fovRadians / 2.0);

    cameraPlane.set(0, focalLength);
}

void Player::move(Direction direction) {
    velocity.set(moveSpeed * direction);
}

void Player::rotate(Direction direction) {
    rotation = rotationSpeed * direction;
}

void Player::update() {
    direction.rotate(rotation);
    cameraPlane.rotate(rotation);
    position += direction * velocity; 
}

bool Player::isMovingForwards() const {
    return velocity.x > 0.0 && velocity.y > 0.0;
}

bool Player::isMovingBackwards() const {
    return velocity.x < 0.0 && velocity.y < 0.0;
}
