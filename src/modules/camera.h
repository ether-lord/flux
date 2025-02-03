#pragma once

#include <flecs.h>

#include <glm/vec3.hpp>

namespace flux {

struct FlyCamera {
  glm::vec3 target{0.f, 0.f, -1.f};
  glm::vec3 up{0.f, 1.f, 0.f};
  glm::vec3 right;
  glm::vec3 world_up{0.f, 1.f, 0.f};

  float pitch;
  float yaw;
  float sensitivity;
  float zoom;
};

struct Camera {
  Camera(flecs::world& world);
};

}  // namespace flux
