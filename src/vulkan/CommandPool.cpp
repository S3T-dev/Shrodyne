#include "CommandPool.h"
#include <volk.h>
#include <iostream>

CommandPool::CommandPool() = default;

CommandPool::~CommandPool() {
    // shutdown must be called explicitly before destruction
}

bool CommandPool::initialize(VkDevice device, const QueueFamilyIndices& queueFamilyIndices) {
    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = static_cast<uint32_t>(queueFamilyIndices.graphicsFamily);
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool_) != VK_SUCCESS) {
        std::cerr << "Failed to create command pool." << std::endl;
        return false;
    }

    return true;
}

void CommandPool::shutdown(VkDevice device) {
    if (commandPool_ != VK_NULL_HANDLE) {
        vkDestroyCommandPool(device, commandPool_, nullptr);
        commandPool_ = VK_NULL_HANDLE;
    }
}
