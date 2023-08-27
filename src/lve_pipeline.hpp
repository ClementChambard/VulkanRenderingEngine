// Copyright dummy
#ifndef LVEPIPELINE_INCLUDED_H
#define LVEPIPELINE_INCLUDED_H

#include "./lve_device.hpp"

#include <string>
#include <vector>

namespace lve {

struct PipelineConfigInfo {
  PipelineConfigInfo(PipelineConfigInfo&) = delete;
  PipelineConfigInfo& operator=(PipelineConfigInfo&) = delete;

  VkPipelineViewportStateCreateInfo viewportInfo;
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
  VkPipelineRasterizationStateCreateInfo rasterizationInfo;
  VkPipelineMultisampleStateCreateInfo multisampleInfo;
  VkPipelineColorBlendAttachmentState colorBlendAttachement;
  VkPipelineColorBlendStateCreateInfo colorBlendInfo;
  VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
  std::vector<VkDynamicState> dynamicStateEnable;
  VkPipelineDynamicStateCreateInfo dynamicStateInfo;
  VkPipelineLayout pipelineLayout = nullptr;
  VkRenderPass renderPass = nullptr;
  uint32_t subpass = 0;
};

class LvePipeline {
public:
  LvePipeline(LveDevice& device,
              std::string const& vertFilePath,
              std::string const& fragFilePath,
              PipelineConfigInfo const& configInfo);
  ~LvePipeline();
  LvePipeline(LvePipeline&) = delete;
  LvePipeline& operator=(LvePipeline&) = delete;

  void bind(VkCommandBuffer commandBuffer);

  static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

private:
  static std::vector<char> readFile(std::string const& filepath);

  void createGraphicsPipeline(std::string const& vertFilePath,
                              std::string const& fragFilePath,
                              PipelineConfigInfo const& configInfo);

  void createShaderModule(std::vector<char> const& code,
                          VkShaderModule* shaderModule);

  LveDevice& lveDevice;
  VkPipeline graphicsPipeline;
  VkShaderModule vertShaderModule;
  VkShaderModule fragShaderModule;
};  // class LvePipeline

}  // namespace lve

#endif  // LVEPIPELINE_INCLUDED_H
