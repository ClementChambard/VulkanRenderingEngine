// Copyright dummy
#include "./lve_model.hpp"
#include <cstring>
#include <vulkan/vulkan_core.h>

namespace lve {

LveModel::LveModel(LveDevice& device, std::vector<Vertex> const& vertices) :
    lveDevice(device) {
    createVertexBuffers(vertices);
}

LveModel::~LveModel() {
  vkDestroyBuffer(lveDevice.device(), vertexBuffer, nullptr);
  vkFreeMemory(lveDevice.device(), vertexBufferMemory, nullptr);
}

void LveModel::createVertexBuffers(std::vector<Vertex> const& vertices) {
  vertexCount = static_cast<uint32_t>(vertices.size());
  assert(vertexCount >= 3 && "Vertex count must be min 3");
  VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
  lveDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         vertexBuffer, vertexBufferMemory);
  void* data;
  vkMapMemory(lveDevice.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
  vkUnmapMemory(lveDevice.device(), vertexBufferMemory);
}

void LveModel::bind(VkCommandBuffer commandBuffer) {
  VkBuffer buffers[] = {vertexBuffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}

void LveModel::draw(VkCommandBuffer commandBuffer) {
  vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
}

std::vector<VkVertexInputBindingDescription>
LveModel::Vertex::getBindingDescriptions() {
  return {{
    .binding = 0,
    .stride = sizeof(Vertex),
    .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
  }};
}

std::vector<VkVertexInputAttributeDescription>
LveModel::Vertex::getAttributeDescriptions() {
  return {{
    .location = 0,
    .binding = 0,
    .format = VK_FORMAT_R32G32B32_SFLOAT,
    .offset = offsetof(Vertex, position)
  }, {
    .location = 1,
    .binding = 0,
    .format = VK_FORMAT_R32G32B32_SFLOAT,
    .offset = offsetof(Vertex, color)
  }};
}

}  // namespace lve
