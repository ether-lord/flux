#pragma once

#define GLFW_INCLUDE_NONE
#define LOG_BUFF_SIZE 1024u

#include <vector>

#include "flecs.h"
#include "glfw3.h"
#include "glm/glm.hpp"

namespace flux {

typedef struct {
  unsigned int count;
  unsigned int instance_count;
  unsigned int first_index;
  unsigned int base_vertex;
  unsigned int base_instance;
} DrawElementsIndirectCommand;

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 uv;
};

struct MeshLayout {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 uv;
};

struct Mesh {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
};

struct Transform {
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale{1.f, 1.f, 1.f};
};

struct Projection {
  glm::mat4 matirx;
};

struct View {
  glm::mat4 matrix;
};

struct MeshesGeometry {
  unsigned int vao;
  unsigned int indices;
};

struct Render {
  Render(flecs::world& world);
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

}  // namespace flux
