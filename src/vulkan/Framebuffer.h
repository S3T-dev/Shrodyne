#pragma once

#include <vulkan/vulkan.h>
#include <vector>

/**
 * @file Framebuffer.h
 * @brief Wrapper for swapchain framebuffers.
 */
class Framebuffer {
public:
    Framebuffer();
    ~Framebuffer();

    // non-copyable
    Framebuffer(const Framebuffer&) = delete;
    Framebuffer& operator=(const Framebuffer&) = delete;

    bool initialize(VkDevice device, VkRenderPass renderPass, const std::vector<VkImageView>& imageViews, VkImageView depthImageView, VkExtent2D extent);
    void shutdown(VkDevice device);

    const std::vector<VkFramebuffer>& getFramebuffers() const { return framebuffers_; }

private:
    std::vector<VkFramebuffer> framebuffers_;
};