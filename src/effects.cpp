#include "stdafx.h"
#include "effects.h"
#include "overlay.h"
#include "config.h"
#include <imgui.h>

void VisualEffects::AddDamage(Vec3 pos, int amount) {
    damageNumbers.push_back({ pos, amount, 0.0f, 1.0f });
    TriggerHitmarker();
}

void VisualEffects::TriggerHitmarker() {
    hitmarkerAlpha = 1.0f;
}

void VisualEffects::Update(float deltaTime) {
    // Update Hitmarker
    if (hitmarkerAlpha > 0.0f) {
        hitmarkerAlpha -= deltaTime / hitmarkerDuration;
        if (hitmarkerAlpha < 0.0f) hitmarkerAlpha = 0.0f;
    }

    // Update Floating Text
    for (auto it = damageNumbers.begin(); it != damageNumbers.end();) {
        it->startTime += deltaTime;
        it->alpha = 1.0f - (it->startTime / floatingTextDuration);
        
        // Float upward
        it->worldPos.z += 3.0f * deltaTime;

        if (it->startTime >= floatingTextDuration) {
            it = damageNumbers.erase(it);
        } else {
            ++it;
        }
    }
}

void VisualEffects::Render(const Matrix& viewMatrix, int screenWidth, int screenHeight, float aimbotFov, float gameFov) {
    auto drawList = ImGui::GetBackgroundDrawList();

    // Render Aimbot FOV Circle
    if (config.drawFov && aimbotFov > 0.0f && gameFov > 0.0f) {
        float centerX = screenWidth / 2.0f;
        float centerY = screenHeight / 2.0f;
        
        float radius = (screenWidth / gameFov) * aimbotFov;
        drawList->AddCircle(ImVec2(centerX, centerY), radius, IM_COL32(200, 200, 200, 80), 64, 1.0f);
    }

    // Render Hitmarker (Center of screen)
    if (config.hitmarkers && hitmarkerAlpha > 0.0f) {
        float centerX = screenWidth / 2.0f;
        float centerY = screenHeight / 2.0f;
        float size = 10.0f;
        ImU32 color = IM_COL32(255, 255, 255, (int)(hitmarkerAlpha * 255));

        drawList->AddLine(ImVec2(centerX - size, centerY - size), ImVec2(centerX + size, centerY + size), color, 2.0f);
        drawList->AddLine(ImVec2(centerX + size, centerY - size), ImVec2(centerX - size, centerY + size), color, 2.0f);
    }

    // Render Floating Text
    if (config.damageNumbers) {
        for (const auto& damage : damageNumbers) {
            Vec2 screenPos;
            if (WorldToScreen(damage.worldPos, screenPos, viewMatrix, screenWidth, screenHeight)) {
                std::string text = "-" + std::to_string(damage.value);
                ImU32 color = IM_COL32(255, 50, 50, (int)(damage.alpha * 255));
                ImU32 shadowColor = IM_COL32(0, 0, 0, (int)(damage.alpha * 255));
                
                ImVec2 pos = ImVec2(screenPos.x, screenPos.y);

                // Draw bold shadow/stroke (4-way offset)
                drawList->AddText(ImVec2(pos.x + 1, pos.y), shadowColor, text.c_str());
                drawList->AddText(ImVec2(pos.x - 1, pos.y), shadowColor, text.c_str());
                drawList->AddText(ImVec2(pos.x, pos.y + 1), shadowColor, text.c_str());
                drawList->AddText(ImVec2(pos.x, pos.y - 1), shadowColor, text.c_str());
                
                // Draw main text
                drawList->AddText(pos, color, text.c_str());
            }
        }
    }
}
