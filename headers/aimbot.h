#pragma once
#include "proc.h"
#include "entity.h"
#include "geom.h"

class Aimbot {
public:
    Aimbot(Process& target);
    
    void Update();

private:
    Process& target;

    uint32_t GetBestTarget(uintptr_t baseAddr, uint32_t localPlayer, int localTeam);
    void AimAt(uint32_t localPlayer, const Vec3& targetPos);
};
