#include "Shrodyne.h"

#include "../vulkan/Instance.h"
#include "../vulkan/Surface.h"
#include "../vulkan/PhysicalDevice.h"
#include "../vulkan/Device.h"
#include "../vulkan/Swapchain.h"
#include "../vulkan/RenderPass.h"
#include "../vulkan/Framebuffer.h"
#include "../vulkan/CommandPool.h"
#include "../vulkan/CommandBuffers.h"
#include "../vulkan/GraphicsPipeline.h"
#include "../vulkan/VertexBuffer.h"
#include "../vulkan/DepthBuffer.h"
#include <volk.h>
#include <GLFW/glfw3.h>
#include <thread>
#include <chrono>
#include <atomic>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace {
struct CameraState {
    float yaw = 0.35f;
    float pitch = 0.22f;
    float distance = 3.1f;
    bool dragging = false;
    double lastCursorX = 0.0;
    double lastCursorY = 0.0;
    double scrollAccumulated = 0.0;
};

void cursorPosCallback(GLFWwindow* window, double xPos, double yPos) {
    auto* state = static_cast<CameraState*>(glfwGetWindowUserPointer(window));
    if (!state) {
        return;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (!state->dragging) {
            state->dragging = true;
            state->lastCursorX = xPos;
            state->lastCursorY = yPos;
        } else {
            state->yaw += static_cast<float>(xPos - state->lastCursorX) * 0.005f;
            state->pitch += static_cast<float>(yPos - state->lastCursorY) * 0.005f;
            state->pitch = glm::clamp(state->pitch, -1.2f, 1.2f);
            state->lastCursorX = xPos;
            state->lastCursorY = yPos;
        }
    } else {
        state->dragging = false;
    }
}

void scrollCallback(GLFWwindow* window, double, double yOffset) {
    auto* state = static_cast<CameraState*>(glfwGetWindowUserPointer(window));
    if (!state) {
        return;
    }
    state->scrollAccumulated += yOffset;
}
} // namespace

Shrodyne::Shrodyne() {
    // lightweight ctor
}

void Shrodyne::initialize() {
    // placeholder for future initialization
}

void Shrodyne::update() {
    // placeholder for simulation update
}

void Shrodyne::render() {
    // placeholder for render
}

