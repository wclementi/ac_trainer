#include "stdafx.h"
#include "esp.h"
#include "offsets.h"
#include <iostream>

ESP::ESP(Game& game, Overlay& overlay) : game(game), overlay(overlay) {}

void ESP::Render() {
    const auto& state = game.GetState();

    for (const auto& ent : state.entities) {
        DrawBox(ent, state.localTeam, state.viewMatrix);
    }
}

void ESP::DrawBox(const Entity& ent, int localTeam, const Matrix& matrix) {
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
