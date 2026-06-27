#include "Surface.h"
#include <iostream>
#include <volk.h>

Surface::Surface() {}

Surface::~Surface() {
    // must call destroy before destructor if instance isn't available
}

bool Surface::create(VkInstance instance, int width, int height, const char* title) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window_) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        return false;
    }

    if (glfwCreateWindowSurface(instance, window_, nullptr, &surface_) != VK_SUCCESS) {
        std::cerr << "Failed to create Vulkan surface" << std::endl;
        return false;
    }

    return true;
}

void Surface::destroy(VkInstance instance) {
    if (surface_ != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(instance, surface_, nullptr);
        surface_ = VK_NULL_HANDLE;
    }
    if (window_) {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }
}
