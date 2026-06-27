#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

/**
 * @file Allocator.cpp
 * @brief Minimal VMA allocator implementation unit.
 *
 * This translation unit defines VMA implementation (VMA_IMPLEMENTATION)
 * so that the VMA symbols are compiled into the final binary.
 */

#include "Allocator.h"
#include <stdexcept>

Allocator::Allocator() = default;

Allocator::~Allocator() {
    shutdown();
}

bool Allocator::initialize(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device) {
    if (initialized_) return true;

    VmaAllocatorCreateInfo createInfo = {};
    createInfo.physicalDevice = physicalDevice;
    createInfo.device = device;
    createInfo.instance = instance;

    if (vmaCreateAllocator(&createInfo, &allocator_) != VK_SUCCESS) {
        return false;
    }

    initialized_ = true;
    return true;
}

void Allocator::shutdown() {
    if (!initialized_) return;
    vmaDestroyAllocator(allocator_);
    allocator_ = VK_NULL_HANDLE;
    initialized_ = false;
}
