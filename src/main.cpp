// Note: stdafx.h is included automatically by CMake via
// target_precompile_headers
#include "proc.h"
#include <iomanip>

int main() {
  std::cout << "--- Process Memory Wrapper Test ---" << std::endl;

  const wchar_t *targetProc = L"ac_client.exe";
  
  Process target(targetProc);

  std::cout << "\nPress Enter to exit..." << std::endl;
  std::cin.get();

  return 0;
}
