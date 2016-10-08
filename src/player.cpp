#include "player.h"
#include "mathutils/mathutils.h"

static const float defaultStartX = 22.0f;
static const float defaultStartY = 11.5f;
static const float defaultFov = 66.0f;
static const float moveSpeed = 3.0f;
static const float rotationSpeed = 2.0f;

Player::Player()
    : position(defaultStartX, defaultStartY),
      direction(-1.0f, 0.0f),
      velocity(),
      rotation(0.0f) {
    float fovRadians = mathutils::toRadians(defaultFov);
    float focalLength = tan(fovRadians / 2.0f);

    cameraPlane.set(0, focalLength);
}

Player::Player(float startX, float startY, float fov) 
    : position(startX, startY),
      direction(-1.0f, 0.0f),
      velocity(),
      rotation(0.0f) { 
    float fovRadians = mathutils::toRadians(fov);
    float focalLength = tan(fovRadians / 2.0f);

    cameraPlane.set(0, focalLength);
}

void Player::move(Direction direction) {
    velocity.set(moveSpeed * direction);
}

void Player::rotate(Direction direction) {
    rotation = rotationSpeed * direction;
}

void Player::update(float delta) {
    direction.rotate(rotation * delta);
    cameraPlane.rotate(rotation * delta);
    position += direction * velocity * delta; 
}

bool Player::isMovingForwards() const {
    return velocity.x > 0.0f && velocity.y > 0.0f;
}

bool Player::isMovingBackwards() const {
    return velocity.x < 0.0f && velocity.y < 0.0f;
}
