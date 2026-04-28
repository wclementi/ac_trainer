#include "game.h"
#include "overlay.h"
#include "esp.h"
#include "aimbot.h"
#include "effects.h"
#include "config.h"
#include "menu.h"
#include <iostream>

Config config;

#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

std::mutex gameMutex;
GameState sharedState;
std::atomic<bool> running = true;

void LogicLoop(Game& game, Aimbot& aimbot, int width, int height) {
    while (running) {
        if (game.Update(width, height)) {
            std::lock_guard<std::mutex> lock(gameMutex);
            sharedState = game.GetState();
        }
        aimbot.Update();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

int main() {
    // Set modern DPI awareness
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    
    std::cout << "--- Assault Cube External Multi-Threaded Trainer ---" << std::endl;

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
    Aimbot aimbot(game, target);
    VisualEffects effects;
    Menu menu;

    std::cout << "[+] Overlay initialized. Trainer is active!" << std::endl;
    std::cout << "[+] Menu trigger: [INSERT]" << std::endl;
    std::cout << "[+] Exit trainer: [DELETE] (Hold)" << std::endl;

    // Start Logic Thread
    std::thread logicThread(LogicLoop, std::ref(game), std::ref(aimbot), overlay.GetWidth(), overlay.GetHeight());

    auto lastFrameTime = std::chrono::high_resolution_clock::now();
    bool insertPressed = false;

    while (overlay.StartFrame()) {
        if (GetAsyncKeyState(VK_DELETE) & 0x8000) {
            running = false;
            break;
        }

        // Toggle menu
        bool insertDown = (GetAsyncKeyState(VK_INSERT) & 0x8000);
        if (insertDown && !insertPressed) {
            config.menuVisible = !config.menuVisible;
            overlay.SetInteractive(config.menuVisible);
        }
        insertPressed = insertDown;

        auto currentFrameTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentFrameTime - lastFrameTime).count();
        lastFrameTime = currentFrameTime;

        overlay.UpdateOverlayPosition(targetHwnd);

        // UI Loop
        GameState localState;
        {
            std::lock_guard<std::mutex> lock(gameMutex);
            localState = sharedState;
        }

        // Process damage events from the logic thread
        for (const auto& event : localState.damageEvents) {
            effects.AddDamage(event.pos, event.amount);
        }

        esp.Render(localState);
        effects.Update(deltaTime);
        effects.Render(localState.viewMatrix, overlay.GetWidth(), overlay.GetHeight(), aimbot.GetFov(), localState.gameFov);
        
        menu.Render();

        overlay.EndFrame();
        // UI thread sleeps less to stay responsive
        std::this_thread::yield();
    }

    running = false;
    if (logicThread.joinable()) logicThread.join();

    return 0;
}
