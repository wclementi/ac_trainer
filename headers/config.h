#pragma once

/**
 * @brief Global configuration structure holding user settings.
 *
 * This struct represents all toggles and values adjustable via the cheat menu.
 */
struct Config {
    // ESP
    bool espEnabled = true;     ///< Master switch for all ESP features
    bool espBoxes = true;       ///< Draw 2D bounding boxes around players
    bool espNames = true;       ///< Draw player names above boxes
    bool espSnaplines = true;   ///< Draw lines from screen bottom to players
    bool espHealth = true;      ///< Draw health bars next to boxes
    
    // Aimbot
    bool aimbotEnabled = true;  ///< Master switch for the aimbot
    float aimbotFov = 15.0f;    ///< Field of View in degrees for target acquisition
    bool drawFov = true;        ///< Draw a circle indicating the aimbot FOV
    
    // Effects
    bool hitmarkers = true;     ///< Draw hitmarkers on screen when dealing damage
    bool damageNumbers = true;  ///< Display floating damage numbers above damaged entities
    
    // Internal State
    bool menuVisible = false;   ///< Whether the configuration menu is currently open
};

/**
 * @brief Global configuration instance.
 */
extern Config config;
