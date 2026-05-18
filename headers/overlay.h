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

/**
 * @brief Manages a transparent DirectX 11 overlay window.
 * 
 * Creates an invisible window that stays on top of the target game window,
 * allowing ImGui to render ESP and menus over it.
 */
class Overlay {
private:
    HWND hwnd = nullptr;        ///< Handle to the overlay window
    WNDCLASSEXW wc{};           ///< Window class structure
    
    // DirectX 11 with Smart Pointers
    ComPtr<ID3D11Device> g_pd3dDevice;
    ComPtr<ID3D11DeviceContext> g_pd3dDeviceContext;
    ComPtr<IDXGISwapChain> g_pSwapChain;
    ComPtr<ID3D11RenderTargetView> g_mainRenderTargetView;
    
    int screenWidth = 1920;     ///< Current overlay width
    int screenHeight = 1080;    ///< Current overlay height

    bool CreateDeviceD3D();
    void CleanupDeviceD3D();
    void CreateRenderTarget();
    void CleanupRenderTarget();

public:
    /**
     * @brief Constructor that creates the overlay window.
     * @param title The title of the overlay window.
     * @param className The class name to register for the window.
     */
    Overlay(const wchar_t* title, const wchar_t* className);
    
    /**
     * @brief Destructor that cleans up DirectX and window resources.
     */
    ~Overlay();

    /**
     * @brief Prepares ImGui and DirectX for a new frame.
     * @return False if the window received a quit message, true otherwise.
     */
    bool StartFrame();

    /**
     * @brief Renders the frame and presents the swapchain.
     */
    void EndFrame();

    /**
     * @brief Resizes and repositions the overlay to track the target game window.
     * @param targetHwnd Handle to the target game window.
     */
    void UpdateOverlayPosition(HWND targetHwnd);

    /**
     * @brief Makes the overlay interactive or click-through.
     * @param interactive If true, the overlay catches mouse inputs (for the menu). 
     *                    If false, clicks pass through to the game.
     */
    void SetInteractive(bool interactive);

    // Getters
    HWND GetHwnd() const { return hwnd; }         ///< Returns the overlay window handle
    int GetWidth() const { return screenWidth; }  ///< Returns the current overlay width
    int GetHeight() const { return screenHeight; }///< Returns the current overlay height
};
