#pragma once
#include <cstdint>
#include "geom.h"
#include "offsets.h"

// Note: These offsets will need to be verified via Cheat Engine for the specific AC version
struct Entity {
    // We store the pointer base address of this entity from the game memory
    uintptr_t baseAddress;
    
    // Offsets updated for Assault Cube v1.3.0.2 based on RE
    char name[16];          // offsets::name
    int health;             // offsets::health
    int prevHealth;         // To detect health drops
    int armor;              // offsets::armor
    int team;               // offsets::team
    
    Vec3 headPos;           // offsets::headPos
    Vec3 feetPos;           // offsets::feetPos
    Vec3 viewAngles;        // offsets::viewAngles
    
    bool IsAlive() const {
        return health > 0 && health <= 100;
    }
};
