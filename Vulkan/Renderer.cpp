#include "stdafx.h"
#include <cstdlib>
#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include "Renderer.h"
#include "Shared.h"
#include "BUILD_OPTIONS.h"
#include "Platform.h"
#include "Window.h"

Renderer::Renderer() {
    setupLayersAndExtensions();
    setupDebug();
    initInstance();
    initDebug();
    initDevice();
}


Renderer::~Renderer() {
    delete mWindow;
    deInitDevice();
    deinitDebug();
    deInitInstance();
}

Window * Renderer::openWindow(uint32_t w, uint32_t h, std::string name) {
    mWindow = new Window(this, w, h, name);
    return mWindow;
}

bool Renderer::run() {
    if (mWindow != nullptr) {
        return mWindow->update();
    }

    return true;
}

const VkInstance Renderer::getVulkanInstance() const {
    return mInstance;
}

const VkPhysicalDevice Renderer::getPhysicalDevice() const {
    return mGpu;
}

const VkDevice Renderer::getDevice() const {
    return mDevice;
}

const VkQueue Renderer::getQueue() const {
    return mGraphicsQueue;
}

const uint32_t Renderer::getGraphicsQueueFamilyIndex() const {
    return mGraphicsFamilyIndex;
}

const VkPhysicalDeviceProperties & Renderer::getPhysicalDeviceProperties() const {
    return mGpuProperties;
}

void Renderer::setupLayersAndExtensions() {
    mInstanceExtensionList.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    mInstanceExtensionList.push_back(PLATFORM_SURFACE_EXTENSION_NAME);

    mDeviceExtensionList.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
}

void Renderer::initInstance() {
    VkApplicationInfo applicationInfo{};
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.apiVersion = VK_MAKE_VERSION(1, 0, 11);
    applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    applicationInfo.pApplicationName = "Vulkan Test Application";

    VkInstanceCreateInfo instanceCreateInfo{};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;
    instanceCreateInfo.enabledLayerCount = mInstanceLayerList.size();
    instanceCreateInfo.ppEnabledLayerNames = mInstanceLayerList.data();
    instanceCreateInfo.enabledExtensionCount = mInstanceExtensionList.size();
    instanceCreateInfo.ppEnabledExtensionNames = mInstanceExtensionList.data();
    instanceCreateInfo.pNext = &mDebugCallbackCreateInfo;

    errorCheck(vkCreateInstance(&instanceCreateInfo, nullptr, &mInstance));
}

void Renderer::deInitInstance() {
    vkDestroyInstance(mInstance, nullptr);
    mInstance = VK_NULL_HANDLE;
}

void Renderer::initDevice() {
    uint32_t gpuCount;
    errorCheck(vkEnumeratePhysicalDevices(mInstance, &gpuCount, nullptr));

    std::vector<VkPhysicalDevice> gpus(gpuCount);
    errorCheck(vkEnumeratePhysicalDevices(mInstance, &gpuCount, gpus.data()));

    printf("GPUS:\n");
    for (auto it = gpus.begin(); it < gpus.end(); it++) {
        checkDeviceProperties(*it);
    }

    mGpu = gpus[0]; // Grabbing the first one (doesn't mean that the first one is the best one)
    vkGetPhysicalDeviceProperties(mGpu, &mGpuProperties);

    uint32_t familyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(mGpu, &familyCount, nullptr);
    std::vector<VkQueueFamilyProperties> familyProperties(familyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(mGpu, &familyCount, familyProperties.data());

    bool found = false;
    for (uint32_t i = 0; i < familyCount; i++) {
        if (familyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            mGraphicsFamilyIndex = i;
            found = true;
            break;
        }
    }

    if (!found) {
        assert(0 && "Couldn't find a graphics queue");
        std::exit(-1);
    }

    // Vulkan layers enumeration example.
    /*{
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> layerProperties(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, layerProperties.data());
        printf("Instance layers installed:\n");
        for (auto &i : layerProperties) {
            printf("  %s\t\t\t | %s\n", i.layerName, i.description);
        }
    }
    {
        uint32_t layerCount;
        vkEnumerateDeviceLayerProperties(mGpu, &layerCount, nullptr);
        std::vector<VkLayerProperties> layerProperties(layerCount);
        vkEnumerateDeviceLayerProperties(mGpu, &layerCount, layerProperties.data());
        printf("Device layers installed:\n");
        for (auto &i : layerProperties) {
            printf("  %s\t\t\t | %s\n", i.layerName, i.description);
        }
    }*/

    float queuePriorities[]{ 1.0 };
    VkDeviceQueueCreateInfo deviceQueueCreateInfo{};
    deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    deviceQueueCreateInfo.queueFamilyIndex = mGraphicsFamilyIndex;
    deviceQueueCreateInfo.queueCount = 1;
    deviceQueueCreateInfo.pQueuePriorities = queuePriorities;

    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
    deviceCreateInfo.enabledLayerCount = mDeviceLayerList.size();
    deviceCreateInfo.ppEnabledLayerNames = mDeviceLayerList.data();
    deviceCreateInfo.enabledExtensionCount = mDeviceExtensionList.size();
    deviceCreateInfo.ppEnabledExtensionNames = mDeviceExtensionList.data();

    errorCheck(vkCreateDevice(mGpu, &deviceCreateInfo, nullptr, &mDevice));
    vkGetDeviceQueue(mDevice, mGraphicsFamilyIndex, 0, &mGraphicsQueue);
}

void Renderer::deInitDevice() {
    vkDeviceWaitIdle(mDevice);
    vkDestroyDevice(mDevice, nullptr);
    mDevice = VK_NULL_HANDLE;
}

#if BUILD_ENABLE_VULKAN_DEBUG

VKAPI_ATTR VkBool32 VKAPI_CALL
VulkanDebugCallback(VkDebugReportFlagsEXT flags,
                    VkDebugReportObjectTypeEXT objType,
                    uint64_t srcObj,
                    size_t location,
                    int32_t msgCode,
                    const char* layerPrefix,
                    const char* msg,
                    void* userData) {

    std::ostringstream stream;
    stream << "VKDBG: ";
    if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
        stream << "INFO: ";
    }
    if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
        stream << "WARNING: ";
    }
    if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
        stream << "PERFORMANCE: ";
    }
    if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
        stream << "ERROR: ";
    }
    if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
        stream << "DEBUG: ";
    }

    stream << "@[" << layerPrefix << "]: " << msg << std::endl;
    std::cout << stream.str();

