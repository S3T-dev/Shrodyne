#pragma once

#include <vulkan/vulkan.h>
#include "PhysicalDevice.h"

/**
 * @file Device.h
 * @brief Wrapper for Vulkan logical device and queues.
 */
class Device {
public:
    Device();
    ~Device();

    // non-copyable
    Device(const Device&) = delete;
    Device& operator=(const Device&) = delete;

    bool initialize(const PhysicalDevice& physicalDevice, VkSurfaceKHR surface);
    void shutdown();

    VkDevice get() const { return device_; }
    VkQueue graphicsQueue() const { return graphicsQueue_; }
    VkQueue presentQueue() const { return presentQueue_; }

private:
    VkDevice device_ = VK_NULL_HANDLE;
    VkQueue graphicsQueue_ = VK_NULL_HANDLE;
    VkQueue presentQueue_ = VK_NULL_HANDLE;
    QueueFamilyIndices queueFamilyIndices_;
};
