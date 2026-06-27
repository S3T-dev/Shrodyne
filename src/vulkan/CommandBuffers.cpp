#include "CommandBuffers.h"
#include <volk.h>
#include <iostream>

CommandBuffers::CommandBuffers() = default;

CommandBuffers::~CommandBuffers() {
    // shutdown must be called explicitly before destruction
}

bool CommandBuffers::initialize(VkDevice device, VkCommandPool commandPool, const std::vector<VkFramebuffer>& framebuffers, VkRenderPass renderPass, VkExtent2D extent, VkPipeline pipeline, VkPipelineLayout pipelineLayout, VkBuffer vertexBuffer, uint32_t vertexCount, const glm::mat4& transform) {
    if (framebuffers.empty()) {
        std::cerr << "No framebuffers provided for command buffer allocation." << std::endl;
        return false;
    }

    commandBuffers_.resize(framebuffers.size());

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers_.size());

    if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers_.data()) != VK_SUCCESS) {
        std::cerr << "Failed to allocate command buffers." << std::endl;
        return false;
    }

    for (size_t i = 0; i < commandBuffers_.size(); ++i) {
        if (!recordCommandBuffer(commandBuffers_[i], renderPass, framebuffers[i], extent, pipeline, pipelineLayout, vertexBuffer, vertexCount, transform)) {
            return false;
        }
    }

    return true;
}

void CommandBuffers::shutdown(VkDevice device, VkCommandPool commandPool) {
    if (!commandBuffers_.empty()) {
        vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers_.size()), commandBuffers_.data());
        commandBuffers_.clear();
    }
}

bool CommandBuffers::recordCommandBuffer(VkCommandBuffer commandBuffer, VkRenderPass renderPass, VkFramebuffer framebuffer, VkExtent2D extent, VkPipeline pipeline, VkPipelineLayout pipelineLayout, VkBuffer vertexBuffer, uint32_t vertexCount, const glm::mat4& transform) {
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        std::cerr << "Failed to begin recording command buffer." << std::endl;
        return false;
    }

    VkClearValue clearValues[2] = {};
    clearValues[0].color = { {0.08f, 0.09f, 0.12f, 1.0f} };
    clearValues[1].depthStencil = { 1.0f, 0 };

    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = framebuffer;
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = extent;
    renderPassInfo.clearValueCount = 2;
    renderPassInfo.pClearValues = clearValues;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &transform);

    VkBuffer vertexBuffers[] = { vertexBuffer };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        std::cerr << "Failed to record command buffer." << std::endl;
        return false;
    }

    return true;
}
