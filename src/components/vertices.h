#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace flux::components {

struct Polygon2d {
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec3> colors;
  std::vector<unsigned int> indices;
};

}  // namespace flux::component