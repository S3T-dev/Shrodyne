#include "PhysicalDevice.h"
#include <volk.h>
#include <algorithm>
#include <iostream>
#include <vector>

static bool hasSwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
    if (formatCount == 0) {
        return false;
    }

    uint32_t presentModeCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
    return presentModeCount != 0;
}

PhysicalDevice::PhysicalDevice() = default;

bool PhysicalDevice::pick(VkInstance instance, VkSurfaceKHR surface) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        std::cerr << "No Vulkan physical devices found." << std::endl;
        return false;
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    VkPhysicalDevice bestDevice = VK_NULL_HANDLE;
    int bestScore = 0;
    QueueFamilyIndices bestIndices;

    for (const auto& device : devices) {
        if (!isDeviceSuitable(device, surface)) {
            continue;
        }

        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(device, &properties);

        int score = 0;
        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            score += 1000;
        }
        score += static_cast<int>(properties.limits.maxImageDimension2D);

        QueueFamilyIndices indices = findQueueFamilies(device, surface);
        if (!indices.isComplete()) {
            continue;
        }

        if (score > bestScore) {
            bestScore = score;
            bestDevice = device;
            bestIndices = indices;
        }
    }

    if (bestDevice == VK_NULL_HANDLE) {
        std::cerr << "Failed to find a suitable Vulkan physical device." << std::endl;
        return false;
    }

    physicalDevice_ = bestDevice;
    queueFamilyIndices_ = bestIndices;
    return true;
}

bool PhysicalDevice::isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) const {
    QueueFamilyIndices indices = findQueueFamilies(device, surface);
    if (!indices.isComplete()) {
        return false;
    }

    if (!checkDeviceExtensionSupport(device)) {
        return false;
    }

    if (!checkSwapChainSupport(device, surface)) {
        return false;
    }

    return true;
}

QueueFamilyIndices PhysicalDevice::findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) const {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        if (presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        ++i;
    }

    return indices;
}

bool PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device) const {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::vector<std::string> required(requiredExtensions_.begin(), requiredExtensions_.end());
    for (const auto& extension : availableExtensions) {
        required.erase(std::remove(required.begin(), required.end(), extension.extensionName), required.end());
    }

    return required.empty();
}

bool PhysicalDevice::checkSwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) const {
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
    if (formatCount == 0) {
        return false;
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
    return presentModeCount != 0;
}
