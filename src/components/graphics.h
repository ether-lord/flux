#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

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
  glm::vec2 uv;
};

struct Texture {
  std::string name;
};

struct TextureBuffer {
  unsigned int id;
};

struct Mesh  {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
};

struct MeshBuffer {
  unsigned int vao;
  unsigned int indices;
};

struct Model {
  std::vector<MeshBuffer> meshes;
  std::string path;
};

}  // namespace flux::components
