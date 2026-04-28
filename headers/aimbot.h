#pragma once
#include "game.h"

class Aimbot {
public:
    Aimbot(Game& game, Process& target);
    
    void Update();
    float GetFov() const;

private:
    Game& game;
    Process& target;
    float fov;

    uint32_t GetBestTarget();
    void AimAt(uint32_t localPlayerPtr, const Vec3& targetPos);
};
