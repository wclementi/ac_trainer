#pragma once
#include <cstdint>
#include "geom.h"

// Note: These offsets will need to be verified via Cheat Engine for the specific AC version
struct Entity {
    // We store the pointer base address of this entity from the game memory
    uintptr_t baseAddress;
    
    // Offsets updated for Assault Cube v1.3.0.2 based on RE
    char name[16];          // 0x205
    int health;             // 0xEC
    int armor;              // 0xF0
    int team;               // 0x30C
    
    Vec3 headPos;           // 0x4
    Vec3 feetPos;           // 0x34
    Vec3 viewAngles;        // 0x40 (pitch, yaw)
    
    bool IsAlive() const {
        return health > 0 && health <= 100;
    }
};
