#include "proc.h"
#include "overlay.h"
#include "esp.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
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

    HWND targetHwnd = FindWindowW(nullptr, L"AssaultCube");
    if (!targetHwnd) {
        std::cout << "[-] Failed to find AssaultCube window." << std::endl;
        return 1;
    }

    Overlay overlay(L"AC_ESP_Overlay", L"AC_ESP_Class");
    ESP esp(target, overlay);

    std::cout << "[+] Overlay initialized. ESP is active!" << std::endl;

    while (overlay.StartFrame()) {
        overlay.UpdateOverlayPosition(targetHwnd);

        esp.Render();

        overlay.EndFrame();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}
