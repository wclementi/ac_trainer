#pragma once

#include <windows.h>
#include <d3d11.h>
#include <string>

// Include ImGui
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

class Overlay {
private:
    HWND hwnd = nullptr;
    WNDCLASSEXW wc{};
    
    // DirectX 11
    ID3D11Device* g_pd3dDevice = nullptr;
    ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
    IDXGISwapChain* g_pSwapChain = nullptr;
    ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
    
    int screenWidth = 1920;
    int screenHeight = 1080;

    bool CreateDeviceD3D();
    void CleanupDeviceD3D();
    void CreateRenderTarget();
    void CleanupRenderTarget();

public:
    Overlay(const wchar_t* title, const wchar_t* className);
    ~Overlay();

    // Returns false if the overlay should be closed
    bool StartFrame();
    void EndFrame();

    // Resize and move overlay to match target game window
    void UpdateOverlayPosition(HWND targetHwnd);

    // Getters
    HWND GetHwnd() const { return hwnd; }
    int GetWidth() const { return screenWidth; }
    int GetHeight() const { return screenHeight; }
};
