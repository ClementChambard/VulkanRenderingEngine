// Copyright dummy
#include "./lve_camera.hpp"

#include <cassert>
#include <limits>

namespace lve {

void LveCamera::setOrthographicProjection(float left, float right,
                                          float top, float bottom,
                                          float near, float far) {
    projectionMatrix = glm::mat4(1.0f);
    projectionMatrix[0][0] = 2.0f / (right - left);
    projectionMatrix[1][1] = 2.0f / (bottom - top);
    projectionMatrix[2][2] = 1.0f / (far - near);
    projectionMatrix[3][0] = -(right + left) / (right - left);
    projectionMatrix[3][1] = -(bottom + top) / (bottom - top);
    projectionMatrix[3][2] = -near / (far - near);
}

void LveCamera::setPerspectiveProjection(float fovy, float aspect,
                                         float near, float far) {
    assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
    const float tanHalfFovy = tan(fovy / 2.0f);
    projectionMatrix = glm::mat4(0.0f);
    projectionMatrix[0][0] = 1.0f / (aspect * tanHalfFovy);
    projectionMatrix[1][1] = 1.0f / tanHalfFovy;
    projectionMatrix[2][2] = far / (far - near);
    projectionMatrix[2][3] = 1.0f;
    projectionMatrix[3][2] = -(far * near) / (far - near);
}

void LveCamera::setViewDirection(glm::vec3 const& position,
                                 glm::vec3 const& direction,
                                 glm::vec3 const& up) {
    const glm::vec3 w{glm::normalize(direction)};
    const glm::vec3 u{glm::normalize(glm::cross(w, up))};
    const glm::vec3 v{glm::cross(w, u)};

    viewMatrix = glm::mat4(1.0f);
    viewMatrix[0][0] = u.x;
    viewMatrix[1][0] = u.y;
    viewMatrix[2][0] = u.z;
    viewMatrix[0][1] = v.x;
    viewMatrix[1][1] = v.y;
    viewMatrix[2][1] = v.z;
    viewMatrix[0][2] = w.x;
    viewMatrix[1][2] = w.y;
    viewMatrix[2][2] = w.z;
    viewMatrix[3][0] = -glm::dot(u, position);
    viewMatrix[3][1] = -glm::dot(v, position);
    viewMatrix[3][2] = -glm::dot(w, position);
}

void LveCamera::setViewTarget(glm::vec3 const& position,
                              glm::vec3 const& target,
                              glm::vec3 const& up) {
    setViewDirection(position, target - position, up);
}

void LveCamera::setViewYXZ(glm::vec3 const& position,
                           glm::vec3 const& rotation) {
    const float cx = glm::cos(rotation.x);
    const float sx = glm::sin(rotation.x);
    const float cy = glm::cos(rotation.y);
    const float sy = glm::sin(rotation.y);
    const float cz = glm::cos(rotation.z);
    const float sz = glm::sin(rotation.z);
    const glm::vec3 u{(sy*sx*sz + cy*cz), (cx*sz), (cy*sx*sz - cz*sy)};
    const glm::vec3 v{(sy*sx*cz - cx*cz), (cx*cz), (cy*sx*cz + sz*sy)};
    const glm::vec3 w{(cx*sy), -sx, (cy*cx)};
    viewMatrix[0][0] = u.x;
    viewMatrix[1][0] = u.y;
    viewMatrix[2][0] = u.z;
    viewMatrix[0][1] = v.x;
    viewMatrix[1][1] = v.y;
    viewMatrix[2][1] = v.z;
    viewMatrix[0][2] = w.x;
    viewMatrix[1][2] = w.y;
    viewMatrix[2][2] = w.z;
    viewMatrix[3][0] = -glm::dot(u, position);
    viewMatrix[3][1] = -glm::dot(v, position);
    viewMatrix[3][2] = -glm::dot(w, position);
}

}  // namespace lve
