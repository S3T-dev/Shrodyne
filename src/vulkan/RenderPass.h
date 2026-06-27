#pragma once

#include <vulkan/vulkan.h>

/**
 * @file RenderPass.h
 * @brief Wrapper for a Vulkan render pass.
 */
class RenderPass {
public:
    RenderPass();
    ~RenderPass();

    // non-copyable
    RenderPass(const RenderPass&) = delete;
    RenderPass& operator=(const RenderPass&) = delete;

    bool initialize(VkDevice device, VkFormat swapchainImageFormat, VkFormat depthImageFormat);
    void shutdown(VkDevice device);

    VkRenderPass get() const { return renderPass_; }

private:
    VkRenderPass renderPass_ = VK_NULL_HANDLE;
};