void Shrodyne::run() {
    Instance instance;
    if (!instance.initialize("Shrodyne", true)) {
        std::cerr << "Failed to initialize Vulkan instance." << std::endl;
        return;
    }

    Surface surface;
    if (!surface.create(instance.get(), 1280, 720, "Shrodyne")) {
        std::cerr << "Failed to create surface/window." << std::endl;
        instance.shutdown();
        return;
    }

    PhysicalDevice physicalDevice;
    if (!physicalDevice.pick(instance.get(), surface.getSurface())) {
        surface.destroy(instance.get());
        instance.shutdown();
        return;
    }

    Device device;
    if (!device.initialize(physicalDevice, surface.getSurface())) {
        std::cerr << "Failed to create logical device." << std::endl;
        surface.destroy(instance.get());
        instance.shutdown();
        return;
    }

    Swapchain swapchain;
    if (!swapchain.initialize(physicalDevice.get(), device.get(), physicalDevice.getQueueFamilyIndices(), surface.getSurface(), surface.getWindow())) {
        std::cerr << "Failed to create swapchain." << std::endl;
        device.shutdown();
        surface.destroy(instance.get());
        instance.shutdown();
        return;
    }

    DepthBuffer depthBuffer;
    if (!depthBuffer.initialize(physicalDevice.get(), device.get(), swapchain.getExtent())) {
        std::cerr << "Failed to create depth buffer." << std::endl;
        swapchain.shutdown(device.get());
        device.shutdown();
        surface.destroy(instance.get());
        instance.shutdown();
        return;
    }

    RenderPass renderPass;
    if (!renderPass.initialize(device.get(), swapchain.getImageFormat(), depthBuffer.getFormat())) {
        std::cerr << "Failed to create render pass." << std::endl;
        depthBuffer.shutdown(device.get());
        swapchain.shutdown(device.get());
        device.shutdown();
        surface.destroy(instance.get());
        instance.shutdown();
        return;
    }

    Framebuffer framebuffers;
    if (!framebuffers.initialize(device.get(), renderPass.get(), swapchain.getImageViews(), depthBuffer.getView(), swapchain.getExtent())) {
        std::cerr << "Failed to create framebuffers." << std::endl;
        depthBuffer.shutdown(device.get());
        renderPass.shutdown(device.get());
        swapchain.shutdown(device.get());
        device.shutdown();
        surface.destroy(instance.get());
        instance.shutdown();
        return;
    }

    CommandPool commandPool;
    if (!commandPool.initialize(device.get(), physicalDevice.getQueueFamilyIndices())) {
        std::cerr << "Failed to create command pool." << std::endl;
        framebuffers.shutdown(device.get());
        renderPass.shutdown(device.get());
        depthBuffer.shutdown(device.get());
        swapchain.shutdown(device.get());
        device.shutdown();
        surface.destroy(instance.get());
        instance.shutdown();
        return;
    }

    GraphicsPipeline graphicsPipeline;
    if (!graphicsPipeline.initialize(device.get(), renderPass.get(), swapchain.getExtent())) {
        std::cerr << "Failed to create graphics pipeline." << std::endl;
        commandPool.shutdown(device.get());
        framebuffers.shutdown(device.get());
        renderPass.shutdown(device.get());
        depthBuffer.shutdown(device.get());
        swapchain.shutdown(device.get());
        device.shutdown();
        surface.destroy(instance.get());
        instance.shutdown();
        return;
    }

    std::vector<Vertex> vertices = {
        {{0.0f, 1.0f, 0.0f}, {1.0f, 0.15f, 0.15f}, {0.0f, 1.0f, 0.0f}},
        {{-1.0f, -1.0f, 1.0f}, {1.0f, 0.15f, 0.15f}, {0.0f, 1.0f, 0.0f}},
        {{1.0f, -1.0f, 1.0f}, {1.0f, 0.15f, 0.15f}, {0.0f, 1.0f, 0.0f}},

        {{0.0f, 1.0f, 0.0f}, {0.15f, 1.0f, 0.2f}, {0.0f, 0.0f, -1.0f}},
        {{1.0f, -1.0f, 1.0f}, {0.15f, 1.0f, 0.2f}, {0.0f, 0.0f, -1.0f}},
        {{0.0f, -1.0f, -1.0f}, {0.15f, 1.0f, 0.2f}, {0.0f, 0.0f, -1.0f}},

        {{0.0f, 1.0f, 0.0f}, {0.15f, 0.2f, 1.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.0f, -1.0f, -1.0f}, {0.15f, 0.2f, 1.0f}, {1.0f, 0.0f, 0.0f}},
        {{-1.0f, -1.0f, 1.0f}, {0.15f, 0.2f, 1.0f}, {1.0f, 0.0f, 0.0f}},

        {{-1.0f, -1.0f, 1.0f}, {1.0f, 0.9f, 0.15f}, {0.0f, -1.0f, 0.0f}},
        {{0.0f, -1.0f, -1.0f}, {1.0f, 0.9f, 0.15f}, {0.0f, -1.0f, 0.0f}},
        {{1.0f, -1.0f, 1.0f}, {1.0f, 0.9f, 0.15f}, {0.0f, -1.0f, 0.0f}}
    };

    VertexBuffer vertexBuffer;
    if (!vertexBuffer.initialize(physicalDevice.get(), device.get(), vertices)) {
        std::cerr << "Failed to create vertex buffer." << std::endl;
        graphicsPipeline.shutdown(device.get());
        commandPool.shutdown(device.get());
        framebuffers.shutdown(device.get());
        renderPass.shutdown(device.get());
        depthBuffer.shutdown(device.get());
        swapchain.shutdown(device.get());
        device.shutdown();
        surface.destroy(instance.get());
        instance.shutdown();
        return;
    }

    GLFWwindow* window = surface.getWindow();

    uint32_t vertexCount = static_cast<uint32_t>(vertices.size());

    CommandBuffers commandBuffers;
    CameraState cameraState;
    glfwSetWindowUserPointer(window, &cameraState);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);

    auto refreshCommandBuffers = [&](int width, int height) -> bool {
        float aspect = (height > 0) ? static_cast<float>(width) / static_cast<float>(height) : 1.0f;
        if (aspect <= 0.0f) {
            aspect = 1.0f;
        }
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 10.0f);
        projection[1][1] *= -1.0f;

        cameraState.distance = glm::clamp(cameraState.distance + static_cast<float>(cameraState.scrollAccumulated * 0.05f), 1.5f, 5.5f);
        cameraState.scrollAccumulated = 0.0;

        glm::vec3 cameraPos(
            cameraState.distance * std::cos(cameraState.yaw) * std::cos(cameraState.pitch),
            cameraState.distance * std::sin(cameraState.pitch),
            cameraState.distance * std::sin(cameraState.yaw) * std::cos(cameraState.pitch));
        glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.15f, 0.0f));
        model = glm::scale(model, glm::vec3(0.65f));
        glm::mat4 transform = projection * view * model;

        commandBuffers.shutdown(device.get(), commandPool.get());
        return commandBuffers.initialize(device.get(), commandPool.get(), framebuffers.getFramebuffers(), renderPass.get(), swapchain.getExtent(), graphicsPipeline.get(), graphicsPipeline.getLayout(), vertexBuffer.get(), vertexCount, transform);
    };

    int initialWidth = 0;
    int initialHeight = 0;
    glfwGetFramebufferSize(window, &initialWidth, &initialHeight);
    if (!refreshCommandBuffers(initialWidth, initialHeight)) {
        std::cerr << "Failed to create command buffers." << std::endl;
        graphicsPipeline.shutdown(device.get());
        commandPool.shutdown(device.get());
        framebuffers.shutdown(device.get());
        renderPass.shutdown(device.get());
        depthBuffer.shutdown(device.get());
        swapchain.shutdown(device.get());
        device.shutdown();
        surface.destroy(instance.get());
        instance.shutdown();
        return;
    }

    VkSemaphore imageAvailableSemaphore = VK_NULL_HANDLE;
    VkSemaphore renderFinishedSemaphore = VK_NULL_HANDLE;
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (vkCreateSemaphore(device.get(), &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(device.get(), &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS) {
        std::cerr << "Failed to create synchronization semaphores." << std::endl;
        commandBuffers.shutdown(device.get(), commandPool.get());
        commandPool.shutdown(device.get());
        framebuffers.shutdown(device.get());
        renderPass.shutdown(device.get());
        swapchain.shutdown(device.get());
        device.shutdown();
        surface.destroy(instance.get());
        instance.shutdown();
        return;
    }

    std::atomic<bool> framebufferResized{false};
    static std::atomic<bool>* resizeFlag = nullptr;
    resizeFlag = &framebufferResized;

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int, int) {
        if (resizeFlag != nullptr) {
            resizeFlag->store(true);
        }
    });

    auto recreateSwapchainResources = [&](int width, int height) -> bool {
        vkDeviceWaitIdle(device.get());

        commandBuffers.shutdown(device.get(), commandPool.get());
        framebuffers.shutdown(device.get());
        graphicsPipeline.shutdown(device.get());
        depthBuffer.shutdown(device.get());
        swapchain.shutdown(device.get());

        if (!swapchain.initialize(physicalDevice.get(), device.get(), physicalDevice.getQueueFamilyIndices(), surface.getSurface(), window)) {
            std::cerr << "Failed to recreate swapchain." << std::endl;
            return false;
        }

        if (!graphicsPipeline.initialize(device.get(), renderPass.get(), swapchain.getExtent())) {
            std::cerr << "Failed to recreate graphics pipeline." << std::endl;
            return false;
        }

        if (!depthBuffer.initialize(physicalDevice.get(), device.get(), swapchain.getExtent())) {
            std::cerr << "Failed to recreate depth buffer." << std::endl;
            return false;
        }

        if (!framebuffers.initialize(device.get(), renderPass.get(), swapchain.getImageViews(), depthBuffer.getView(), swapchain.getExtent())) {
            std::cerr << "Failed to recreate framebuffers." << std::endl;
            return false;
        }

        if (!refreshCommandBuffers(width, height)) {
            std::cerr << "Failed to recreate command buffers." << std::endl;
            return false;
        }

        return true;
    };

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        if (width == 0 || height == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
            continue;
        }

        if (framebufferResized.exchange(false)) {
            if (!recreateSwapchainResources(width, height)) {
                std::cerr << "Failed to recreate Vulkan resources after resize." << std::endl;
                break;
            }
        }

        if (!refreshCommandBuffers(width, height)) {
            std::cerr << "Failed to refresh draw command buffers." << std::endl;
            break;
        }

        uint32_t imageIndex = 0;
        VkResult acquireResult = vkAcquireNextImageKHR(device.get(), swapchain.get(), UINT64_MAX,
            imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
        if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR || acquireResult == VK_SUBOPTIMAL_KHR) {
            framebufferResized.store(true);
            continue;
        }
        if (acquireResult != VK_SUCCESS) {
            std::cerr << "Failed to acquire swapchain image." << std::endl;
            break;
        }

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffers.get()[imageIndex];

        VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(device.graphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
            std::cerr << "Failed to submit draw command buffer." << std::endl;
            break;
        }

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        VkSwapchainKHR swapchains[] = { swapchain.get() };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapchains;
        presentInfo.pImageIndices = &imageIndex;

        VkResult presentResult = vkQueuePresentKHR(device.presentQueue(), &presentInfo);
        if (presentResult == VK_ERROR_OUT_OF_DATE_KHR || presentResult == VK_SUBOPTIMAL_KHR) {
            framebufferResized.store(true);
            continue;
        }
        if (presentResult != VK_SUCCESS) {
            std::cerr << "Failed to present swapchain image." << std::endl;
            break;
        }

        vkQueueWaitIdle(device.presentQueue());
    }

    vkDestroySemaphore(device.get(), renderFinishedSemaphore, nullptr);
    vkDestroySemaphore(device.get(), imageAvailableSemaphore, nullptr);
    commandBuffers.shutdown(device.get(), commandPool.get());
    vertexBuffer.shutdown(device.get());
    framebuffers.shutdown(device.get());
    depthBuffer.shutdown(device.get());
    graphicsPipeline.shutdown(device.get());
    commandPool.shutdown(device.get());
    renderPass.shutdown(device.get());
    swapchain.shutdown(device.get());
    device.shutdown();
    surface.destroy(instance.get());
    instance.shutdown();
}