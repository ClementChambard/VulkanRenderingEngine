// Copyright dummy
#ifndef FIRSTAPP_INCLUDED_H
#define FIRSTAPP_INCLUDED_H

#include "./lve_window.hpp"
#include "./lve_pipeline.hpp"
#include "./lve_swap_chain.hpp"
#include "./lve_model.hpp"

#include <vector>
#include <memory>

namespace lve {

class FirstApp {
 public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    FirstApp();
    ~FirstApp();
    FirstApp(FirstApp const&) = delete;
    FirstApp& operator=(FirstApp const&) = delete;

    void run();

 private:
    void loadModels();
    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffers();
    void freeCommandBuffers();
    void drawFrame();
    void recreateSwapChain();
    void recordCommandBuffer(int imageIndex);

    LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
    LveDevice lveDevice{lveWindow};
    std::unique_ptr<LveSwapChain> lveSwapChain;
    std::unique_ptr<LvePipeline> lvePipeline;
    VkPipelineLayout pipelineLayout;
    std::vector<VkCommandBuffer> commandBuffers;
    std::unique_ptr<LveModel> lveModel;
};  // class FirstApp

}  // namespace lve

#endif  // FIRSTAPP_INCLUDED_H
