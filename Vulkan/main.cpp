// main.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "Renderer.h"
#include "Shared.h"
#include <process.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
BOOL                CreateConsole();
BOOL                CloseConsole();
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

FILE* fStdOut;
FILE* fStdIn;
FILE* fStdErr;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef _DEBUG
    CreateConsole();
#endif

    Renderer* renderer = new Renderer();
    renderer->openWindow(800, 600, "Vulkan");
    while (renderer->run()) {}
    delete renderer;

    /*
    VkDevice device = renderer.mDevice;
    VkQueue queue = renderer.mGraphicsQueue;

    VkFence fence;
    VkFenceCreateInfo fenceCreateInfo{};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    errorCheck(vkCreateFence(device, &fenceCreateInfo, nullptr, &fence));

    VkSemaphore semaphore;
    VkSemaphoreCreateInfo semaphoreCreateInfo{};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    errorCheck(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &semaphore));

    VkCommandPool commandPool;
    VkCommandPoolCreateInfo poolCreateInfo{};
    poolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolCreateInfo.queueFamilyIndex = renderer.mGraphicsFamilyIndex;
    poolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    errorCheck(vkCreateCommandPool(device, &poolCreateInfo, nullptr, &commandPool));

    VkCommandBuffer commandBuffer[2];
    VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.commandPool = commandPool;
    commandBufferAllocateInfo.commandBufferCount = 2;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    errorCheck(vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, commandBuffer));

    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        errorCheck(vkBeginCommandBuffer(commandBuffer[0], &beginInfo));

        vkCmdPipelineBarrier(commandBuffer[0],
                             VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                             VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                             0,
                             0, nullptr,
                             0, nullptr,
                             0, nullptr);

        VkViewport viewport{};
        viewport.maxDepth = 1.0f;
        viewport.minDepth = 0.0f;
        viewport.width = 512;
        viewport.height = 512;
        viewport.x = 0;
        viewport.y = 0;
        vkCmdSetViewport(commandBuffer[0], 0, 1, &viewport);

        errorCheck(vkEndCommandBuffer(commandBuffer[0]));
    }
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        errorCheck(vkBeginCommandBuffer(commandBuffer[1], &beginInfo));

        VkViewport viewport{};
        viewport.maxDepth = 1.0f;
        viewport.minDepth = 0.0f;
        viewport.width = 512;
        viewport.height = 512;
        viewport.x = 0;
        viewport.y = 0;
        vkCmdSetViewport(commandBuffer[1], 0, 1, &viewport);

        errorCheck(vkEndCommandBuffer(commandBuffer[1]));
    }

    {
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer[0];
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &semaphore;
        errorCheck(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));
    }
    {
        VkPipelineStageFlags flags[]{ VK_PIPELINE_STAGE_ALL_COMMANDS_BIT };
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer[1];
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &semaphore;
        submitInfo.pWaitDstStageMask = flags;
        errorCheck(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));
    }

    //vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX);
    vkQueueWaitIdle(queue);

    vkDestroyCommandPool(device, commandPool, nullptr);
    vkDestroySemaphore(device, semaphore, nullptr);
    vkDestroyFence(device, fence, nullptr);
    */

    // TODO: Place code here.

    /*
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_VULKAN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VULKAN));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Render frame
        // Swap buffer
    }

    return (int) msg.wParam;
    */

#ifdef _DEBUG
    CloseConsole();
#endif
    return 0;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VULKAN));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_VULKAN);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

BOOL CreateConsole() {
    BOOL success = AllocConsole();

    if (!success) {
        return false;
    }

    freopen_s(&fStdOut, "CONOUT$", "w", stdout);
    freopen_s(&fStdErr, "CONOUT$", "w", stderr);
    freopen_s(&fStdIn, "CONIN", "r", stdin);

    return true;
}

BOOL CloseConsole() {
    //fclose(fStdOut);
    //fclose(fStdErr);
    //fclose(fStdIn);

    //system("pause");
    return FreeConsole();
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId) {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
