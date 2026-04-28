#include "stdafx.h"
#include "esp.h"
#include "offsets.h"
#include <iostream>

ESP::ESP(Game& game, Overlay& overlay) : game(game), overlay(overlay) {}

void ESP::Render() {
    const auto& state = game.GetState();

    for (const auto& ent : state.entities) {
        DrawBox(ent, state.localTeam, state.viewMatrix);
        
        // Draw Snapline (from bottom center to enemy feet)
        Vec2 screenBot;
        if (WorldToScreen(ent.feetPos, screenBot, state.viewMatrix, overlay.GetWidth(), overlay.GetHeight())) {
            ImU32 snapColor = (ent.team == state.localTeam) ? IM_COL32(0, 255, 0, 100) : IM_COL32(255, 0, 0, 100);
            ImGui::GetBackgroundDrawList()->AddLine(
                ImVec2(overlay.GetWidth() / 2.0f, (float)overlay.GetHeight()),
                ImVec2(screenBot.x, screenBot.y),
                snapColor,
                1.0f
            );
        }
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

        // Draw Name below box
        std::string nameStr(ent.name);
        ImVec2 textSize = ImGui::CalcTextSize(nameStr.c_str());
        ImVec2 textPos = ImVec2(screenTop.x - textSize.x / 2, screenBot.y + 2);
        
        // Shadow
        ImGui::GetBackgroundDrawList()->AddText(
            ImVec2(textPos.x + 1, textPos.y + 1),
            IM_COL32(0, 0, 0, 255),
            nameStr.c_str()
        );
        
        // Main Text
        ImGui::GetBackgroundDrawList()->AddText(
            textPos,
            IM_COL32(255, 255, 255, 255),
            nameStr.c_str()
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
