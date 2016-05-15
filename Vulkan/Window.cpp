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
    initSwapChain();
}

Window::~Window() {
    deinitSwapChain();
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
    if (mSurfaceCapabilities.currentExtent.width < UINT32_MAX) {
        mSurfaceSizeX = mSurfaceCapabilities.currentExtent.width;
        mSurfaceSizeY = mSurfaceCapabilities.currentExtent.height;
    }

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

void Window::initSwapChain() {
    if (mSwapchainImageCount > mSurfaceCapabilities.maxImageCount) mSwapchainImageCount = mSurfaceCapabilities.maxImageCount;
    if (mSwapchainImageCount < mSurfaceCapabilities.minImageCount + 1) mSwapchainImageCount = mSurfaceCapabilities.minImageCount + 1;

    VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
    uint32_t presentModeCount = 0;
    errorCheck(vkGetPhysicalDeviceSurfacePresentModesKHR(mRenderer->getPhysicalDevice(), mSurface, &presentModeCount, nullptr));
    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    errorCheck(vkGetPhysicalDeviceSurfacePresentModesKHR(mRenderer->getPhysicalDevice(), mSurface, &presentModeCount, presentModes.data()));
    for (auto pm : presentModes) {
        if (pm == VK_PRESENT_MODE_MAILBOX_KHR) {
            presentMode = pm;
        }
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = mSurface;
    createInfo.minImageCount = mSwapchainImageCount;
    createInfo.imageFormat = mSurfaceFormat.format;
    createInfo.imageColorSpace = mSurfaceFormat.colorSpace;
    createInfo.imageExtent.width = mSurfaceSizeX;
    createInfo.imageExtent.height = mSurfaceSizeY;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices = nullptr;
    createInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    errorCheck(vkCreateSwapchainKHR(mRenderer->getDevice(), &createInfo, nullptr, &mSwapchain));

    errorCheck(vkGetSwapchainImagesKHR(mRenderer->getDevice(), mSwapchain, &mSwapchainImageCount, nullptr));
}

void Window::deinitSwapChain() {
    vkDestroySwapchainKHR(mRenderer->getDevice(), mSwapchain, nullptr);
}
