#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "glad.h"

using vec3 = glm::vec3;
using mat4 = glm::mat4;

namespace flux::camera {

Camera::Camera() {
  yaw = 90.f;
  pitch = 0.f;
  fov = 60.f;

  position = vec3(0.f, 0.f, 0.f);
  target = vec3(0.f, 0.f, -1.f);
  up = vec3(0.f, 1.f, 0.f);
}

mat4 Camera::GetViewMatrix() const {
  return glm::lookAt(position, position + target, up);
}

}  // namespace flux::camera