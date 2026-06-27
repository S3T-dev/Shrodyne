#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

/**
 * @file Surface.h
 * @brief Wrapper for a GLFW window and Vulkan surface.
 */
class Surface {
public:
    Surface();
    ~Surface();

    // non-copyable
    Surface(const Surface&) = delete;
    Surface& operator=(const Surface&) = delete;

    /** Create a window and Vulkan surface. Returns true on success. */
    bool create(VkInstance instance, int width = 1280, int height = 720, const char* title = "Shrodyne");

    /** Destroy window and surface. */
    void destroy(VkInstance instance);

    VkSurfaceKHR getSurface() const { return surface_; }
    GLFWwindow* getWindow() const { return window_; }

private:
    GLFWwindow* window_ = nullptr;
    VkSurfaceKHR surface_ = VK_NULL_HANDLE;
};
