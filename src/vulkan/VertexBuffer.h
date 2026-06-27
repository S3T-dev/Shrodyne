#pragma once

#include <vulkan/vulkan.h>
#include <vector>

struct Vertex {
    float position[3];
    float color[3];
    float normal[3];
};

class VertexBuffer {
public:
    VertexBuffer();
    ~VertexBuffer();

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    bool initialize(VkPhysicalDevice physicalDevice, VkDevice device, const std::vector<Vertex>& vertices);
    void shutdown(VkDevice device);

    VkBuffer get() const { return buffer_; }

private:
    VkBuffer buffer_ = VK_NULL_HANDLE;
    VkDeviceMemory memory_ = VK_NULL_HANDLE;
};
