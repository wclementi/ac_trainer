// Note: stdafx.h is included automatically by CMake via target_precompile_headers
#include "proc.h"
#include "entity.h"
#include "offsets.h"
#include "overlay.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    // Ensure the process is DPI aware so our coordinates match the game's window exactly
    SetProcessDPIAware();
    
    std::cout << "--- Assault Cube External ESP ---" << std::endl;

    const wchar_t* targetProc = L"ac_client.exe";
    Process target(targetProc);

    if (!target.IsAttached()) {
        std::wcout << L"[-] Failed to attach to " << targetProc << L". Ensure the game is running." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return 1;
    }

    std::cout << "[+] Successfully attached to process!" << std::endl;

    uintptr_t baseAddr = target.GetModuleBaseAddress(targetProc);
    if (!baseAddr) {
        std::cout << "[-] Failed to find module base address." << std::endl;
        return 1;
    }

    std::wcout << L"[+] " << targetProc << L" base address: 0x" << std::hex << baseAddr << std::dec << std::endl;

    HWND targetHwnd = FindWindowW(nullptr, L"AssaultCube");
    if (!targetHwnd) {
        std::cout << "[-] Failed to find AssaultCube window." << std::endl;
        return 1;
    }

    Overlay overlay(L"AC_ESP_Overlay", L"AC_ESP_Class");
    std::cout << "[+] Overlay initialized. ESP is active!" << std::endl;

    while (overlay.StartFrame()) {
        // Track the game window
        overlay.UpdateOverlayPosition(targetHwnd);

        // Read global variables
        int playerCount = target.Read<int>(baseAddr + offsets::playerCount);
        uint32_t entList = target.Read<uint32_t>(baseAddr + offsets::entityList);
        uint32_t localPlayer = target.Read<uint32_t>(baseAddr + offsets::localPlayer);

        float matrix[16];
        if (target.GetHandle()) {
            ReadProcessMemory(target.GetHandle(), (LPCVOID)(baseAddr + offsets::viewMatrix), &matrix, sizeof(matrix), nullptr);
        }

        // Validate local player
        if (!localPlayer) {
            overlay.EndFrame();
            continue;
        }
        int localTeam = target.Read<int>(localPlayer + offsets::team);

        // Loop through entities (1 to playerCount)
        for (int i = 1; i < playerCount; ++i) {
            uint32_t entPtr = target.Read<uint32_t>(entList + i * 4);
            if (!entPtr) continue;

            Entity ent;
            ent.health = target.Read<int>(entPtr + offsets::health);
            
            // Basic alive check
            if (ent.health <= 0 || ent.health > 100) continue;

            ent.team = target.Read<int>(entPtr + offsets::team);
            ent.headPos = target.Read<Vec3>(entPtr + offsets::headPos);
            ent.feetPos = target.Read<Vec3>(entPtr + offsets::feetPos);

            // Slightly increase head height for the box top
            Vec3 topPos = ent.headPos;
            topPos.z += 0.5f;

            Vec2 screenTop, screenBot;
            if (WorldToScreen(topPos, screenTop, matrix, overlay.GetWidth(), overlay.GetHeight()) &&
                WorldToScreen(ent.feetPos, screenBot, matrix, overlay.GetWidth(), overlay.GetHeight())) {

                float height = screenBot.y - screenTop.y;
                float width = height / 2.0f;

                // Color based on team
                ImU32 boxColor = (ent.team == localTeam) ? IM_COL32(0, 255, 0, 255) : IM_COL32(255, 0, 0, 255);

                // Draw Box
                ImGui::GetBackgroundDrawList()->AddRect(
                    ImVec2(screenTop.x - width / 2, screenTop.y),
                    ImVec2(screenTop.x + width / 2, screenBot.y),
                    boxColor,
                    0.0f, 0, 1.5f
                );

                // Draw Health Bar
                float healthPct = ent.health / 100.0f;
                float healthHeight = height * healthPct;
                
                // Red background for lost health
                ImGui::GetBackgroundDrawList()->AddRectFilled(
                    ImVec2(screenTop.x - width / 2 - 5, screenTop.y),
                    ImVec2(screenTop.x - width / 2 - 2, screenBot.y),
                    IM_COL32(255, 0, 0, 255)
                );
                
                // Green foreground for current health
                ImGui::GetBackgroundDrawList()->AddRectFilled(
                    ImVec2(screenTop.x - width / 2 - 5, screenBot.y - healthHeight),
                    ImVec2(screenTop.x - width / 2 - 2, screenBot.y),
                    IM_COL32(0, 255, 0, 255)
                );
            }
        }

        overlay.EndFrame();
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Save some CPU
    }

    return 0;
}
