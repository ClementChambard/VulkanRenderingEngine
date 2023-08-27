// Copyright dummy
#include "./lve_pipeline.hpp"
#include "./lve_model.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cassert>

namespace lve {

LvePipeline::LvePipeline(LveDevice& device,
            std::string const& vertFilePath,
            std::string const& fragFilePath,
            PipelineConfigInfo const& configInfo) : lveDevice(device) {
    createGraphicsPipeline(vertFilePath, fragFilePath, configInfo);
}

LvePipeline::~LvePipeline() {
    vkDestroyShaderModule(lveDevice.device(), vertShaderModule, nullptr);
    vkDestroyShaderModule(lveDevice.device(), fragShaderModule, nullptr);
    vkDestroyPipeline(lveDevice.device(), graphicsPipeline, nullptr);
}

std::vector<char> LvePipeline::readFile(std::string const& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file: " + filename);
    }

    size_t fileSize = static_cast<size_t>(file.tellg());

    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();
    return buffer;
}

void LvePipeline::createShaderModule(std::vector<char> const& code,
                                     VkShaderModule* shaderModule) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    if (vkCreateShaderModule(lveDevice.device(), &createInfo, nullptr,
                             shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module");
    }
}

void LvePipeline::createGraphicsPipeline(std::string const& vertFilePath,
                                         std::string const& fragFilePath,
                                         PipelineConfigInfo const& configInfo) {
    assert(configInfo.pipelineLayout != VK_NULL_HANDLE &&
           "Cannot create graphics pipeline: "
           "no pipelineLayout provided in config");
    assert(configInfo.renderPass != VK_NULL_HANDLE &&
           "Cannot create graphics pipeline: "
           "no renderPass provided in config");

    auto vertCode = readFile(vertFilePath);
    auto fragCode = readFile(fragFilePath);

    createShaderModule(vertCode, &vertShaderModule);
    createShaderModule(fragCode, &fragShaderModule);

    VkPipelineShaderStageCreateInfo shaderStages[2]{};

    shaderStages[0].sType =
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].module = vertShaderModule;
    shaderStages[0].pName = "main";
    shaderStages[0].flags = 0;
    shaderStages[0].pNext = nullptr;
    shaderStages[0].pSpecializationInfo = nullptr;

    shaderStages[1].sType =
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].module = fragShaderModule;
    shaderStages[1].pName = "main";
    shaderStages[1].flags = 0;
    shaderStages[1].pNext = nullptr;
    shaderStages[1].pSpecializationInfo = nullptr;

    auto bindingDescriptions = LveModel::Vertex::getBindingDescriptions();
    auto attributeDescriptions = LveModel::Vertex::getAttributeDescriptions();
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexAttributeDescriptionCount =
        static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.vertexBindingDescriptionCount =
        static_cast<uint32_t>(bindingDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
    vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType =
        VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
    pipelineInfo.pViewportState = &configInfo.viewportInfo;
    pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo;
    pipelineInfo.pMultisampleState = &configInfo.multisampleInfo;
    pipelineInfo.pColorBlendState = &configInfo.colorBlendInfo;
    pipelineInfo.pDepthStencilState = &configInfo.depthStencilInfo;
    pipelineInfo.pDynamicState = &configInfo.dynamicStateInfo;

    pipelineInfo.layout = configInfo.pipelineLayout;
    pipelineInfo.renderPass = configInfo.renderPass;
    pipelineInfo.subpass = configInfo.subpass;

    pipelineInfo.basePipelineIndex = -1;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(lveDevice.device(), VK_NULL_HANDLE,
        1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline");
    }
}

void LvePipeline::defaultPipelineConfigInfo(PipelineConfigInfo& ci) {
    ci.inputAssemblyInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    ci.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    ci.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

    ci.viewportInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    ci.viewportInfo.viewportCount = 1;
    ci.viewportInfo.pViewports = nullptr;
    ci.viewportInfo.scissorCount = 1;
    ci.viewportInfo.pScissors = nullptr;

    ci.rasterizationInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    ci.rasterizationInfo.depthClampEnable = VK_FALSE;
    ci.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
    ci.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
    ci.rasterizationInfo.lineWidth = 1.0f;
    ci.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
    ci.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    ci.rasterizationInfo.depthBiasEnable = VK_FALSE;
    ci.rasterizationInfo.depthBiasConstantFactor = 0.0f;
    ci.rasterizationInfo.depthBiasClamp = 0.0f;
    ci.rasterizationInfo.depthBiasSlopeFactor = 0.0f;

    ci.multisampleInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    ci.multisampleInfo.sampleShadingEnable = VK_FALSE;
    ci.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    ci.multisampleInfo.minSampleShading = 1.0f;
    ci.multisampleInfo.pSampleMask = nullptr;
    ci.multisampleInfo.alphaToCoverageEnable = VK_FALSE;
    ci.multisampleInfo.alphaToOneEnable = VK_FALSE;

    ci.colorBlendAttachement.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    ci.colorBlendAttachement.blendEnable = VK_FALSE;
    ci.colorBlendAttachement.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    ci.colorBlendAttachement.srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    ci.colorBlendAttachement.colorBlendOp = VK_BLEND_OP_ADD;
    ci.colorBlendAttachement.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    ci.colorBlendAttachement.srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    ci.colorBlendAttachement.alphaBlendOp = VK_BLEND_OP_ADD;

    ci.colorBlendInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    ci.colorBlendInfo.logicOpEnable = VK_FALSE;
    ci.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
    ci.colorBlendInfo.attachmentCount = 1;
    ci.colorBlendInfo.pAttachments = &ci.colorBlendAttachement;
    ci.colorBlendInfo.blendConstants[0] = 0.0f;
    ci.colorBlendInfo.blendConstants[1] = 0.0f;
    ci.colorBlendInfo.blendConstants[2] = 0.0f;
    ci.colorBlendInfo.blendConstants[3] = 0.0f;

    ci.depthStencilInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    ci.depthStencilInfo.depthTestEnable = VK_TRUE;
    ci.depthStencilInfo.depthWriteEnable = VK_TRUE;
    ci.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
    ci.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
    ci.depthStencilInfo.minDepthBounds = 0.0f;
    ci.depthStencilInfo.maxDepthBounds = 1.0f;
    ci.depthStencilInfo.stencilTestEnable = VK_FALSE;
    ci.depthStencilInfo.front = {};
    ci.depthStencilInfo.back = {};

    ci.dynamicStateEnable = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
    };
    ci.dynamicStateInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    ci.dynamicStateInfo.pDynamicStates = ci.dynamicStateEnable.data();
    ci.dynamicStateInfo.dynamicStateCount =
        static_cast<uint32_t>(ci.dynamicStateEnable.size());
    ci.dynamicStateInfo.flags = 0;
}

void LvePipeline::bind(VkCommandBuffer commandBuffer) {
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                      graphicsPipeline);
}

}  // namespace lve
