#pragma once
#include "game.h"

/**
 * @brief Handles automated aiming at target entities.
 *
 * This class calculates the best target within a specified FOV
 * and manipulates the local player's view angles to aim at them.
 */
class Aimbot {
public:
    /**
     * @brief Constructor for the Aimbot.
     * @param game Reference to the current game state.
     * @param target Reference to the process memory handler.
     */
    Aimbot(Game& game, Process& target);
    
    /**
     * @brief Updates the aimbot logic, locking onto targets if enabled.
     * Should be called once per frame.
     */
    void Update();

    /**
     * @brief Gets the current Field of View (FOV) setting for the aimbot.
     * @return The FOV in degrees.
     */
    float GetFov() const;

private:
    Game& game;     ///< Reference to the game state
    Process& target; ///< Reference to the process
    float fov;      ///< Current FOV

    /**
     * @brief Finds the best target within the FOV.
     * @return The base address pointer of the best target, or 0 if none found.
     */
    uint32_t GetBestTarget();

    /**
     * @brief Aims the local player's view at a specific world position.
     * @param localPlayerPtr Pointer to the local player entity.
     * @param targetPos The 3D world position to aim at.
     */
    void AimAt(uint32_t localPlayerPtr, const Vec3& targetPos);
};
