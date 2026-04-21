// Note: stdafx.h is included automatically by CMake via target_precompile_headers
#include "proc.h"

int main() {
    DWORD procId = GetProcId(L"firefox.exe");
    
    std::cout << "Target Process: firefox.exe" << std::endl;
    std::cout << "Process ID found: " << procId << std::endl;
    
    std::cout << "Press Enter to proceed..." << std::endl;
    std::cin.get();
    
    return 0;
}
