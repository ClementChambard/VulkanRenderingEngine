// Copyright dummy
#ifndef LVEMODEL_INCLUDED_H
#define LVEMODEL_INCLUDED_H

#include "./lve_device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <vector>

namespace lve {

class LveModel {
 public:
  struct Vertex {
    glm::vec2 position;
    glm::vec3 color;

    static std::vector<VkVertexInputBindingDescription>
    getBindingDescriptions();
    static std::vector<VkVertexInputAttributeDescription>
    getAttributeDescriptions();
  };

  LveModel(LveDevice& device, std::vector<Vertex> const& vertices);
  ~LveModel();
  LveModel(LveModel&) = delete;
  LveModel& operator=(LveModel&) = delete;

  void bind(VkCommandBuffer commandBuffer);
  void draw(VkCommandBuffer commandBuffer);

 private:
  void createVertexBuffers(std::vector<Vertex> const& vertices);

  LveDevice& lveDevice;
  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  uint32_t vertexCount;
};  // class LveModel

}  // namespace lve

#endif  // LVEMODEL_INCLUDED_H
