#include "stdafx.h"
#include "aimbot.h"
#include "offsets.h"
#include <limits>
#include <iostream>

Aimbot::Aimbot(Game& game, Process& target, float fov) : game(game), target(target), fov(fov) {}

void Aimbot::Update() {
    // Aim if either Right Mouse Button or Left Mouse Button is held
    if (!(GetAsyncKeyState(VK_RBUTTON) & 0x8000) && !(GetAsyncKeyState(VK_LBUTTON) & 0x8000)) {
        return;
    }

    uint32_t bestTargetPtr = GetBestTarget();
    if (bestTargetPtr) {
        Vec3 targetHead = target.Read<Vec3>(bestTargetPtr + offsets::headPos);
        AimAt(game.GetState().localPlayerPtr, targetHead);
    }
}

uint32_t Aimbot::GetBestTarget() {
    const auto& state = game.GetState();
    if (!state.localPlayerPtr) return 0;

    Vec3 localHead = target.Read<Vec3>(state.localPlayerPtr + offsets::headPos);
    Vec3 currentAngles = target.Read<Vec3>(state.localPlayerPtr + offsets::viewAngles);
    
    uint32_t bestTarget = 0;
    float closestAngleDistance = std::numeric_limits<float>::max();

    for (const auto& ent : state.entities) {
        if (ent.team == state.localTeam) continue;

        Vec3 requiredAngles = CalcAngle(localHead, ent.headPos);

        // Calculate angle distance (FOV distance)
        float diffYaw = std::abs(currentAngles.x - requiredAngles.x);
        if (diffYaw > 180.0f) diffYaw = 360.0f - diffYaw;

        float diffPitch = std::abs(currentAngles.y - requiredAngles.y);
        
        float angleDistance = std::sqrt(diffYaw * diffYaw + diffPitch * diffPitch);

        // Check if target is within FOV radius
        if (angleDistance > fov) continue;

        if (angleDistance < closestAngleDistance) {
            closestAngleDistance = angleDistance;
            bestTarget = (uint32_t)ent.baseAddress;
        }
    }

    return bestTarget;
}

void Aimbot::AimAt(uint32_t localPlayerPtr, const Vec3& targetPos) {
    Vec3 localHead = target.Read<Vec3>(localPlayerPtr + offsets::headPos);
    Vec3 angles = CalcAngle(localHead, targetPos);

    // Write to memory
    target.Write<float>(localPlayerPtr + offsets::viewAngles, angles.x);
    target.Write<float>(localPlayerPtr + offsets::viewAngles + 4, angles.y);
}
