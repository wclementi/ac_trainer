#include "game.h"
#include "overlay.h"
#include "esp.h"
#include "aimbot.h"
#include "effects.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    // Set modern DPI awareness
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    
    std::cout << "--- Assault Cube External ESP ---" << std::endl;

    const wchar_t* targetProc = L"ac_client.exe";
    Process target(targetProc);

    if (!target.IsAttached()) {
        std::wcout << L"[-] Failed to attach to " << targetProc << L". Ensure the game is running." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return 1;
    }

    std::cout << "[+] Successfully attached to process!" << std::endl;

    HWND targetHwnd = FindWindowW(nullptr, L"AssaultCube");
    if (!targetHwnd) {
        std::cout << "[-] Failed to find AssaultCube window." << std::endl;
        return 1;
    }

    Game game(target);
    Overlay overlay(L"AC_ESP_Overlay", L"AC_ESP_Class");
    ESP esp(game, overlay);
    Aimbot aimbot(game, target, 15.0f);
    VisualEffects effects;

    std::cout << "[+] Overlay initialized. Trainer is active!" << std::endl;
    std::cout << "[+] Aimbot trigger: [Left Click] or [Right Click]" << std::endl;
    std::cout << "[+] Exit trainer: [INSERT]" << std::endl;

    auto lastFrameTime = std::chrono::high_resolution_clock::now();

    while (overlay.StartFrame()) {
        if (GetAsyncKeyState(VK_INSERT) & 0x8000) break;

        auto currentFrameTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentFrameTime - lastFrameTime).count();
        lastFrameTime = currentFrameTime;

        overlay.UpdateOverlayPosition(targetHwnd);

        if (game.Update()) {
            const auto& state = game.GetState();
            
            // Process damage events
            for (const auto& event : state.damageEvents) {
                effects.AddDamage(event.pos, event.amount);
            }

            esp.Render();
            effects.Update(deltaTime);
            effects.Render(state.viewMatrix, overlay.GetWidth(), overlay.GetHeight());
            aimbot.Update();
        }

        overlay.EndFrame();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}
