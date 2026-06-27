/**
 * @file Allocator.h
 * @brief Thin wrapper around the Vulkan Memory Allocator (VMA).
 *
 * This small RAII-style helper constructs and destroys the VMA allocator
 * which simplifies buffer/image allocation across the engine.
 */
#pragma once

#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"

/**
 * @brief VMA allocator wrapper.
 *
 * Construct with default ctor, then call `initialize` after creating the
 * Vulkan instance, physical device and logical device. Call `shutdown` or
 * rely on the destructor for cleanup.
 */
class Allocator {
public:
    /** Default constructor. */
    Allocator();

    /** Destructor - destroys the VMA allocator if initialized. */
    ~Allocator();

    // non-copyable
    Allocator(const Allocator&) = delete;
    Allocator& operator=(const Allocator&) = delete;

    /**
     * @brief Initialize the allocator.
     * @param instance Vulkan instance.
     * @param physicalDevice Selected physical device.
     * @param device Logical device handle.
     * @return true on success.
     */
    bool initialize(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device);

    /**
     * @brief Shutdown and free the allocator.
     */
    void shutdown();

    /**
     * @brief Returns the raw VMA allocator handle.
     */
    VmaAllocator get() const { return allocator_; }

private:
    VmaAllocator allocator_ = VK_NULL_HANDLE;
    bool initialized_ = false;
};
