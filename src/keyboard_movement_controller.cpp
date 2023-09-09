#include "keyboard_movement_controller.hpp"
#include <limits>

namespace lve {

void KeyboardMovementController::moveInPlaneXZ(GLFWwindow* window, float dt, LveGameObject& gameObject) {
    glm::vec3 rotate{0};
    if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate.y += 1;
    if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate.y -= 1;
    if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate.x += 1;
    if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate.x -= 1;

    if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
        gameObject.transformComponent.rotation += lookSpeed * dt * glm::normalize(rotate);

    gameObject.transformComponent.rotation.x = glm::clamp(gameObject.transformComponent.rotation.x, -1.5f, 1.5f);
    gameObject.transformComponent.rotation.y = glm::mod(gameObject.transformComponent.rotation.y, glm::two_pi<float>());

    float yaw = gameObject.transformComponent.rotation.y;
    const glm::vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
    const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
    const glm::vec3 upDir{0.f, -1.f, 0.f};

    glm::vec3 moveDir{0};
    if (glfwGetKey(window, keys.moveForwards) == GLFW_PRESS) moveDir += forwardDir;
    if (glfwGetKey(window, keys.moveBackwards) == GLFW_PRESS) moveDir -= forwardDir;
    if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
    if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
    if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir += upDir;
    if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir -= upDir;

    if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon())
        gameObject.transformComponent.translation += moveSpeed * dt * glm::normalize(moveDir);
}

}  // namespace lve