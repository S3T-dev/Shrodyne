#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <vector>

/**
 * @file CommandBuffers.h
 * @brief Wrapper for Vulkan command buffer allocation and recording.
 */
class CommandBuffers {
public:
    CommandBuffers();
    ~CommandBuffers();

    // non-copyable
    CommandBuffers(const CommandBuffers&) = delete;
    CommandBuffers& operator=(const CommandBuffers&) = delete;

    bool initialize(VkDevice device, VkCommandPool commandPool, const std::vector<VkFramebuffer>& framebuffers, VkRenderPass renderPass, VkExtent2D extent, VkPipeline pipeline, VkPipelineLayout pipelineLayout, VkBuffer vertexBuffer, uint32_t vertexCount, const glm::mat4& transform);
    void shutdown(VkDevice device, VkCommandPool commandPool);

    const std::vector<VkCommandBuffer>& get() const { return commandBuffers_; }

private:
    bool recordCommandBuffer(VkCommandBuffer commandBuffer, VkRenderPass renderPass, VkFramebuffer framebuffer, VkExtent2D extent, VkPipeline pipeline, VkPipelineLayout pipelineLayout, VkBuffer vertexBuffer, uint32_t vertexCount, const glm::mat4& transform);

    std::vector<VkCommandBuffer> commandBuffers_;
};