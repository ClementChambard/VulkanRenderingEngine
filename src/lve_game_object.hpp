// Copyright dummy
#ifndef LVEGAMEOBJECT_INCLUDED_H
#define LVEGAMEOBJECT_INCLUDED_H

#include "./lve_model.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <memory>

namespace lve {

struct TransformComponent {
    glm::vec3 translation{};
    glm::vec3 scale{1.0f, 1.0f, 1.0f};
    glm::vec3 rotation{};

    glm::mat4 mat4() {
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
};

class LveGameObject {
 public:
    using id_t = unsigned int;

    LveGameObject(LveGameObject&) = delete;
    LveGameObject& operator=(LveGameObject&) = delete;
    LveGameObject(LveGameObject&&) = default;
    LveGameObject& operator=(LveGameObject&&) = default;

    static LveGameObject createGameObject() {
        static id_t currentId = 0;
        return LveGameObject(currentId++);
    }

    id_t getId() const { return id; }

    std::shared_ptr<LveModel> model{};
    glm::vec3 color{};
    TransformComponent transformComponent{};

 private:
    explicit LveGameObject(id_t objId) : id(objId) {}

    id_t id;

};  // class LveGameObject

}  // namespace lve

#endif  // LVEGAMEOBJECT_INCLUDED_H
