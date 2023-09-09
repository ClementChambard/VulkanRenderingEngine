// Copyright dummy
#include "./first_app.hpp"
#include "./simple_render_system.hpp"
#include "./lve_camera.hpp"
#include "./keyboard_movement_controller.hpp"
#include <cassert>
#include <vulkan/vulkan_core.h>
#include <stdexcept>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <chrono>

namespace lve {

FirstApp::FirstApp() {
    loadGameObjects();
}

FirstApp::~FirstApp() {}

void FirstApp::run() {
    SimpleRenderSystem simpleRenderSystem(lveDevice,
                                        lveRenderer.getSwapChainRenderPass());
    LveCamera camera{};

    auto viewerObject = LveGameObject::createGameObject();
    KeyboardMovementController cameraController{};

    auto currentTime = std::chrono::high_resolution_clock::now();
    while (!lveWindow.shouldClose()) {
        glfwPollEvents();

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime =
            std::chrono::duration<float, std::chrono::seconds::period>
            (newTime - currentTime).count();
        currentTime = newTime;

        // frameTime = glm::min(frameTime, MAX_FRAME_TIME);

        cameraController.moveInPlaneXZ(lveWindow.getGlfwWindow(),
                                       frameTime, viewerObject);
        camera.setViewYXZ(viewerObject.transformComponent.translation,
                          viewerObject.transformComponent.rotation);

        float aspect = lveRenderer.getAspectRatio();
        camera.setPerspectiveProjection(glm::radians(50.0f),
                                        aspect, 0.1f, 10.0f);

        if (auto commandBuffer = lveRenderer.beginFrame()) {
            lveRenderer.beginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects,
                                                 camera);
            lveRenderer.endSwapChainRenderPass(commandBuffer);
            lveRenderer.endFrame();
        }
    }
    vkDeviceWaitIdle(lveDevice.device());
}

void FirstApp::loadGameObjects() {
    std::shared_ptr<LveModel> lveModel =
        LveModel::createModelFromFile(lveDevice, "models/flat_vase.obj");
    std::shared_ptr<LveModel> lveModel2 =
        LveModel::createModelFromFile(lveDevice, "models/smooth_vase.obj");

    auto gameObj = LveGameObject::createGameObject();
    gameObj.model = lveModel;
    gameObj.transformComponent.translation = {-.5f, 0.5f, 2.5f};
    gameObj.transformComponent.scale = glm::vec3(3);
    gameObjects.push_back(std::move(gameObj));

    auto gameObj2 = LveGameObject::createGameObject();
    gameObj2.model = lveModel2;
    gameObj2.transformComponent.translation = {0.5f, 0.5f, 2.5f};
    gameObj2.transformComponent.scale = glm::vec3(3);
    gameObjects.push_back(std::move(gameObj2));
}

}  // namespace lve
