#include "stdafx.h"
#include "Window.h"
#include "Shared.h"
#include "Renderer.h"

#include <assert.h>

Window::Window(Renderer* renderer, uint32_t sizeX, uint32_t sizeY, std::string name) {
    mRenderer = renderer;
    mSurfaceSizeX = sizeX;
    mSurfaceSizeY = sizeY;
    mWindowName = s2ws(name);
    initOSWindow();
    initSurface();
}

Window::~Window() {
    deinitOSWindow();
    deinitSurface();
}

void Window::close() {
    mWindowShouldRun = false;
}

bool Window::update() {
    updateOSWindow();
    return mWindowShouldRun;
}

void Window::initSurface() {
    initOSSurface();

    VkPhysicalDevice gpu = mRenderer->getPhysicalDevice();

    VkBool32 wsiSupported;
    vkGetPhysicalDeviceSurfaceSupportKHR(gpu, mRenderer->getGraphicsQueueFamilyIndex(), mSurface, &wsiSupported);
    if (!wsiSupported) {
        assert(0 && "WSI not supported");
        std::exit(-1);
    }

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, mSurface, &mSurfaceCapabilities);

    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, mSurface, &formatCount, nullptr);
    if (formatCount == 0) {
        assert(0 && "No surface format found");
        std::exit(-1);
    }
    std::vector<VkSurfaceFormatKHR> formats(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, mSurface, &formatCount, formats.data());
    if (formats[0].format == VK_FORMAT_UNDEFINED) {
        mSurfaceFormat.format = VK_FORMAT_B8G8R8A8_UNORM;
        mSurfaceFormat.colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    } else {
        mSurfaceFormat = formats[0];
    }
}

void Window::deinitSurface() {
    vkDestroySurfaceKHR(mRenderer->getVulkanInstance(), mSurface, nullptr);
}
