#include "raycaster.h"
#include "resource_loader.h"

Raycaster::Raycaster() :
    Game("./settings.json") { }

void Raycaster::load() {
    ResourceLoader::instance().queue<Texture>("assets/textures/texture-0.png", "tex0");
    ResourceLoader::instance().queue<Texture>("assets/textures/texture-1.png", "tex1");
    ResourceLoader::instance().queue<Texture>("assets/textures/texture-2.png", "tex2");
    ResourceLoader::instance().queue<Texture>("assets/textures/texture-3.png", "tex3");
    ResourceLoader::instance().queue<Texture>("assets/textures/texture-4.png", "tex4");
    ResourceLoader::instance().queue<Texture>("assets/textures/texture-5.png", "tex5");
    ResourceLoader::instance().queue<Texture>("assets/textures/texture-6.png", "tex6");
    ResourceLoader::instance().queue<Texture>("assets/textures/texture-7.png", "tex7");
    ResourceLoader::instance().loadAll();
}

void Raycaster::create(const Settings& settings) {
    world.create(settings.mapfile);
}

void Raycaster::handleInput() {

}

void Raycaster::update() {
    world.update();
}

void Raycaster::draw() {
    world.draw(graphics);
}
