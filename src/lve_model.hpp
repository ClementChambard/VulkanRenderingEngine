// Copyright dummy
#ifndef LVEMODEL_INCLUDED_H
#define LVEMODEL_INCLUDED_H

#include "./lve_device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace lve {

class LveModel {
 public:
  struct Vertex {
    glm::vec3 position{};
    glm::vec3 color{};
    glm::vec3 normal{};
    glm::vec2 uv{};

    static std::vector<VkVertexInputBindingDescription>
    getBindingDescriptions();
    static std::vector<VkVertexInputAttributeDescription>
    getAttributeDescriptions();

    bool operator==(Vertex const& other) const {
        return position == other.position &&
               color == other.color &&
               normal == other.normal &&
               uv == other.uv;
    }
  };

  struct Builder {
    std::vector<Vertex> vertices{};
    std::vector<uint32_t> indices{};

    void loadModel(std::string const& filepath);
  };

  LveModel(LveDevice& device, Builder const& builder);
  ~LveModel();
  LveModel(LveModel&) = delete;
  LveModel& operator=(LveModel&) = delete;

  void bind(VkCommandBuffer commandBuffer);
  void draw(VkCommandBuffer commandBuffer);

  static std::unique_ptr<LveModel> createModelFromFile(LveDevice &device, std::string const& filepath);

 private:
  void createVertexBuffers(std::vector<Vertex> const& vertices);
  void createIndexBuffers(std::vector<uint32_t> const& indices);

  LveDevice& lveDevice;
  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  uint32_t vertexCount;

  bool hasIndexBuffer = false;
  VkBuffer indexBuffer;
  VkDeviceMemory indexBufferMemory;
  uint32_t indexCount;
};  // class LveModel

}  // namespace lve

#endif  // LVEMODEL_INCLUDED_H
