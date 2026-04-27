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

    if (!state.localPlayerPtr) return false;

    state.localTeam = target.Read<int>(state.localPlayerPtr + offsets::team);

    return UpdateEntities();
}

bool Game::UpdateEntities() {
    state.entities.clear();
    
    // We start from 1 to skip local player (index 0 is often local or empty in AC)
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
        
        // Read name
        target.ReadBuffer(entPtr + offsets::name, &ent.name, sizeof(ent.name));

        state.entities.push_back(ent);
    }
    return true;
}
