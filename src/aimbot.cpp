#include "stdafx.h"
#include "aimbot.h"
#include "offsets.h"
#include <limits>
#include <iostream>

Aimbot::Aimbot(Process& target) : target(target) {}

void Aimbot::Update() {
    // Only aim if Right Mouse Button is held
    if (!(GetAsyncKeyState(VK_RBUTTON) & 0x8000)) {
        return;
    }

    uintptr_t baseAddr = target.GetModuleBaseAddress(L"ac_client.exe");
    if (!baseAddr) return;

    uint32_t localPlayer = target.Read<uint32_t>(baseAddr + offsets::localPlayer);
    if (!localPlayer) return;

    int localTeam = target.Read<int>(localPlayer + offsets::team);
    
    uint32_t bestTargetPtr = GetBestTarget(baseAddr, localPlayer, localTeam);
    if (bestTargetPtr) {
        Vec3 targetHead = target.Read<Vec3>(bestTargetPtr + offsets::headPos);
        AimAt(localPlayer, targetHead);
    }
}

uint32_t Aimbot::GetBestTarget(uintptr_t baseAddr, uint32_t localPlayer, int localTeam) {
    int playerCount = target.Read<int>(baseAddr + offsets::playerCount);
    uint32_t entList = target.Read<uint32_t>(baseAddr + offsets::entityList);

    Vec3 localPos = target.Read<Vec3>(localPlayer + offsets::feetPos);
    
    uint32_t bestTarget = 0;
    float closestDistance = std::numeric_limits<float>::max();

    for (int i = 1; i < playerCount; ++i) {
        uint32_t entPtr = target.Read<uint32_t>(entList + i * 4);
        if (!entPtr) continue;

        int health = target.Read<int>(entPtr + offsets::health);
        if (health <= 0 || health > 100) continue;

        int team = target.Read<int>(entPtr + offsets::team);
        if (team == localTeam) continue;

        Vec3 entPos = target.Read<Vec3>(entPtr + offsets::feetPos);
        float distance = GetDistance(localPos, entPos);

        if (distance < closestDistance) {
            closestDistance = distance;
            bestTarget = entPtr;
        }
    }

    return bestTarget;
}

void Aimbot::AimAt(uint32_t localPlayer, const Vec3& targetPos) {
    Vec3 localHead = target.Read<Vec3>(localPlayer + offsets::headPos);
    Vec3 angles = CalcAngle(localHead, targetPos);

    // Write to memory
    // Assault Cube view angles are at offset 0x34 (yaw) and 0x38 (pitch)
    if (target.GetHandle()) {
        WriteProcessMemory(target.GetHandle(), (LPVOID)(localPlayer + offsets::viewAngles), &angles.x, sizeof(float), nullptr);
        WriteProcessMemory(target.GetHandle(), (LPVOID)(localPlayer + offsets::viewAngles + 4), &angles.y, sizeof(float), nullptr);
    }
}
