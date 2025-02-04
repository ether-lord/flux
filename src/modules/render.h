#pragma once

#include <flecs.h>

#define LOG_BUFF_SIZE 1024u
#define GLFW_INCLUDE_NONE

#include <glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <vector>

namespace flux {

struct Vertex {
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

struct MeshesRenderData {
  unsigned int vao;
  unsigned int indices;
};

struct AmbientLight {
  glm::vec3 color;
  float intensity;
};

struct Render {
  Render(flecs::world& world);
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

}  // namespace flux
