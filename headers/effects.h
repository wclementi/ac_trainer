#pragma once
#include "geom.h"
#include <vector>
#include <string>

/**
 * @brief Represents a single floating text element in 3D space.
 * 
 * Typically used for RPG-style damage numbers that float above entities.
 */
struct FloatingText {
    Vec3 worldPos;      ///< 3D origin position of the text
    int value;          ///< Numeric value to display
    float startTime;    ///< Timestamp when the text was spawned
    float alpha = 1.0f; ///< Current opacity level (0.0 to 1.0)
};

/**
 * @brief Manages screen-space visual effects like damage numbers and hitmarkers.
 */
class VisualEffects {
public:
    /**
     * @brief Spawns a new floating damage number at the specified location.
     * @param pos The 3D world position where the damage occurred.
     * @param amount The numerical amount of damage dealt.
     */
    void AddDamage(Vec3 pos, int amount);

    /**
     * @brief Triggers a 2D hitmarker at the center of the screen.
     */
    void TriggerHitmarker();
    
    /**
     * @brief Updates the animations and lifetimes of all active effects.
     * @param deltaTime Time elapsed since the last frame in seconds.
     */
    void Update(float deltaTime);

    /**
     * @brief Renders all active effects using ImGui.
     * @param viewMatrix The current game view matrix for 3D to 2D projection.
     * @param screenWidth Width of the overlay window.
     * @param screenHeight Height of the overlay window.
     * @param aimbotFov The configured aimbot FOV to draw the FOV circle.
     * @param gameFov The actual FOV of the game camera.
     */
    void Render(const Matrix& viewMatrix, int screenWidth, int screenHeight, float aimbotFov, float gameFov);

private:
    std::vector<FloatingText> damageNumbers; ///< List of active floating text elements
    float hitmarkerAlpha = 0.0f;             ///< Current opacity of the hitmarker
    const float hitmarkerDuration = 0.5f;    ///< How long a hitmarker lasts in seconds
    const float floatingTextDuration = 1.5f; ///< How long floating text lasts in seconds
};
