#pragma once

#include "Platform.h"
#include <string>

class Renderer;

class Window {
public:
    Window(Renderer* renderer, uint32_t sizeX, uint32_t sizeY, std::string name);
    ~Window();

    void close();
    bool update();
private:
    void initOSWindow();
    void deinitOSWindow();
    void updateOSWindow();
    void initOSSurface();
    void initSurface();
    void deinitSurface();

    Renderer* mRenderer = nullptr;

    VkSurfaceKHR mSurface = VK_NULL_HANDLE;

    uint32_t mSurfaceSizeX = 512;
    uint32_t mSurfaceSizeY = 512;
    VkSurfaceFormatKHR mSurfaceFormat = {};
    VkSurfaceCapabilitiesKHR mSurfaceCapabilities = {};
    bool mWindowShouldRun = true;

#if VK_USE_PLATFORM_WIN32_KHR
    std::wstring mWindowName;
    HINSTANCE mWin32Instance = NULL;
    HWND mWin32Window = NULL;
    std::wstring mWin32ClassName;
    static uint64_t mWin32ClassIdCounter;
#elif VK_USE_PLATFORM_XCB_KHR
    std::string mWindowName;
    xcb_connection* mXcbConnection = nullptr;
    xcb_screen_t* mXcbScreen = nullptr;
    xcb_window_t* mXcbWindow = 0;
    xcb_intern_atom_reply_t* mXcbAtomWindowReply = nullptr;
#endif
};

