// Note: stdafx.h is included automatically by CMake via
// target_precompile_headers
#include "proc.h"
#include "entity.h"
#include "offsets.h"
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <conio.h>

int main() {
    std::cout << "--- Assault Cube LocalPlayer Tracker ---" << std::endl;

    const wchar_t *targetProc = L"ac_client.exe";
    Process target(targetProc);

    if (!target.IsAttached()) {
        std::wcout << L"[-] Failed to attach to " << targetProc << L". Ensure the game is running." << std::endl;
        std::cout << "Press any key to exit..." << std::endl;
        _getch();
        return 1;
    }

    std::cout << "[+] Successfully attached to process!" << std::endl;

    uintptr_t baseAddr = target.GetModuleBaseAddress(targetProc);
    if (!baseAddr) {
        std::cout << "[-] Failed to find module base address." << std::endl;
        return 1;
    }

    std::wcout << L"[+] " << targetProc << L" base address: 0x" << std::hex << baseAddr << std::dec << std::endl;

    Entity localPlayer{};
    
    // The local player base pointer is at ac_client.exe + offsets::localPlayer
    localPlayer.baseAddress = target.Read<uintptr_t>(baseAddr + offsets::localPlayer);

    if (!localPlayer.baseAddress) {
        std::cout << "[-] Failed to resolve local player base address." << std::endl;
        return 1;
    }

    std::cout << "[+] Local Player Address: 0x" << std::hex << localPlayer.baseAddress << std::dec << std::endl;
    std::cout << "\nStarting live tracker. Press any key to stop...\n" << std::endl;
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    _getch();
    while (!_kbhit()) {
        // Read memory into localPlayer object
        localPlayer.health = target.Read<int>(localPlayer.baseAddress + offsets::health);
        localPlayer.armor = target.Read<int>(localPlayer.baseAddress + offsets::armor);
        localPlayer.team = target.Read<int>(localPlayer.baseAddress + offsets::team);
        
        localPlayer.headPos = target.Read<Vec3>(localPlayer.baseAddress + offsets::headPos);
        localPlayer.feetPos = target.Read<Vec3>(localPlayer.baseAddress + offsets::feetPos);
        localPlayer.viewAngles = target.Read<Vec3>(localPlayer.baseAddress + offsets::viewAngles);

        // Name is an array of chars, read manually
        ReadProcessMemory(target.GetHandle(), (LPCVOID)(localPlayer.baseAddress + offsets::name), localPlayer.name, sizeof(localPlayer.name), nullptr);
        localPlayer.name[15] = '\0'; // Ensure null termination

        // Clear console screen (Windows specific)
        system("cls");

        std::cout << "=== Local Player Info ===" << std::endl;
        std::cout << "Name: " << localPlayer.name << std::endl;
        std::cout << "Team: " << localPlayer.team << std::endl;
        std::cout << "Health: " << localPlayer.health << std::endl;
        std::cout << "Armor: " << localPlayer.armor << std::endl;
        std::cout << "Alive: " << (localPlayer.IsAlive() ? "Yes" : "No") << std::endl;
        
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "\nHead Position: X: " << localPlayer.headPos.x 
                  << " | Y: " << localPlayer.headPos.y 
                  << " | Z: " << localPlayer.headPos.z << std::endl;
                  
        std::cout << "Feet Position: X: " << localPlayer.feetPos.x 
                  << " | Y: " << localPlayer.feetPos.y 
                  << " | Z: " << localPlayer.feetPos.z << std::endl;

        // Note: ViewAngles might be X=Yaw, Y=Pitch, Z=Roll
        std::cout << "View Angles:   X: " << localPlayer.viewAngles.x 
                  << " | Y: " << localPlayer.viewAngles.y 
                  << " | Z: " << localPlayer.viewAngles.z << std::endl;

        std::cout << "\nPress any key to exit..." << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    // Consume the key press that broke the loop
    _getch();

    // Clear screen on exit
    system("cls");
    std::cout << "Exiting live tracker..." << std::endl;

    return 0;
}
