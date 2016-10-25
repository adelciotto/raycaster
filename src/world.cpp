#include "world.h"
#include "graphics.h"

World::World() {

}

void World::create(const std::string& mapFile) {
    map.create(&player, mapFile);
}

void World::update() {
    map.update();
}

void World::draw(Graphics& graphics) {
    map.draw(graphics); 
}
