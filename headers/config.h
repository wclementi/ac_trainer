#pragma once

struct Config {
    // ESP
    bool espEnabled = true;
    bool espBoxes = true;
    bool espNames = true;
    bool espSnaplines = true;
    bool espHealth = true;
    
    // Aimbot
    bool aimbotEnabled = true;
    float aimbotFov = 15.0f;
    bool drawFov = true;
    
    // Effects
    bool hitmarkers = true;
    bool damageNumbers = true;
    
    // Internal State
    bool menuVisible = false;
};

extern Config config;