#ifdef _WIN32
    if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
        MessageBox(NULL, s2ws(stream.str()).c_str(), L"Vulkan Error!", 0);
    }
#endif

    return false;
}

void Renderer::setupDebug() {
    mDebugCallbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    mDebugCallbackCreateInfo.pfnCallback = VulkanDebugCallback;
    mDebugCallbackCreateInfo.flags =
        //VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
        VK_DEBUG_REPORT_WARNING_BIT_EXT |
        VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
        VK_DEBUG_REPORT_ERROR_BIT_EXT |
        //VK_DEBUG_REPORT_DEBUG_BIT_EXT |
        //VK_DEBUG_REPORT_FLAG_BITS_MAX_ENUM_EXT |
        0;

    mInstanceLayerList.push_back("VK_LAYER_LUNARG_standard_validation");
    mInstanceExtensionList.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    mDeviceLayerList.push_back("VK_LAYER_LUNARG_standard_validation");
}

PFN_vkCreateDebugReportCallbackEXT fvkCreateDebugReportCallbackEXT = nullptr;
PFN_vkDestroyDebugReportCallbackEXT fvkDestroyDebugReportCallbackEXT = nullptr;

void Renderer::initDebug() {
    fvkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(mInstance, "vkCreateDebugReportCallbackEXT");
    fvkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(mInstance, "vkDestroyDebugReportCallbackEXT");

    if (fvkCreateDebugReportCallbackEXT == nullptr || fvkDestroyDebugReportCallbackEXT == nullptr) {
        assert(0 && "Error querying debug report functions");
        std::exit(-1);
    }

    errorCheck(fvkCreateDebugReportCallbackEXT(mInstance, &mDebugCallbackCreateInfo, nullptr, &mDebugReport));
}

void Renderer::deinitDebug() {
    fvkDestroyDebugReportCallbackEXT(mInstance, mDebugReport, nullptr);
    mDebugReport = VK_NULL_HANDLE;
}

#else

void Renderer::setupDebug() {};
void Renderer::initDebug() {};
void Renderer::deinitDebug() {};

#endif // BUILD_ENABLE_VULKAN_DEBUG

void Renderer::checkDeviceProperties(VkPhysicalDevice gpu) {
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;

    vkGetPhysicalDeviceProperties(gpu, &properties);
    vkGetPhysicalDeviceFeatures(gpu, &features);

    printf("%s\n", properties.deviceName);
    printf("Vulkan Version: %d.%d.%d\n",
           VK_VERSION_MAJOR(properties.apiVersion),
           VK_VERSION_MINOR(properties.apiVersion),
           VK_VERSION_PATCH(properties.apiVersion));
    printf("Driver Version: %d.%d.%d\n\n",
           VK_VERSION_MAJOR(properties.driverVersion),
           VK_VERSION_MINOR(properties.driverVersion),
           VK_VERSION_PATCH(properties.driverVersion));
}
