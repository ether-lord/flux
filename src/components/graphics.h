#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <glfw3.h>

namespace flux::components {

struct Window {
  GLFWwindow* ptr;
  const GLFWvidmode* video_mode;
};

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

struct Mesh {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
};

struct MeshBuffer {
  unsigned int vao;
  unsigned int indices;
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

struct FlyCamera {
  glm::vec3 position{0.f, 0.f, 3.f};
  glm::vec3 target{0.f, 0.f, -1.f};
  glm::vec3 up{0.f, 1.f, 0.f};
  glm::vec3 right;
  glm::vec3 world_up{0.f, 1.f, 0.f};

  float pitch;
  float yaw;
  float speed;
  float sensitivity;
  float zoom;
};

}  // namespace flux::components
