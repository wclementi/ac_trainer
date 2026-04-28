#pragma once
#include "proc.h"
#include "entity.h"
#include "geom.h"
#include "offsets.h"
#include <vector>

struct DamageEvent {
    Vec3 pos;
    int amount;
};

struct GameState {
    uint32_t localPlayerPtr;
    int localTeam;
    int playerCount;
    uint32_t entityListPtr;
    Matrix viewMatrix;
    float gameFov;
    std::vector<Entity> entities;
    std::vector<DamageEvent> damageEvents;
};

class Game {
public:
    Game(Process& target);
    
    bool Update();
    const GameState& GetState() const { return state; }

private:
    Process& target;
    GameState state;
    std::vector<Entity> entityCache; // Store entities to track health changes

    bool UpdateLocalPlayer();
    bool UpdateEntities();
};
