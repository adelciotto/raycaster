#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "game.h"
#include "world.h"

class Raycaster : public Game {
public:  
    Raycaster();
    ~Raycaster() { }

protected:
    void load() override;
    void create(const Settings& settings) override;
    void handleInput() override;
    void update() override;
    void draw() override;

private:
    World world;
};

#endif // RAYCASTER_H
