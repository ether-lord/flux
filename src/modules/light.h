#pragma once

#include <flecs.h>

#include <glm/glm.hpp>

namespace flux {
  
struct AmbientLight {
  glm::vec3 color;
  float intensity;
};

struct Light {
  Light(flecs::world& world);
};

}  // namespace flux