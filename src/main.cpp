// Note: stdafx.h is included automatically by CMake via target_precompile_headers
#include "proc.h"
#include <iomanip>

int main() {
    std::cout << "--- Process Memory Wrapper Test ---" << std::endl;
    
    const wchar_t* targetProc = L"firefox.exe";
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
            
            // Test: Dummy DMA Addy simulation
            std::cout << "\n--- Testing FindDMAAddy (Information only) ---" << std::endl;
            std::cout << "[*] To resolve a pointer chain, use: target.FindDMAAddy(baseAddr + ptrOffset, {offset1, offset2});" << std::endl;
            std::cout << "[*] Example: target.FindDMAAddy(baseAddr + 0x10f4f4, {0x374, 0x14, 0x0});" << std::endl;

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
