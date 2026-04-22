// Note: stdafx.h is included automatically by CMake via target_precompile_headers
#include "proc.h"
#include <iomanip>

int main() {
    std::cout << "--- Process Memory Wrapper Test ---" << std::endl;
    
    const wchar_t* targetProc = L"ac_client.exe";
    std::wcout << L"Attempting to attach to " << targetProc << L"..." << std::endl;
    
    // Instantiate the Process object
    Process target(targetProc);
    
    if (target.IsAttached()) {
        std::cout << "[+] Successfully attached to process!" << std::endl;
        std::cout << "[+] Process ID: " << target.GetId() << std::endl;
        std::cout << "[+] Handle: " << target.GetHandle() << std::endl;
        
        // Test: Get Module Base Address
        std::cout << "\n--- Testing GetModuleBaseAddress ---" << std::endl;
        uintptr_t baseAddr = target.GetModuleBaseAddress(targetProc);
        if (baseAddr) {
            std::wcout << L"[+] " << targetProc << L" base address: 0x" << std::hex << baseAddr << std::dec << std::endl;
            
            // Test: Read Memory (reading the DOS header 'MZ' magic number from base address)
            std::cout << "\n--- Testing Read<T> ---" << std::endl;
            uint16_t magicNumber = target.Read<uint16_t>(baseAddr);
            std::cout << "[+] Reading first 2 bytes from base address (should be 0x5A4D / 'MZ'): 0x" 
                      << std::hex << magicNumber << std::dec << std::endl;
            
            // Test: Resolving Local Player Health (Assault Cube 1.3.0.2)
            std::cout << "\n--- Testing Local Player Health (FindDMAAddy + Read/Write) ---" << std::endl;
            
            // The static offset to the local player object from the module base
            uintptr_t localPlayerPtrAddr = baseAddr + 0x10F4F4; 
            
            // The health variable is at offset 0xF8 within the local player object
            std::vector<unsigned int> healthOffsets = {0xF8}; 
            
            // Resolve the full DMA address pointing to the actual health variable
            uintptr_t healthAddr = target.FindDMAAddy(localPlayerPtrAddr, healthOffsets);
            
            if (healthAddr) {
                std::cout << "[+] Resolved Health Address: 0x" << std::hex << healthAddr << std::dec << std::endl;
                
                // Read current health
                int currentHealth = target.Read<int>(healthAddr);
                std::cout << "[+] Current Health: " << currentHealth << std::endl;
                
                // Write new health (e.g., God Mode / 1337 health)
                int newHealth = 1337;
                if (target.Write<int>(healthAddr, newHealth)) {
                    std::cout << "[+] Successfully wrote " << newHealth << " to Health!" << std::endl;
                    
                    // Verify the write
                    int verifiedHealth = target.Read<int>(healthAddr);
                    std::cout << "[+] Verified New Health: " << verifiedHealth << std::endl;
                } else {
                    std::cout << "[-] Failed to write to Health address." << std::endl;
                }
            } else {
                std::cout << "[-] Failed to resolve Health pointer chain. (Is the player spawned in a map?)" << std::endl;
            }

        } else {
            std::cout << "[-] Failed to find module base address. (Is the target a 64-bit process while we are 32-bit?)" << std::endl;
        }

    } else {
        std::wcout << L"[-] Failed to attach. Ensure " << targetProc << L" is running and you have Administrator privileges." << std::endl;
    }
    
    std::cout << "\nPress Enter to exit..." << std::endl;
    std::cin.get();
    
    return 0;
}
