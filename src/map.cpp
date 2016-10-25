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
#include "resource_loader.h"

namespace {
    const int defaultMapWidth = 24;
    const int defaultMapHeight = 24;
    const int defaultWorldMap[defaultMapWidth * defaultMapHeight] = {
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
    const int collisionOffset = 2;
}

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
    textures[0] = ResourceLoader::instance().get<Texture>("tex0");
    textures[1] = ResourceLoader::instance().get<Texture>("tex1");
    textures[2] = ResourceLoader::instance().get<Texture>("tex2");
    textures[3] = ResourceLoader::instance().get<Texture>("tex3");
    textures[4] = ResourceLoader::instance().get<Texture>("tex4");
    textures[5] = ResourceLoader::instance().get<Texture>("tex5");
    textures[6] = ResourceLoader::instance().get<Texture>("tex6");
    textures[7] = ResourceLoader::instance().get<Texture>("tex7");
}

void Map::loadFile(const std::string& file) {
    JsonParser jsonParser(file);

    const double startX = jsonParser.get<double>("startX", player->position.x);
    const double startY = jsonParser.get<double>("startY", player->position.y);
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

void Map::update() {
    resolveCollisions();
    player->update();
}

void Map::resolveCollisions() {
    const auto vel = player->velocity;
    const auto pos = player->position;
    const auto dir = player->direction;
    auto checkWall = [&](const mathutils::Vec2& heading) -> void {
        if (getMapIndex(int(heading.x), int(pos.y))) player->velocity.x = 0;
        if (getMapIndex(int(pos.x), int(heading.y))) player->velocity.y = 0;
    };

    // Really simple player-wall collision.
    if (player->isMovingForwards()) {
        const auto headingPos = pos + dir * vel*collisionOffset;

        checkWall(headingPos);
    }
    if (player->isMovingBackwards()) {
        const auto headingPos = pos - dir * vel*-collisionOffset;

        checkWall(headingPos);
    }
}

int Map::getMapIndex(int x, int y) const {
    size_t idx = mapWidth * x + y;

    return idx < worldMap.size() ? worldMap[idx] : 0;
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
        double cameraX = 2 * x / (double)w - 1;
        double distance = castRay(cameraX);

        drawWall(graphics, x, distance);
    } 
}

double Map::castRay(double cameraX) {
    rayPos.set(player->position);
    rayDir.set(player->direction + player->cameraPlane * cameraX);

    mapX = (int)rayPos.x;
    mapY = (int)rayPos.y;

    double deltaDistX = sqrt(1 + (rayDir.y*rayDir.y) / (rayDir.x*rayDir.x));
    double deltaDistY = sqrt(1 + (rayDir.x*rayDir.x) / (rayDir.y*rayDir.y));
    
    double sideDistX, sideDistY;
    int stepX, stepY;

    if (rayDir.x < 0) {
        stepX = -1;
        sideDistX = (rayPos.x - mapX) * deltaDistX;
    } else {
        stepX = 1;
        sideDistX = (mapX + 1.0 - rayPos.x) * deltaDistX;
    }
    if (rayDir.y < 0) {
        stepY = -1;
        sideDistY = (rayPos.y - mapY) * deltaDistY;
    } else {
        stepY = 1;
        sideDistY = (mapY + 1.0 - rayPos.y) * deltaDistY;
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

        if (getMapIndex(mapX, mapY) > 0) {
            hit = true;
        }
    }
    
    if (side == 0) {
        return (mapX - rayPos.x + (1 - stepX) / 2) / rayDir.x;   
    } else {
        return (mapY - rayPos.y + (1 - stepY) / 2) / rayDir.y;   
    }
}

void Map::drawWall(Graphics& graphics, int x, double distance) {
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
    switch (getMapIndex(mapX, mapY)) {
        case 1: color.set(0xFF0000FF); break;
        case 2: color.set(0x00FF00FF); break;
        case 3: color.set(0x0000FFFF); break;
        case 4: color.set(0xFFFFFFFF); break;
    }

    // Uncomment this for a really shit lighting effect.
    //color /= std::max(distance/2, 1.0f);

    if (side == 1) color /= 2.0;
    graphics.drawVertLine(x, drawStart, drawEnd, color.toHex());
}


void Map::drawTexturedWall(Graphics& graphics, int x, double distance, int lineHeight, int drawStart, int drawEnd) {
    int texNum = getMapIndex(mapX, mapY) - 1;
    const Texture& tex = *textures[texNum];
    int texWidth = tex.getWidth();
    int texHeight = tex.getHeight();
    double wallX;

    if (side == 0) {
        wallX = rayPos.y + distance * rayDir.y;
    } else {
        wallX = rayPos.x + distance * rayDir.x;
    }

    wallX -= std::floor(wallX);

    int texX = int(wallX * double(texWidth));

    if ((side == 0 && rayDir.x > 0) ||
        (side == 1 && rayDir.y > 0)) {
        texX = texWidth - texX - 1;
    }     

    for (int y = drawStart; y < drawEnd; y++) {
        int d = y * 256 - graphics.height() * 128 + lineHeight * 128;
        int texY = ((d * texHeight) / lineHeight) / 256;
        uint32_t color = tex.getPixel(texX, texY);

        if (side == 1) {
            color = (color & 0xFEFEFEFE) >> 1;
        }

        graphics.setPixel(x, y, color);
    }
}
