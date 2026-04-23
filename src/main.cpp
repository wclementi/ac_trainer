// Note: stdafx.h is included automatically by CMake via
// target_precompile_headers
#include "proc.h"
#include <iomanip>

int main() {
  std::cout << "--- Process Memory Wrapper Test ---" << std::endl;

  const wchar_t *targetProc = L"ac_client.exe";
  
  Process target(targetProc);

  if (target.IsAttached()) {
    std::cout << "[+] Successfully attached to process!" << std::endl;

    uintptr_t baseAddr = target.GetModuleBaseAddress(targetProc);
    if (baseAddr) {
      std::wcout << L"[+] " << targetProc << L" base address: 0x" << std::hex
                 << baseAddr << std::dec << std::endl;

      // Pointer to player object is ac_client.exe + 0x17E0A8
      uintptr_t playerPtrAddr = baseAddr + 0x17E0A8;

      // Health offset is 0xEC
      std::vector<unsigned int> healthOffsets = {0xEC};

      // Resolve the DMA address
      uintptr_t healthAddr = target.FindDMAAddy(playerPtrAddr, healthOffsets);

      if (healthAddr) {
        std::cout << "[+] Resolved Health Address: 0x" << std::setfill('0')
                  << std::setw(8) << std::hex << healthAddr << std::dec
                  << std::endl;

        // Read current health (4 bytes)
        int currentHealth = target.Read<int>(healthAddr);
        std::cout << "[+] Current Health: " << std::dec << currentHealth
                  << std::endl;
      } else {
        std::cout << "[-] Failed to resolve Health pointer chain." << std::endl;
      }
    } else {
      std::cout << "[-] Failed to find module base address." << std::endl;
    }
  } else {
    std::wcout << L"[-] Failed to attach. Ensure " << targetProc
               << L" is running." << std::endl;
  }

  std::cout << "\nPress Enter to exit..." << std::endl;
  std::cin.get();

  return 0;
}
