#pragma once

#include <glm/glm.hpp>

namespace flux::camera {

class Camera {
  using vec3 = glm::vec3;
  using mat4 = glm::mat4;

 public:
  Camera();

  mat4 GetViewMatrix() const;

  float yaw;
  float pitch;
  float fov;

  vec3 position;
  vec3 target;
  vec3 up;

 private:
};

}  // namespace flux::camera