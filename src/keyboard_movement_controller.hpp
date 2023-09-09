// Copyright dummy
#ifndef KEYBOARDMOVEMENTCONTROLLER_INCLUDED_H
#define KEYBOARDMOVEMENTCONTROLLER_INCLUDED_H

#include "./lve_game_object.hpp"
#include "./lve_window.hpp"

namespace lve {

class KeyboardMovementController {
 public:
  struct KeyMappings {
    int moveLeft = GLFW_KEY_A;
    int moveRight = GLFW_KEY_D;
    int moveForwards = GLFW_KEY_W;
    int moveBackwards = GLFW_KEY_S;
    int moveUp = GLFW_KEY_SPACE;
    int moveDown = GLFW_KEY_LEFT_SHIFT;
    int lookLeft = GLFW_KEY_LEFT;
    int lookRight = GLFW_KEY_RIGHT;
    int lookUp = GLFW_KEY_UP;
    int lookDown = GLFW_KEY_DOWN;
  };  // struct KeyMappings

  void moveInPlaneXZ(GLFWwindow* window, float dt, LveGameObject& gameObject);

  KeyMappings keys{};
  float moveSpeed{3.f};
  float lookSpeed{1.5f};
};  // class KeyboardMovementController

}  // namespace lve

#endif
