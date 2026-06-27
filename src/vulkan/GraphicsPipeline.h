#pragma once

#include <vulkan/vulkan.h>
#include <string>

/**
 * @file GraphicsPipeline.h
 * @brief Wrapper for Vulkan graphics pipeline creation.
 */
class GraphicsPipeline {
public:
    GraphicsPipeline();
    ~GraphicsPipeline();

    // non-copyable
    GraphicsPipeline(const GraphicsPipeline&) = delete;
    GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;

    bool initialize(VkDevice device, VkRenderPass renderPass, VkExtent2D extent);
    void shutdown(VkDevice device);

    VkPipeline get() const { return pipeline_; }
    VkPipelineLayout getLayout() const { return pipelineLayout_; }

private:
    bool createShaderModule(VkDevice device, const std::string& path, VkShaderModule& shaderModule) const;

    VkPipeline pipeline_ = VK_NULL_HANDLE;
    VkPipelineLayout pipelineLayout_ = VK_NULL_HANDLE;
};