#include "stdafx.h"
#include "menu.h"
#include <imgui.h>

void Menu::Render() {
    if (!config.menuVisible) return;

    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Assault Cube Trainer", &config.menuVisible)) {
        
        if (ImGui::BeginTabBar("Tabs")) {
            
            if (ImGui::BeginTabItem("Visuals")) {
                ImGui::Checkbox("Master ESP", &config.espEnabled);
                ImGui::Separator();
                
                ImGui::BeginDisabled(!config.espEnabled);
                ImGui::Checkbox("Boxes", &config.espBoxes);
                ImGui::Checkbox("Names", &config.espNames);
                ImGui::Checkbox("Health Bar", &config.espHealth);
                ImGui::Checkbox("Snaplines", &config.espSnaplines);
                ImGui::EndDisabled();
                
                ImGui::Separator();
                ImGui::Checkbox("Hitmarkers", &config.hitmarkers);
                ImGui::Checkbox("Damage Numbers", &config.damageNumbers);
                
                ImGui::EndTabItem();
            }
            
            if (ImGui::BeginTabItem("Aimbot")) {
                ImGui::Checkbox("Enabled", &config.aimbotEnabled);
                ImGui::SliderFloat("FOV", &config.aimbotFov, 1.0f, 180.0f, "%.1f");
                ImGui::Checkbox("Draw FOV Circle", &config.drawFov);
                
                ImGui::EndTabItem();
            }
            
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}
