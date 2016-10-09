#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include "map.h"
#include "player.h"
#include "graphics.h"
#include "color.h"
#include "texture.h"
#include "json_parser.h"

static const int defaultMapWidth = 24;
static const int defaultMapHeight = 24;
static const int defaultWorldMap[defaultMapWidth * defaultMapHeight] = {
    4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7,
    4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,
    4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,
    4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7,
    4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7,
    4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1,
    4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8,
    4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1,
    4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8,
    4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1,
    4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1,
    6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6,
    8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6,
    4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3,
    4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2,
    4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2,
    4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2,
    4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2,
    4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2,
    4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2,
    4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2,
    4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3
};
static const int collisionOffset = 4;

Map::Map()
    : player(nullptr),
      mapX(0),
      mapY(0) { }

Map::Map(Player *player, const std::string& file) {
    create(player, file);
}

void Map::create(Player *p, const std::string& file) {
    player = p;
    mapX = mapY = 0;

    loadTextures();
    loadFile(file);
}

void Map::loadTextures() {
    textures[0].fromFile("assets/textures/texture-0.png");
    textures[1].fromFile("assets/textures/texture-1.png");
    textures[2].fromFile("assets/textures/texture-2.png");
    textures[3].fromFile("assets/textures/texture-3.png");
    textures[4].fromFile("assets/textures/texture-4.png");
    textures[5].fromFile("assets/textures/texture-5.png");
    textures[6].fromFile("assets/textures/texture-6.png");
    textures[7].fromFile("assets/textures/texture-7.png");
}

void Map::loadFile(const std::string& file) {
    JsonParser jsonParser(file);

    const float startX = jsonParser.get<double>("startX", player->position.x);
    const float startY = jsonParser.get<double>("startY", player->position.y);
    const int width = (int)jsonParser.get<double>("width", defaultMapWidth);
    const int height = (int)jsonParser.get<double>("height", defaultMapHeight);
    const auto data = jsonParser.get<picojson::array>("data", picojson::array());
    auto loadDefaultMap = [&]() -> void {
        logger::warn("%s could not be loaded, using default map\n", file.c_str());
        mapWidth = 24;
        mapHeight = 24;
        worldMap = std::vector<uint8_t>(std::begin(defaultWorldMap), std::end(defaultWorldMap));
    };

    worldMap.reserve(width*height);
    for (const auto& v : data) {
        if (v.is<double>()) {
            worldMap.push_back((uint8_t)v.get<double>());
        } else {
            loadDefaultMap();
            return;
        }
    }

    if (worldMap.size() !=  size_t(width*height)) {
        logger::warn("invalid map data for %s\n", file.c_str());
        loadDefaultMap();
        return;
    }

    mapWidth = width;
    mapHeight = height;
    player->position.set(startX, startY);
    logger::info("Map %s loaded with w: %d, h: %d\n", file.c_str(), mapWidth, mapHeight);
}

void Map::update(float delta) {
    resolveCollisions(delta);
    player->update(delta);
}

void Map::resolveCollisions(float delta) {
    const auto vel = player->velocity;
    const auto pos = player->position;
    const auto dir = player->direction;
    auto checkWall = [&](const mathutils::Vec2& heading) -> void {
        if (worldMap[mapWidth * int(heading.x) + int(pos.y)]) player->velocity.x = 0;
        if (worldMap[mapWidth * int(pos.x) + int(heading.y)]) player->velocity.y = 0;
    };

    // Really simple player-wall collision.
    if (player->isMovingForwards()) {
        const auto headingPos = pos + dir * vel*delta*collisionOffset;

        checkWall(headingPos);
    }
    if (player->isMovingBackwards()) {
        const auto headingPos = pos - dir * vel*delta*-collisionOffset;

        checkWall(headingPos);
    }
}

void Map::draw(Graphics& graphics) {
    const int w = graphics.width();
    const int halfH = graphics.height() >> 1;

    // Draw the ceiling. For now the ceiling and floor are simply flat
    // color rectangles drawn before the walls.
    graphics.drawRect(0, 0, w, halfH, 0x38699DFF);

    // Draw the floor.
    graphics.drawRect(0, halfH, w, halfH, 0x606060FF);

    // For each vertical column in the screen buffer.
    for (int x = 0; x < w; x++) {
        float cameraX = 2 * x / (float)w - 1;
        float distance = castRay(cameraX);

        drawWall(graphics, x, distance);
    } 
}

float Map::castRay(float cameraX) {
    rayPos.set(player->position);
    rayDir.set(player->direction + player->cameraPlane * cameraX);

    mapX = (int)rayPos.x;
    mapY = (int)rayPos.y;

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

    while (!hit) {
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            mapX += stepX;
            side = 0;
        } else {
            sideDistY += deltaDistY;
            mapY += stepY;
            side = 1;
        }

        if (worldMap[mapWidth * mapX + mapY] > 0) {
            hit = true;
        }
    }
    
    if (side == 0) {
        return (mapX - rayPos.x + (1 - stepX) / 2) / rayDir.x;   
    } else {
        return (mapY - rayPos.y + (1 - stepY) / 2) / rayDir.y;   
    }
}

void Map::drawWall(Graphics& graphics, int x, float distance) {
    const int h = graphics.height();

    int lineHeight = int(h / distance);
    int drawStart = -lineHeight / 2 + h / 2;
    int drawEnd = lineHeight / 2 + h / 2;

    if (drawStart < 0) drawStart = 0;
    if (drawEnd >= h) drawEnd = h - 1;

    drawTexturedWall(graphics, x, distance, lineHeight, drawStart, drawEnd);
}

void Map::drawFlatColoredWall(Graphics& graphics, int x, int drawStart, int drawEnd) {
    Color color(0x000000FF);
    switch (worldMap[mapWidth * mapX + mapY]) {
        case 1: color.set(0xFF0000FF); break;
        case 2: color.set(0x00FF00FF); break;
        case 3: color.set(0x0000FFFF); break;
        case 4: color.set(0xFFFFFFFF); break;
    }

    // Uncomment this for a really shit lighting effect.
    //color /= std::max(distance/2, 1.0f);

    if (side == 1) color /= 2.0f;
    graphics.drawVertLine(x, drawStart, drawEnd, color.toHex());
}


void Map::drawTexturedWall(Graphics& graphics, int x, float distance, int lineHeight, int drawStart, int drawEnd) {
    int texNum = worldMap[mapWidth * mapX + mapY] - 1;
    const Texture& tex = textures[texNum];
    float wallX;

    if (side == 0) {
        wallX = rayPos.y + distance * rayDir.y;
    } else {
        wallX = rayPos.x + distance * rayDir.x;
    }

    wallX -= std::floor(wallX);

    int texX = int(wallX * float(tex.width));

    if ((side == 0 && rayDir.x > 0) ||
        (side == 1 && rayDir.y > 0)) {
        texX = tex.width - texX - 1;
    }     

    for (int y = drawStart; y < drawEnd; y++) {
        int d = y * 256 - graphics.height() * 128 + lineHeight * 128;
        int texY = ((d * tex.height) / lineHeight) / 256;
        uint32_t color = tex.pixels[tex.height * texX + texY];

        if (side == 1) {
            color = (color & 0xFEFEFEFE) >> 1;
        }

        graphics.setPixel(x, y, color);
    }
}
