// Note: stdafx.h is included automatically by CMake via target_precompile_headers
#include "proc.h"

int main() {
    std::cout << "Initializing Process memory wrapper..." << std::endl;
    
    // Instantiate the Process object
    Process target(L"firefox.exe");
    
    std::cout << "Target Process: firefox.exe" << std::endl;
    
    if (target.IsAttached()) {
        std::cout << "Successfully attached to process!" << std::endl;
        std::cout << "Process ID found: " << target.GetId() << std::endl;
        std::cout << "Process Handle: " << target.GetHandle() << std::endl;
    } else {
        std::cout << "Failed to attach. Is the process running?" << std::endl;
    }
    
    std::cout << "Press Enter to proceed..." << std::endl;
    std::cin.get();
    
    return 0;
}
