#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <string>

/**
 * @file Instance.h
 * @brief Wrapper for VkInstance creation and debug utilities.
 */
class Instance {
public:
    Instance();
    ~Instance();

    /** Initialize the Vulkan instance.
     * @param appName Application name shown to the driver.
     * @param enableValidation Enable validation layers and debug utils if available.
     */
    bool initialize(const char* appName = "Shrodyne", bool enableValidation = true);

    /** Destroy the instance and cleanup debug messenger. */
    void shutdown();

    VkInstance get() const { return instance_; }
    bool validationEnabled() const { return enableValidation_; }

private:
    bool checkValidationLayerSupport();
    std::vector<const char*> getRequiredExtensions();

    VkInstance instance_ = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT debugMessenger_ = VK_NULL_HANDLE;
    bool enableValidation_ = false;
    bool glfwInitialized_ = false;
    std::vector<const char*> enabledLayers_;
};
