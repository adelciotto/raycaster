#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include "map.h"
#include "player.h"
#include "graphics.h"
#include "color.h"

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
const int texWidth = 64;
const int texHeight = 64;
const int collisionOffset = 4;

Map::Map(Player *player, const std::string& file) 
    : player(player),
      mapX(0),
      mapY(0) {
    generateTextures();    
    loadFile(file);
}

// TODO: Load map textures from files..
// For now generate really random looking textures with some simple math.
void Map::generateTextures() {
    // Resize each of the texture vectors.
    for (auto &t: textures) {
        t.resize(texWidth * texHeight);
    }

    for (int x = 0; x < texWidth; x++) {
        for (int y = 0; y < texHeight; y++) {
            int i = texWidth * y + x;
            int xorcolor = (x * 512 / texWidth) ^ (y * 512 / texHeight);

            textures[0][i] = 0xFF000000 * (x != y && x != texWidth - y);
            textures[1][i] = 0x7F00FFFF * (x % 16 && y % 16);
            textures[2][i] = xorcolor + 256 * xorcolor;
            textures[3][i] = 0x0000FFFF * (x % 16 && y % 16);
            textures[4][i] = 512 * xorcolor;
            textures[5][i] = 0xFF8000FF * (x % 16 && y % 16);
            textures[6][i] = 128 * xorcolor;
            textures[7][i] = 0xCCCCCCCC;
        }
    }
}

void Map::loadFile(const std::string& file) {
    std::ifstream infile;
    std::string line;
    auto loadDefaultMap = [&]() -> void {
        logger::warn("%s could not be loaded, using default map\n", file.c_str());
        mapWidth = 24;
        mapHeight = 24;
        worldMap = std::vector<uint8_t>(std::begin(defaultWorldMap), std::end(defaultWorldMap));
    };

    infile.open(file);
    if (infile.fail()) {
        loadDefaultMap();
        return;
    }

    auto getProperty = [&](const std::string& prop, uint32_t& out) -> void {
        std::getline(infile, line);
        auto tokens = utils::stringSplit(line, '=');

        if (tokens.size() == 2 && tokens[0].compare(prop)) {
            out = std::stoi(tokens[1]);
        } else {
            loadDefaultMap();
            return;
        }
    };

    // Get the map dimensions.
    uint32_t startX, startY;
    getProperty("width", mapWidth);
    getProperty("height", mapHeight);
    getProperty("startingX", startX);
    getProperty("startignY", startY);

    worldMap.reserve(mapWidth*mapHeight);
    player->position.set(startX, startY);

    // Get the map data.
    while (std::getline(infile, line)) {
        auto mapData = utils::stringSplit(line, ',');

        if (mapData.size() == mapWidth) {
            for (const auto& i : mapData) {
                worldMap.push_back(std::stoi(i));
            } 
        } else {
            logger::warn("invalid map data for %s\n", file.c_str());
            loadDefaultMap();
            return;
        }
    }

    if (worldMap.size() != mapWidth*mapHeight) {
        logger::warn("invalid map data for %s\n", file.c_str());
        loadDefaultMap();
        return;
    }

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
    const int halfH = graphics.height() / 2;

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

    //drawFlatColoredWall(graphics, x, drawStart, drawEnd);
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
    float wallX;

    if (side == 0) {
        wallX = rayPos.y + distance * rayDir.y;
    } else {
        wallX = rayPos.x + distance * rayDir.x;
    }

    wallX -= std::floor(wallX);

    int texX = int(wallX * float(texWidth));

    if ((side == 0 && rayDir.x > 0) ||
        (side == 1 && rayDir.y > 0)) {
        texX = texWidth - texX - 1;
    }     

    for (int y = drawStart; y < drawEnd; y++) {
        int d = y * 256 - graphics.height() * 128 + lineHeight * 128;
        int texY = ((d * texHeight) / lineHeight) / 256;
        uint32_t color = textures[texNum][texHeight * texY + texX];

        if (side == 1) {
            color = (color & 0xFEFEFEFE) >> 1;
        }

        graphics.setPixel(x, y, color);
    }
}
