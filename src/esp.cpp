#include "stdafx.h"
#include "esp.h"
#include "offsets.h"
#include <iostream>

ESP::ESP(Process& target, Overlay& overlay) : target(target), overlay(overlay) {}

void ESP::Render() {
    uintptr_t baseAddr = target.GetModuleBaseAddress(L"ac_client.exe");
    if (!baseAddr) return;

    int playerCount = target.Read<int>(baseAddr + offsets::playerCount);
    uint32_t entList = target.Read<uint32_t>(baseAddr + offsets::entityList);
    uint32_t localPlayer = target.Read<uint32_t>(baseAddr + offsets::localPlayer);

    if (!localPlayer) return;

    float matrix[16];
    if (target.GetHandle()) {
        ReadProcessMemory(target.GetHandle(), (LPCVOID)(baseAddr + offsets::viewMatrix), &matrix, sizeof(matrix), nullptr);
    }

    int localTeam = target.Read<int>(localPlayer + offsets::team);

    for (int i = 1; i < playerCount; ++i) {
        uint32_t entPtr = target.Read<uint32_t>(entList + i * 4);
        if (!entPtr) continue;

        Entity ent;
        ent.health = target.Read<int>(entPtr + offsets::health);
        
        if (!ent.IsAlive()) continue;

        ent.team = target.Read<int>(entPtr + offsets::team);
        ent.headPos = target.Read<Vec3>(entPtr + offsets::headPos);
        ent.feetPos = target.Read<Vec3>(entPtr + offsets::feetPos);

        DrawBox(ent, localTeam, matrix);
    }
}

void ESP::DrawBox(const Entity& ent, int localTeam, float matrix[16]) {
    Vec3 topPos = ent.headPos;
    topPos.z += 0.5f;

    Vec2 screenTop, screenBot;
    if (WorldToScreen(topPos, screenTop, matrix, overlay.GetWidth(), overlay.GetHeight()) &&
        WorldToScreen(ent.feetPos, screenBot, matrix, overlay.GetWidth(), overlay.GetHeight())) {

        float height = screenBot.y - screenTop.y;
        float width = height / 2.0f;

        ImU32 boxColor = (ent.team == localTeam) ? IM_COL32(0, 255, 0, 255) : IM_COL32(255, 0, 0, 255);

        // Draw Box
        ImGui::GetBackgroundDrawList()->AddRect(
            ImVec2(screenTop.x - width / 2, screenTop.y),
            ImVec2(screenTop.x + width / 2, screenBot.y),
            boxColor,
            0.0f, 0, 1.5f
        );

        DrawHealthBar(screenTop, height, width, ent.health);
    }
}

void ESP::DrawHealthBar(const Vec2& screenTop, float height, float width, int health) {
    float healthPct = health / 100.0f;
    float healthHeight = height * healthPct;
    float screenBotY = screenTop.y + height;
    
    // Red background
    ImGui::GetBackgroundDrawList()->AddRectFilled(
        ImVec2(screenTop.x - width / 2 - 5, screenTop.y),
        ImVec2(screenTop.x - width / 2 - 2, screenBotY),
        IM_COL32(255, 0, 0, 255)
    );
    
    // Green foreground
    ImGui::GetBackgroundDrawList()->AddRectFilled(
        ImVec2(screenTop.x - width / 2 - 5, screenBotY - healthHeight),
        ImVec2(screenTop.x - width / 2 - 2, screenBotY),
        IM_COL32(0, 255, 0, 255)
    );
}
