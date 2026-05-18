#include "proc.h"

// Constructor: Initializes the process ID and opens a handle
Process::Process(const wchar_t *procName) : procId(0), hProcess(nullptr), baseAddress(0) {
  procId = GetProcId(procName);
  if (procId != 0) {
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
    if (hProcess) {
      baseAddress = GetModuleBaseAddress(procName);
    }
  }
}

// Destructor: Closes the process handle to prevent resource leaks
Process::~Process() {
  if (hProcess && hProcess != INVALID_HANDLE_VALUE) {
    CloseHandle(hProcess);
  }
}

// Internal helper to retrieve the process ID by name
DWORD Process::GetProcId(const wchar_t *procName) {
  DWORD pId = 0;
  HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

  if (hSnap != INVALID_HANDLE_VALUE) {
    PROCESSENTRY32W procEntry;
    procEntry.dwSize = sizeof(procEntry);

    if (Process32FirstW(hSnap, &procEntry)) {
      do {
        if (!_wcsicmp(procEntry.szExeFile, procName)) {
          pId = procEntry.th32ProcessID;
          break;
        }
      } while (Process32NextW(hSnap, &procEntry));
    }
    CloseHandle(hSnap);
  }
  return pId;
}

// Retrieves the base address of a module within the attached process
uintptr_t Process::GetModuleBaseAddress(const wchar_t *modName) {
  if (procId == 0)
    return 0;

  uintptr_t modBaseAddr = 0;
  HANDLE hSnap =
      CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);

  if (hSnap != INVALID_HANDLE_VALUE) {
    MODULEENTRY32W modEntry;
    modEntry.dwSize = sizeof(modEntry);

    if (Module32FirstW(hSnap, &modEntry)) {
      do {
        if (!_wcsicmp(modEntry.szModule, modName)) {
          modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
          break;
        }
      } while (Module32NextW(hSnap, &modEntry));
    }
    CloseHandle(hSnap);
  }
  return modBaseAddr;
}

// Resolves a multi-level pointer using DMA offsets
uintptr_t Process::FindDMAAddy(uintptr_t ptr,
                               const std::vector<unsigned int> &offsets) {
  if (!hProcess)
    return 0;

  uintptr_t addr = ptr;
  for (unsigned int i = 0; i < offsets.size(); ++i) {
    ReadProcessMemory(hProcess, (BYTE *)addr, &addr, sizeof(addr), nullptr);
    addr += offsets[i];
  }
  return addr;
}
