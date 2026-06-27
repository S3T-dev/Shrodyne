#pragma once

#include <vulkan/vulkan.h>
#include <vector>

/**
 * @file PhysicalDevice.h
 * @brief Selects a suitable Vulkan physical device and queue families.
 */

struct QueueFamilyIndices {
    int graphicsFamily = -1;
    int presentFamily = -1;

    bool isComplete() const {
        return graphicsFamily >= 0 && presentFamily >= 0;
    }
};

class PhysicalDevice {
public:
    PhysicalDevice();

    /**
     * Pick a suitable physical device for the given instance and surface.
     * @return true if a suitable device was found.
     */
    bool pick(VkInstance instance, VkSurfaceKHR surface);

    VkPhysicalDevice get() const { return physicalDevice_; }
    const QueueFamilyIndices& getQueueFamilyIndices() const { return queueFamilyIndices_; }

private:
    bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) const;
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) const;
    bool checkDeviceExtensionSupport(VkPhysicalDevice device) const;
    bool checkSwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) const;

    VkPhysicalDevice physicalDevice_ = VK_NULL_HANDLE;
    QueueFamilyIndices queueFamilyIndices_;
    const std::vector<const char*> requiredExtensions_ = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
};
