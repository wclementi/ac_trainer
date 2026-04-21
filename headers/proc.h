#pragma once

// Note: std::vector and Windows headers are precompiled via stdafx.h, 
// but including them here makes the header self-contained.
#include <vector>
#include <windows.h>
#include <tlhelp32.h>

// Retrieves the process ID of a given process by name
DWORD GetProcId(const wchar_t* procName);

// Retrieves the base address of a module within a given process
uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);

// Resolves a multi-level pointer using DMA (Dynamic Memory Allocation) offsets
uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);
