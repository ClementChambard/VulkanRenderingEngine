#include "lve_game_object.hpp"

namespace lve {

glm::mat4 TransformComponent::mat4() {
    const float cx = glm::cos(rotation.x);
    const float sx = glm::sin(rotation.x);
    const float cy = glm::cos(rotation.y);
    const float sy = glm::sin(rotation.y);
    const float cz = glm::cos(rotation.z);
    const float sz = glm::sin(rotation.z);
    return {
      {
        scale.x * (cy * cz + sy * sx * sz),
        scale.x * (cx * sz),
        scale.x * (cy * sx * sz - cz * sy),
        0.0f
      },
      {
        scale.y * (cz * sy * sx - cy * sz),
        scale.y * (cx * cz),
        scale.y * (cy * cz * sx + sy * sz),
        0.0f
      },
      {
        scale.z * (cx * sy),
        scale.z * (-sx),
        scale.z * (cy * cx),
        0.0f
      },
      {translation, 1.0f}
    };
}

glm::mat3 TransformComponent::normalMatrix() {
    const float cx = glm::cos(rotation.x);
    const float sx = glm::sin(rotation.x);
    const float cy = glm::cos(rotation.y);
    const float sy = glm::sin(rotation.y);
    const float cz = glm::cos(rotation.z);
    const float sz = glm::sin(rotation.z);
    const glm::vec3 invScale = 1.f / scale;
    return {
      {
        invScale.x * (cy * cz + sy * sx * sz),
        invScale.x * (cx * sz),
        invScale.x * (cy * sx * sz - cz * sy),
      },
      {
        invScale.y * (cz * sy * sx - cy * sz),
        invScale.y * (cx * cz),
        invScale.y * (cy * cz * sx + sy * sz),
      },
      {
        invScale.z * (cx * sy),
        invScale.z * (-sx),
        invScale.z * (cy * cx),
      },
    };
}

}  // namespace lve
