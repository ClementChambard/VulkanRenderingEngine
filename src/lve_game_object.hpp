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

    glm::mat4 mat4();
    glm::mat3 normalMatrix();
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
