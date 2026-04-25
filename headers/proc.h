#pragma once

// Note: std::vector and Windows headers are precompiled via stdafx.h, 
// but including them here makes the header self-contained.
#include <vector>
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

class Process {
private:
    DWORD procId;
    HANDLE hProcess;

    // Internal helper to find the process ID by name
    DWORD GetProcId(const wchar_t* procName);

public:
    // Constructor initializes the process ID and Handle
    Process(const wchar_t* procName);

    // Destructor cleanly closes the handle
    ~Process();

    // Retrieves the base address of a module within the attached process
    uintptr_t GetModuleBaseAddress(const wchar_t* modName);

    // Resolves a multi-level pointer using DMA (Dynamic Memory Allocation) offsets
    uintptr_t FindDMAAddy(uintptr_t ptr, const std::vector<unsigned int>& offsets);

    // Read memory wrapper
    template <typename T>
    T Read(uintptr_t address) {
        T value{};
        if (hProcess) {
            ReadProcessMemory(hProcess, (LPCVOID)address, &value, sizeof(T), nullptr);
        }
        return value;
    }

    // Write memory wrapper
    template <typename T>
    bool Write(uintptr_t address, const T& value) {
        if (hProcess) {
            return WriteProcessMemory(hProcess, (LPVOID)address, &value, sizeof(T), nullptr) != 0;
        }
        return false;
    }

    // Accessors
    DWORD GetId() const { return procId; }
    HANDLE GetHandle() const { return hProcess; }
    bool IsAttached() const { return hProcess != nullptr; }
};
