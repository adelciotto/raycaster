#ifndef WORLD_H
#define WORLD_H

#include "map.h"
#include "player.h"

class Graphics;

class World {
public:
    World();

    void create(const std::string& mapFile);
    void update();
    void draw(Graphics& graphics); 

private:
    Map map;
    Player player;
};

#endif // WORLD_H
