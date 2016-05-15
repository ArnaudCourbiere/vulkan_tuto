#include "stdafx.h"
#include "BUILD_OPTIONS.h"
#include "Platform.h"
#include "Window.h"
#include "Renderer.h"
#include <assert.h>

#if VK_USE_PLATFORM_WIN32_KHR

LRESULT CALLBACK WindowsEventHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Window* window = (Window*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);

    switch (uMsg) {
    case WM_CLOSE:
        window->close();
        return 0;
    case WM_SIZE:
        // Recreate swap chain, blabla
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

uint64_t Window::mWin32ClassIdCounter = 0;

void Window::initOSWindow() {
    WNDCLASSEX winClass{};

    assert(mSurfaceSizeX > 0);
    assert(mSurfaceSizeY > 0);

    mWin32Instance = GetModuleHandle(nullptr);
    mWin32ClassName = mWindowName + L"_" + std::to_wstring(mWin32ClassIdCounter);
    mWin32ClassIdCounter++;

    winClass.cbSize = sizeof(WNDCLASSEX);
    winClass.style = CS_HREDRAW | CS_VREDRAW;
    winClass.lpfnWndProc = WindowsEventHandler;
    winClass.cbClsExtra = 0;
    winClass.cbWndExtra = 0;
    winClass.hInstance = mWin32Instance;
    winClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    winClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    winClass.lpszMenuName = NULL;
    winClass.lpszClassName = mWin32ClassName.c_str();
    winClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

    if (!RegisterClassEx(&winClass)) {
        assert(0 && "Cannot create a window in which to draw!\n");
        fflush(stdout);
        std::exit(-1);
    }

    DWORD exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

    RECT wr = { 0, 0, LONG(mSurfaceSizeX), LONG(mSurfaceSizeY) };
    AdjustWindowRectEx(&wr, style, FALSE, exStyle);
    mWin32Window = CreateWindowEx(0,
                                   mWin32ClassName.c_str(),		    // class name
                                   mWindowName.c_str(),			    // app name
                                   style,							// window style
                                   CW_USEDEFAULT, CW_USEDEFAULT,	// x/y coords
                                   wr.right - wr.left,				// width
                                   wr.bottom - wr.top,				// height
                                   NULL,							// handle to parent
                                   NULL,							// handle to menu
                                   mWin32Instance,				    // hInstance
                                   NULL);							// no extra parameters
    if (!mWin32Window) {
        assert(1 && "Cannot create a window in which to draw!\n");
        fflush(stdout);
        std::exit(-1);
    }

    SetWindowLongPtr(mWin32Window, GWLP_USERDATA, (LONG_PTR)this);

    ShowWindow(mWin32Window, SW_SHOW);
    SetForegroundWindow(mWin32Window);
    SetFocus(mWin32Window);

}

void Window::deinitOSWindow() {
    DestroyWindow(mWin32Window);
    UnregisterClass(mWin32ClassName.c_str(), mWin32Instance);
}

void Window::updateOSWindow() {
    MSG msg;
    if (PeekMessage(&msg, mWin32Window, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Window::initOSSurface() {
    VkWin32SurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hinstance = mWin32Instance;
    createInfo.hwnd = mWin32Window;
    vkCreateWin32SurfaceKHR(mRenderer->getVulkanInstance(), &createInfo, nullptr, &mSurface);
}

#endif
