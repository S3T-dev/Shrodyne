#include "Device.h"
#include <set>
#include <vector>
#include <iostream>
#include <volk.h>

Device::Device() = default;

Device::~Device() {
    shutdown();
}

bool Device::initialize(const PhysicalDevice& physicalDevice, VkSurfaceKHR surface) {
    queueFamilyIndices_ = physicalDevice.getQueueFamilyIndices();

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<int> uniqueQueueFamilies = {
        queueFamilyIndices_.graphicsFamily,
        queueFamilyIndices_.presentFamily
    };

    float queuePriority = 1.0f;
    for (int queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = static_cast<uint32_t>(queueFamily);
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (vkCreateDevice(physicalDevice.get(), &createInfo, nullptr, &device_) != VK_SUCCESS) {
        std::cerr << "Failed to create logical device." << std::endl;
        return false;
    }

    volkLoadDevice(device_);

    vkGetDeviceQueue(device_, static_cast<uint32_t>(queueFamilyIndices_.graphicsFamily), 0, &graphicsQueue_);
    vkGetDeviceQueue(device_, static_cast<uint32_t>(queueFamilyIndices_.presentFamily), 0, &presentQueue_);

    return true;
}

void Device::shutdown() {
    if (device_ != VK_NULL_HANDLE) {
        vkDestroyDevice(device_, nullptr);
        device_ = VK_NULL_HANDLE;
    }
}
