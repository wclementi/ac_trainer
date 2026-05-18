#pragma once
#include "game.h"
#include "overlay.h"
#include "imgui.h"
#include <vector>

/**
 * @brief Manages Extra Sensory Perception (ESP) rendering.
 * 
 * The ESP class is responsible for drawing bounding boxes, health bars, 
 * player names, and snaplines over game entities.
 */
class ESP {
public:
    /**
     * @brief Constructs an ESP instance.
     * @param game Reference to the game state manager.
     * @param overlay Reference to the DirectX overlay for drawing.
     */
    ESP(Game& game, Overlay& overlay);
    
    /**
     * @brief Renders the ESP for all valid entities.
     * @param state The current snapshot of the game state.
     */
    void Render(const GameState& state);

private:
    Game& game;         ///< Reference to the game state
    Overlay& overlay;   ///< Reference to the overlay

    /**
     * @brief Draws a 2D bounding box and info around an entity.
     * @param ent The target entity to draw around.
     * @param localTeam The local player's team ID (for enemy/teammate coloring).
     * @param matrix The current view matrix to project 3D to 2D.
     */
    void DrawBox(const Entity& ent, int localTeam, const Matrix& matrix);

    /**
     * @brief Draws a vertical health bar next to the entity's bounding box.
     * @param screenTop The 2D screen coordinate of the entity's head.
     * @param height The height of the bounding box.
     * @param width The width of the bounding box.
     * @param health The entity's health (0-100).
     */
    void DrawHealthBar(const Vec2& screenTop, float height, float width, int health);
};
