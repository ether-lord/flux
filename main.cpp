#include <iostream>
#include <vector>

#define GLFW_INCLUDE_NONE

#include "flecs.h"
#include "glad.h"
#include "glfw3.h"
#include "glm/glm.hpp"
#include "resources_manager.h"
#include "shader.h"
#include "shader_program.h"

using namespace std;
using namespace flecs;
using namespace glm;
using namespace flux;
using namespace flux::shader;
using namespace flux::resources;

struct BufferableShape2d {
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec3> colors;
  std::vector<unsigned int> indices;
};

struct Buffered {};

struct Shape2d {
  unsigned int vao;
  unsigned long edges;
};

void process_input(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  auto monitor = glfwGetPrimaryMonitor();
  auto video_mode = glfwGetVideoMode(monitor);

  auto window = glfwCreateWindow(video_mode->width, video_mode->height, "FLUX",
                                 monitor, NULL);

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  glEnable(GL_DEPTH_TEST);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  world engine;

  auto triangle = engine.entity("triangle").add<BufferableShape2d>();

  std::vector<glm::vec3> vertices = {
      {-0.5f, -0.5f, 0.f}, {0.0f, 0.5f, 0.f}, {0.5f, -0.5f, 0.f}};
  std::vector<glm::vec3> colors = {{0, 0, 1}, {0, 1, 0}, {1, 0, 0}};
  std::vector<unsigned int> indices = {0, 1, 2};

  triangle.set<BufferableShape2d>(BufferableShape2d{
      .vertices = vertices, .colors = colors, .indices = indices});

      auto buffering_func = [](flecs::entity e, BufferableShape2d& bs2d) {
    auto indices = bs2d.indices;
    auto colors = bs2d.colors;
    auto vertices = bs2d.vertices;

    vector<float> vbo_data;
    for (const auto& i : vertices) {
      vbo_data.push_back(i.x);
      vbo_data.push_back(i.y);
      vbo_data.push_back(i.z);
    }
    for (const auto& i : colors) {
      vbo_data.push_back(i.x);
      vbo_data.push_back(i.y);
      vbo_data.push_back(i.z);
    }
    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vbo_data[0]) * vbo_data.size(),
                 &vbo_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(),
                 &indices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0,
                          (void*)(sizeof(vertices[0]) * vertices.size()));
    glEnableVertexAttribArray(1);

    e.remove<BufferableShape2d>();
    e.set<Shape2d>(Shape2d{vao, indices.size()});
  };

  auto buffering_system = engine.system<BufferableShape2d>()
                              .kind(flecs::OnLoad)
                              .each(buffering_func);

  // ===== Move to shader system =====
  auto shader_src = ResourcesManager::get().GetShaderSource("basic_vertex");
  auto vertex_shader = Shader(ShaderType::VERTEX, shader_src);
  vertex_shader.Compile();
  if (!vertex_shader.GetCompileStatus()) {
    cout << vertex_shader.GetInfoLog() << endl;
  }
  shader_src = ResourcesManager::get().GetShaderSource("basic_frag");
  auto frag_shader = Shader(ShaderType::FRAGMENT, shader_src);
  frag_shader.Compile();
  if (!frag_shader.GetCompileStatus()) {
    cout << frag_shader.GetInfoLog() << endl;
  }

  auto shader_program = ShaderProgram();
  shader_program.AttachShader(vertex_shader);
  shader_program.AttachShader(frag_shader);
  shader_program.Link();
  shader_program.Use();

  if (!shader_program.GetLinkStatus()) {
    cout << shader_program.GetInfoLog() << endl;
  }
  // ===== Move to shader system =====

  auto render_system =
      engine.system<Shape2d>().kind(flecs::OnStore).each([=](Shape2d& shape) {
        process_input(window);

        glClearColor(0.009f, 0.195f, 0.0126f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(shape.vao);
        glDrawElements(GL_TRIANGLES, shape.edges, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
      });

  while (!glfwWindowShouldClose(window)) {
    engine.progress();
  }

  return 0;
}

void process_input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}
