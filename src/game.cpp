#include "stdafx.h"
#include "game.h"

Game::Game(Process& target) : target(target), state{} {}

bool Game::Update() {
    uintptr_t baseAddr = target.GetBaseAddress();
    if (!baseAddr) return false;

    // Read global state
    state.localPlayerPtr = target.Read<uint32_t>(baseAddr + offsets::localPlayer);
    state.playerCount = target.Read<int>(baseAddr + offsets::playerCount);
    state.entityListPtr = target.Read<uint32_t>(baseAddr + offsets::entityList);

    target.ReadBuffer(baseAddr + offsets::viewMatrix, &state.viewMatrix, sizeof(state.viewMatrix));
    state.gameFov = target.Read<float>(baseAddr + offsets::fov);

    if (!state.localPlayerPtr) return false;

    state.localTeam = target.Read<int>(state.localPlayerPtr + offsets::team);

    return UpdateEntities();
}

bool Game::UpdateEntities() {
    state.entities.clear();
    state.damageEvents.clear();
    
    std::vector<Entity> newEntities;

    for (int i = 1; i < state.playerCount; ++i) {
        uint32_t entPtr = target.Read<uint32_t>(state.entityListPtr + i * 4);
        if (!entPtr) continue;

        Entity ent;
        ent.baseAddress = entPtr;
        ent.health = target.Read<int>(entPtr + offsets::health);
        
        if (!ent.IsAlive()) continue;

        ent.team = target.Read<int>(entPtr + offsets::team);
        ent.headPos = target.Read<Vec3>(entPtr + offsets::headPos);
        ent.feetPos = target.Read<Vec3>(entPtr + offsets::feetPos);
        target.ReadBuffer(entPtr + offsets::name, &ent.name, sizeof(ent.name));

        // Find in cache to check for damage
        bool found = false;
        for (auto& cachedEnt : entityCache) {
            if (cachedEnt.baseAddress == ent.baseAddress) {
                ent.prevHealth = cachedEnt.health;
                
                // Detect damage
                if (ent.health < ent.prevHealth) {
                    state.damageEvents.push_back({ ent.headPos, ent.prevHealth - ent.health });
                }
                found = true;
                break;
            }
        }

        if (!found) {
            ent.prevHealth = ent.health;
        }

        newEntities.push_back(ent);
    }

    state.entities = newEntities;
    entityCache = newEntities;
    return true;
}
