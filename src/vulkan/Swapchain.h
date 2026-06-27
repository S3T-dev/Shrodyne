#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>

struct QueueFamilyIndices;

/**
 * @file Swapchain.h
 * @brief Wrapper for Vulkan swapchain creation and image views.
 */
class Swapchain {
public:
    Swapchain();
    ~Swapchain();

    // non-copyable
    Swapchain(const Swapchain&) = delete;
    Swapchain& operator=(const Swapchain&) = delete;

    bool initialize(VkPhysicalDevice physicalDevice, VkDevice device, const QueueFamilyIndices& queueFamilyIndices, VkSurfaceKHR surface, GLFWwindow* window);
    void shutdown(VkDevice device);

    VkSwapchainKHR get() const { return swapchain_; }
    VkFormat getImageFormat() const { return surfaceFormat_; }
    VkExtent2D getExtent() const { return extent_; }
    const std::vector<VkImageView>& getImageViews() const { return imageViews_; }

private:
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities = {};
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    bool querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface, SwapChainSupportDetails& details) const;
    VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
    VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const;
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window) const;
    bool createImageViews(VkDevice device);

    VkSwapchainKHR swapchain_ = VK_NULL_HANDLE;
    VkFormat surfaceFormat_ = VK_FORMAT_UNDEFINED;
    VkExtent2D extent_ = {};
    std::vector<VkImage> images_;
    std::vector<VkImageView> imageViews_;
};
