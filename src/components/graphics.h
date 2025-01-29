#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace flux::components {

struct Shape2d {
  unsigned int vao;
  unsigned long vertices;
};

struct Shape2dData {
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec3> colors;
  std::vector<unsigned int> indices;
};

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 uv;
};

struct Texture {
  unsigned int id;
  std::string name;
};

struct MeshData  {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  Texture texture;
};

struct Mesh {
  unsigned int vao;
  unsigned int texture_id;
};

struct Model {
  std::vector<Mesh> meshes;
  std::string path;
};

}  // namespace flux::components
