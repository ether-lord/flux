#include <iostream>

#include "resources_manager.h"
#include "shader.h"
#include "shader_program.h"
#include "system_manager.h"
#include "types.h"

using namespace flux;
using namespace std;

using namespace flux::shader;
using namespace flux::types;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int main() {
  resources::ResourcesManager resources_manager;
  system::SystemManager sys_manager;

  auto window = sys_manager.GetWindow();

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  auto vertex_shader_src = resources_manager.GetShaderSource("vertex");
  auto vertex_shader = Shader(ShaderType::VERTEX, vertex_shader_src);
  vertex_shader.Compile();

  if (!vertex_shader.GetCompileStatus()) {
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << vertex_shader.GetInfoLog() << std::endl;
  }

  auto fragment_shader_src = resources_manager.GetShaderSource("fragment");
  auto fragment_shader = Shader(ShaderType::FRAGMENT, fragment_shader_src);
  fragment_shader.Compile();

  if (!fragment_shader.GetCompileStatus()) {
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << fragment_shader.GetInfoLog() << std::endl;
  }

  auto shader_program = ShaderProgram();
  shader_program.AttachShader(vertex_shader);
  shader_program.AttachShader(fragment_shader);
  shader_program.Link();
  shader_program.Use();

  if (!shader_program.GetLinkStatus()) {
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << shader_program.GetInfoLog() << std::endl;
  }

  glDeleteShader(fragment_shader.GetId());
  glDeleteShader(fragment_shader.GetId());

  float vertices[] = {
      0.5f,  0.5f,  0.0f,  // top right
      0.5f,  -0.5f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f,  // bottom left
      -0.5f, 0.5f,  0.0f   // top left
  };
  unsigned int indices[] = {
      // note that we start from 0!
      0, 1, 3,  // first Triangle
      1, 2, 3   // second Triangle
  };

  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  shader_program.Delete();

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
