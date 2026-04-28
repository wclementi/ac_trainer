#pragma once

#include <windows.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <string>

// Include ImGui
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

using Microsoft::WRL::ComPtr;

class Overlay {
private:
    HWND hwnd = nullptr;
    WNDCLASSEXW wc{};
    
    // DirectX 11 with Smart Pointers
    ComPtr<ID3D11Device> g_pd3dDevice;
    ComPtr<ID3D11DeviceContext> g_pd3dDeviceContext;
    ComPtr<IDXGISwapChain> g_pSwapChain;
    ComPtr<ID3D11RenderTargetView> g_mainRenderTargetView;
    
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
    void SetInteractive(bool interactive);

    // Getters
    HWND GetHwnd() const { return hwnd; }
    int GetWidth() const { return screenWidth; }
    int GetHeight() const { return screenHeight; }
};
