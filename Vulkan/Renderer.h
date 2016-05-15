#pragma once

#include "Platform.h"

#include <vector>

class Window;

class Renderer {
public:
    Renderer();
    ~Renderer();

    Window* openWindow(uint32_t w, uint32_t h, std::string name);
    bool run();

    const VkInstance getVulkanInstance() const;
    const VkPhysicalDevice getPhysicalDevice() const;
    const VkDevice getDevice() const;
    const VkQueue getQueue() const;
    const uint32_t getGraphicsQueueFamilyIndex() const;
    const VkPhysicalDeviceProperties& getPhysicalDeviceProperties() const;

private:
    void setupLayersAndExtensions();
    void initInstance();
    void deInitInstance();

    void initDevice();
    void deInitDevice();

    void setupDebug();
    void initDebug();
    void deinitDebug();

    void checkDeviceProperties(VkPhysicalDevice gpu);

    VkInstance mInstance = VK_NULL_HANDLE;
    VkPhysicalDevice mGpu = VK_NULL_HANDLE;
    VkDevice mDevice = VK_NULL_HANDLE;
    VkQueue mGraphicsQueue = VK_NULL_HANDLE;
    VkPhysicalDeviceProperties mGpuProperties = {};
    uint32_t mGraphicsFamilyIndex = 0;

    Window* mWindow = nullptr;

    std::vector<const char*> mInstanceLayerList;
    std::vector<const char*> mInstanceExtensionList;
    std::vector<const char*> mDeviceLayerList;
    std::vector<const char*> mDeviceExtensionList;

    VkDebugReportCallbackEXT mDebugReport = VK_NULL_HANDLE;
    VkDebugReportCallbackCreateInfoEXT mDebugCallbackCreateInfo = {};
};

