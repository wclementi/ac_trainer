#pragma once
#include <cstdint>
#include "geom.h"
#include "offsets.h"

// Note: These offsets will need to be verified via Cheat Engine for the specific AC version
/**
 * @brief Represents a player or entity in the game.
 * 
 * Contains cached information about the entity read from the game's memory.
 * Note: These offsets are configured for Assault Cube v1.3.0.2.
 */
struct Entity {
    uintptr_t baseAddress;  ///< The base pointer address of this entity in game memory
    
    char name[16];          ///< Null-terminated player name string
    int health;             ///< Current health (0-100)
    int prevHealth;         ///< Previous frame's health (used to detect damage events)
    int armor;              ///< Current armor value
    int team;               ///< Team ID (0 or 1 typically in team modes)
    
    Vec3 headPos;           ///< 3D world coordinates of the entity's head
    Vec3 feetPos;           ///< 3D world coordinates of the entity's feet
    Vec3 viewAngles;        ///< The pitch/yaw view angles of the entity
    
    /**
     * @brief Checks if the entity is currently alive.
     * @return True if health is between 1 and 100 inclusive.
     */
    bool IsAlive() const {
        return health > 0 && health <= 100;
    }
};
