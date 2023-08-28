// Copyright dummy
#ifndef SIMPLERENDERSYSTEM_INCLUDED_H
#define SIMPLERENDERSYSTEM_INCLUDED_H

#include "./lve_pipeline.hpp"
#include "./lve_camera.hpp"
#include "./lve_game_object.hpp"

#include <vector>
#include <memory>

namespace lve {

class SimpleRenderSystem {
 public:
    SimpleRenderSystem(LveDevice& device, VkRenderPass renderPass);
    ~SimpleRenderSystem();
    SimpleRenderSystem(SimpleRenderSystem const&) = delete;
    SimpleRenderSystem& operator=(SimpleRenderSystem const&) = delete;

    void renderGameObjects(VkCommandBuffer commandBuffer,
                           std::vector<LveGameObject>& gameObjects,
                           LveCamera const& camera);

 private:
    void createPipelineLayout();
    void createPipeline(VkRenderPass renderPass);

    LveDevice& lveDevice;
    std::unique_ptr<LvePipeline> lvePipeline;
    VkPipelineLayout pipelineLayout;
};  // class SimpleRenderSystem

}  // namespace lve

#endif  // SIMPLERENDERSYSTEM_INCLUDED_H
