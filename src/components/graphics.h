#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace flux::components {

struct Shape2dBuffered {
  unsigned int vao;
  unsigned long edges;
};

struct Shape2d {
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec3> colors;
  std::vector<unsigned int> indices;
};

}  // namespace flux::components
