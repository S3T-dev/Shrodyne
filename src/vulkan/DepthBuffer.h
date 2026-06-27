#pragma once

#include <vulkan/vulkan.h>

/**
 * @file DepthBuffer.h
 * @brief Wrapper for a depth attachment image and view.
 */
class DepthBuffer {
public:
    DepthBuffer();
    ~DepthBuffer();

    DepthBuffer(const DepthBuffer&) = delete;
    DepthBuffer& operator=(const DepthBuffer&) = delete;

    bool initialize(VkPhysicalDevice physicalDevice, VkDevice device, VkExtent2D extent);
    void shutdown(VkDevice device);

    VkImageView getView() const { return view_; }
    VkFormat getFormat() const { return format_; }

private:
    VkImage image_ = VK_NULL_HANDLE;
    VkDeviceMemory memory_ = VK_NULL_HANDLE;
    VkImageView view_ = VK_NULL_HANDLE;
    VkFormat format_ = VK_FORMAT_D32_SFLOAT;
};
