#pragma once
#include "game.h"

class Aimbot {
public:
    Aimbot(Game& game, Process& target, float fov = 360.0f);
    
    void Update();
    float GetFov() const { return fov; }

private:
    Game& game;
    Process& target;
    float fov;

    uint32_t GetBestTarget();
    void AimAt(uint32_t localPlayerPtr, const Vec3& targetPos);
};
