#include "stdafx.h"
#include "Shared.h"
#include "BUILD_OPTIONS.h"

#ifdef _WIN32
std::wstring s2ws(const std::string& s) {
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}

#endif

#if BUILD_ENABLE_VULKAN_RUNTIME_DEBUG

void errorCheck(VkResult result) {
    if (result < 0) {
        switch (result) {
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            std::cout << "VK_ERROR_OUT_OF_HOST_MEMORY" << std::endl;
            break;
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            std::cout << "VK_ERROR_OUT_OF_DEVICE_MEMORY" << std::endl;
            break;
        case VK_ERROR_INITIALIZATION_FAILED:
            std::cout << "VK_ERROR_INITIALIZATION_FAILED" << std::endl;
            break;
        case VK_ERROR_DEVICE_LOST:
            std::cout << "VK_ERROR_DEVICE_LOST" << std::endl;
            break;
        case VK_ERROR_MEMORY_MAP_FAILED:
            std::cout << "VK_ERROR_MEMORY_MAP_FAILED" << std::endl;
            break;
        case VK_ERROR_LAYER_NOT_PRESENT:
            std::cout << "VK_ERROR_LAYER_NOT_PRESENT" << std::endl;
            break;
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            std::cout << "VK_ERROR_EXTENSION_NOT_PRESENT" << std::endl;
            break;
        case VK_ERROR_FEATURE_NOT_PRESENT:
            std::cout << "VK_ERROR_FEATURE_NOT_PRESENT" << std::endl;
            break;
        case VK_ERROR_INCOMPATIBLE_DRIVER:
            std::cout << "VK_ERROR_INCOMPATIBLE_DRIVER" << std::endl;
            break;
        case VK_ERROR_TOO_MANY_OBJECTS:
            std::cout << "VK_ERROR_TOO_MANY_OBJECTS" << std::endl;
            break;
        case VK_ERROR_FORMAT_NOT_SUPPORTED:
            std::cout << "VK_ERROR_FORMAT_NOT_SUPPORTED" << std::endl;
            break;
        case VK_ERROR_SURFACE_LOST_KHR:
            std::cout << "VK_ERROR_SURFACE_LOST_KHR" << std::endl;
            break;
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
            std::cout << "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR" << std::endl;
            break;
        case VK_SUBOPTIMAL_KHR:
            std::cout << "VK_SUBOPTIMAL_KHR" << std::endl;
            break;
        case VK_ERROR_OUT_OF_DATE_KHR:
            std::cout << "VK_ERROR_OUT_OF_DATE_KHR" << std::endl;
            break;
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
            std::cout << "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR" << std::endl;
            break;
        case VK_ERROR_VALIDATION_FAILED_EXT:
            std::cout << "VK_ERROR_VALIDATION_FAILED_EXT" << std::endl;
            break;
        case VK_ERROR_INVALID_SHADER_NV:
            std::cout << "VK_ERROR_INVALID_SHADER_NV" << std::endl;
            break;
        default:
            break;
        }
        assert(0 && "Vulkan runtime error.");
    }
}

#else

void errorCheck(VkResult result) {}

#endif // BUILD_ENABLE_VULKAN_RUNTIME_DEBUG