#ifndef MAP_H
#define MAP_H

#include <vector>
#include "mathutils/vec2.h"
#include "texture.h"

struct Player;
class Graphics;

class Map {
public:
    Map();
    Map(Player *player, const std::string& file);

    void create(Player *player, const std::string& file);
    void update(double dt);
    void draw(Graphics& graphics);

private:
    Player *player;
    Texture textures[8];
    std::vector<uint8_t> worldMap;
    uint32_t mapWidth, mapHeight;
    mathutils::Vec2 rayPos;
    mathutils::Vec2 rayDir;
    int mapX, mapY;
    int side;

    void loadTextures();
    void loadFile(const std::string& file);
    double castRay(double cameraX);
    void resolveCollisions(double dt);
    void drawWall(Graphics& graphics, int x, double distance);
    void drawFlatColoredWall(Graphics& graphics, int x, int drawStart, int drawEnd);
    void drawTexturedWall(Graphics& graphics, int x, double distance, int lineHeight, int drawStart, int drawEnd);
};

#endif // MAP_H
