#include "Framebuffer.h"
#include <volk.h>
#include <iostream>

Framebuffer::Framebuffer() = default;

Framebuffer::~Framebuffer() {
    // shutdown must be called explicitly before destruction
}

bool Framebuffer::initialize(VkDevice device, VkRenderPass renderPass, const std::vector<VkImageView>& imageViews, VkImageView depthImageView, VkExtent2D extent) {
    framebuffers_.resize(imageViews.size());

    for (size_t i = 0; i < imageViews.size(); ++i) {
        std::vector<VkImageView> attachments = { imageViews[i], depthImageView };

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = extent.width;
        framebufferInfo.height = extent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &framebuffers_[i]) != VK_SUCCESS) {
            std::cerr << "Failed to create framebuffer." << std::endl;
            return false;
        }
    }

    return true;
}

void Framebuffer::shutdown(VkDevice device) {
    for (auto framebuffer : framebuffers_) {
        if (framebuffer != VK_NULL_HANDLE) {
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }
    }
    framebuffers_.clear();
}
