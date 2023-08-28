// Copyright dummy
#ifndef LVECAMERA_INCLUDED_H
#define LVECAMERA_INCLUDED_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace lve {

class LveCamera {
 public:
    void setOrthographicProjection(float left, float right,
                                   float top, float bottom,
                                   float near, float far);

    void setPerspectiveProjection(float fovy, float aspect,
                                  float near, float far);

    void setViewDirection(glm::vec3 const& position,
                          glm::vec3 const& direction,
                          glm::vec3 const& up = {0, -1, 0});
    void setViewTarget(glm::vec3 const& position,
                       glm::vec3 const& target,
                       glm::vec3 const& up = {0, -1, 0});
    void setViewYXZ(glm::vec3 const& position,
                    glm::vec3 const& rotation);

    const glm::mat4& getProjection() const { return projectionMatrix; }
    const glm::mat4& getView() const { return viewMatrix; }
 private:
    glm::mat4 projectionMatrix{1.0f};
    glm::mat4 viewMatrix{1.0f};
};  // class LveCamera

}  // namespace lve

#endif  // LVECAMERA_INCLUDED_H
