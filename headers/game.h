#pragma once
#include "proc.h"
#include "entity.h"
#include "geom.h"
#include "offsets.h"
#include <vector>

struct GameState {
    uint32_t localPlayerPtr;
    int localTeam;
    int playerCount;
    uint32_t entityListPtr;
    Matrix viewMatrix;
    std::vector<Entity> entities;
};

class Game {
public:
    Game(Process& target);
    
    bool Update();
    const GameState& GetState() const { return state; }

private:
    Process& target;
    GameState state;

    bool UpdateLocalPlayer();
    bool UpdateEntities();
};
