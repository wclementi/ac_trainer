#pragma once

// Note: std::vector and Windows headers are precompiled via stdafx.h, 
// but including them here makes the header self-contained.
#include <vector>
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

/**
 * @brief Handles reading and writing memory for a specific target process.
 */
class Process {
private:
    DWORD procId;               ///< Process ID of the attached target
    HANDLE hProcess;            ///< Handle to the attached process
    uintptr_t baseAddress;      ///< Base memory address of the primary module

    /**
     * @brief Internal helper to find the process ID by its executable name.
     * @param procName The name of the process (e.g., L"ac_client.exe").
     * @return The Process ID, or 0 if not found.
     */
    DWORD GetProcId(const wchar_t* procName);

public:
    /**
     * @brief Attaches to a process by its name and opens a handle.
     * @param procName The executable name of the target process.
     */
    Process(const wchar_t* procName);

    /**
     * @brief Destructor that cleanly closes the process handle.
     */
    ~Process();

    /**
     * @brief Retrieves the base address of a specific module within the process.
     * @param modName The name of the module (e.g., L"ac_client.exe").
     * @return The base address, or 0 if not found.
     */
    uintptr_t GetModuleBaseAddress(const wchar_t* modName);

    /**
     * @brief Resolves a multi-level pointer using a base address and a list of offsets.
     * @param ptr The starting base address.
     * @param offsets A vector of offsets to follow.
     * @return The final resolved memory address.
     */
    uintptr_t FindDMAAddy(uintptr_t ptr, const std::vector<unsigned int>& offsets);

    /**
     * @brief Reads a specific type of data from a memory address.
     * @tparam T The type of data to read.
     * @param address The target memory address.
     * @return The value read from memory. Returns a default-constructed T on failure.
     */
    template <typename T>
    T Read(uintptr_t address) {
        T value{};
        if (hProcess) {
            ReadProcessMemory(hProcess, (LPCVOID)address, &value, sizeof(T), nullptr);
        }
        return value;
    }

    /**
     * @brief Writes a specific type of data to a memory address.
     * @tparam T The type of data to write.
     * @param address The target memory address.
     * @param value The value to write.
     * @return True if the write was successful, false otherwise.
     */
    template <typename T>
    bool Write(uintptr_t address, const T& value) {
        if (hProcess) {
            return WriteProcessMemory(hProcess, (LPVOID)address, &value, sizeof(T), nullptr) != 0;
        }
        return false;
    }

    /**
     * @brief Reads a block of memory into a raw buffer.
     * @param address The target memory address.
     * @param buffer Pointer to the destination buffer.
     * @param size Number of bytes to read.
     * @return True if the read was successful, false otherwise.
     */
    bool ReadBuffer(uintptr_t address, void* buffer, size_t size) {
        if (hProcess) {
            return ReadProcessMemory(hProcess, (LPCVOID)address, buffer, size, nullptr) != 0;
        }
        return false;
    }

    // Accessors
    DWORD GetId() const { return procId; }          ///< Gets the attached Process ID
    HANDLE GetHandle() const { return hProcess; }   ///< Gets the attached Process Handle
    uintptr_t GetBaseAddress() const { return baseAddress; } ///< Gets the primary module base address
    bool IsAttached() const { return hProcess != nullptr && hProcess != INVALID_HANDLE_VALUE; } ///< Checks if attached
};
