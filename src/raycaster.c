#include "raycaster.h"
#include "mathutil.h"
#include "common.h"

// TODO: For now have this here.
typedef struct {
    Vec2 position;
    Vec2 direction;
    Vec2 plane;
} Player;

const int initialX = 22;
const int initialY = 12;
const int worldMap[24][24] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// TODO: For now define these here.
static Player player;

void Raycaster_Init(float fov) {
    float fovRadians = MathUtil_ToRadians(fov);
    float focalLength = tan(fovRadians / 2.0f);

    // TODO: for now do this here.
    Vec2_Set(&player.position, initialX, initialY);
    Vec2_Set(&player.direction, -1, 0);
    Vec2_Set(&player.plane, 0, focalLength);
}

float Raycaster_CastRay(float cameraX) {
    Vec2 rayPos, rayDir;
    
    Vec2_Set(&rayPos, player.position.x, player.position.y);
    Vec2_Set(
        &rayDir, 
        player.direction.x + player.plane.x * cameraX,
        player.direction.y + player.plane.y * cameraX
    );

    int mapX = (int)rayPos.x;
    int mapY = (int)rayPos.y;
    float deltaDistX = sqrt(1 + (rayDir.y*rayDir.y) / (rayDir.x*rayDir.x));
    float deltaDistY = sqrt(1 + (rayDir.x*rayDir.x) / (rayDir.y*rayDir.y));
    
    float sideDistX, sideDistY;
    int stepX, stepY;

    if (rayDir.x < 0) {
        stepX = -1;
        sideDistX = (rayPos.x - mapX) * deltaDistX;
    } else {
        stepX = 1;
        sideDistX = (mapX + 1.0f - rayPos.x) * deltaDistX;
    }
    if (rayDir.y < 0) {
        stepY = -1;
        sideDistY = (rayPos.y - mapY) * deltaDistY;
    } else {
        stepY = 1;
        sideDistY = (mapY + 1.0f - rayPos.y) * deltaDistY;
    }

    bool hit = false;
    bool xSide = true;

    while (!hit) {
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            mapX += stepX;
            xSide = true;
        } else {
            sideDistY += deltaDistY;
            mapY += stepY;
            xSide = false;
        }

        if (worldMap[mapX][mapY] > 0) {
            hit = true;
        }
    }
    
    if (xSide) {
        return (mapX - rayPos.x + (1 - stepX) / 2) / rayDir.x;   
    } else {
        return (mapY - rayPos.y + (1 - stepY) / 2) / rayDir.y;   
    }
}
