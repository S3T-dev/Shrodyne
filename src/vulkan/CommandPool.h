#pragma once

#include <vulkan/vulkan.h>
#include "PhysicalDevice.h"

/**
 * @file CommandPool.h
 * @brief Wrapper for a Vulkan command pool.
 */
class CommandPool {
public:
    CommandPool();
    ~CommandPool();

    // non-copyable
    CommandPool(const CommandPool&) = delete;
    CommandPool& operator=(const CommandPool&) = delete;

    bool initialize(VkDevice device, const QueueFamilyIndices& queueFamilyIndices);
    void shutdown(VkDevice device);

    VkCommandPool get() const { return commandPool_; }

private:
    VkCommandPool commandPool_ = VK_NULL_HANDLE;
};