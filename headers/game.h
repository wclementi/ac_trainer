#pragma once
#include "proc.h"
#include "entity.h"
#include "geom.h"
#include "offsets.h"
#include <vector>

/**
 * @brief Represents a single event where damage was dealt.
 */
struct DamageEvent {
    Vec3 pos;   ///< 3D world position where the damage occurred
    int amount; ///< Amount of damage dealt
};

/**
 * @brief A snapshot of the current game state for a single frame.
 */
struct GameState {
    uint32_t localPlayerPtr;             ///< Pointer to the local player in memory
    int localTeam;                       ///< Team ID of the local player
    int playerCount;                     ///< Total number of players in the game
    uint32_t entityListPtr;              ///< Pointer to the entity list array
    Matrix viewMatrix;                   ///< The 4x4 view projection matrix
    float gameFov;                       ///< The camera field of view
    int screenWidth;                     ///< Width of the game window
    int screenHeight;                    ///< Height of the game window
    std::vector<Entity> entities;        ///< List of all active entities (excluding local player)
    std::vector<DamageEvent> damageEvents; ///< Events representing damage dealt this frame
};

/**
 * @brief Reads and manages the game state from the target process.
 */
class Game {
public:
    /**
     * @brief Constructor for the Game state manager.
     * @param target The target process to read from.
     */
    Game(Process& target);
    
    /**
     * @brief Updates the game state by reading from process memory.
     * @param width Current width of the overlay.
     * @param height Current height of the overlay.
     * @return True if the update was successful, false if memory read failed.
     */
    bool Update(int width, int height);

    /**
     * @brief Gets the most recently updated game state.
     * @return Constant reference to the game state.
     */
    const GameState& GetState() const { return state; }

private:
    Process& target;                    ///< Reference to the target process
    GameState state;                    ///< The current game state
    std::vector<Entity> entityCache;    ///< Cached entities to track delta changes (e.g. health)

    /**
     * @brief Updates information about the local player.
     * @return True on success.
     */
    bool UpdateLocalPlayer();

    /**
     * @brief Updates the entity list and checks for damage events.
     * @return True on success.
     */
    bool UpdateEntities();
};
