#include "stdafx.h"
#include "aimbot.h"
#include "offsets.h"
#include "config.h"
#include <limits>
#include <iostream>

Aimbot::Aimbot(Game& game, Process& target) : game(game), target(target) {}

float Aimbot::GetFov() const {
    return config.aimbotFov;
}

void Aimbot::Update() {
    if (!config.aimbotEnabled) return;

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
    if (!state.localPlayerPtr || state.gameFov <= 0) return 0;

    float centerX = (float)state.screenWidth / 2.0f;
    float centerY = (float)state.screenHeight / 2.0f;
    
    // Calculate the same radius used in rendering
    const float PI = 3.1415926535f;
    float aimbotFovRad = (config.aimbotFov / 2.0f) * (PI / 180.0f);
    float gameFovRad = (state.gameFov / 2.0f) * (PI / 180.0f);
    
    float screenWidth = (float)state.screenWidth; 
    float screenHeight = (float)state.screenHeight;

    float maxPixelRadius = (tanf(aimbotFovRad) / tanf(gameFovRad)) * (screenWidth / 2.0f);
    
    uint32_t bestTarget = 0;
    float closestPixelDistance = std::numeric_limits<float>::max();

    for (const auto& ent : state.entities) {
        if (ent.team == state.localTeam) continue;

        Vec2 screenPos;
        if (WorldToScreen(ent.headPos, screenPos, state.viewMatrix, (int)screenWidth, (int)screenHeight)) {
            float dx = screenPos.x - (screenWidth / 2.0f);
            float dy = screenPos.y - (screenHeight / 2.0f);
            float pixelDist = std::sqrt(dx * dx + dy * dy);

            if (pixelDist <= maxPixelRadius && pixelDist < closestPixelDistance) {
                closestPixelDistance = pixelDist;
                bestTarget = (uint32_t)ent.baseAddress;
            }
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
