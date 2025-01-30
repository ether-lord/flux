#include "render.h"

#include "components/graphics.h"
#include "components/shader.h"

using namespace flux::components;

namespace flux::modules {

Render::Render(flecs::world& world) {
  world.component<Shape2d>();

  world.system<const Shape2d>("Shape2dRenderer")
      .kind(flecs::OnStore)
      .each([](flecs::iter& it, size_t, const Shape2d& shape) {
        auto shader = it.world().lookup("ColorShader").get<Shader>();

        glUseProgram(shader->id);
        glBindVertexArray(shape.vao);
        glDrawElements(GL_TRIANGLES, shape.vertices, GL_UNSIGNED_INT, 0);
      });

  world.system<const MeshBuffer, const TextureBuffer>("MeshRenderer")
      .kind(flecs::OnStore)
      .each([](flecs::iter& it, size_t, const MeshBuffer& mesh,
               const TextureBuffer& texture) {
        auto shader = it.world().lookup("BasicShader").get<Shader>();

        glUseProgram(shader->id);
        glBindTexture(GL_TEXTURE_2D, texture.id);
        glBindVertexArray(mesh.vao);
        glDrawElements(GL_TRIANGLES, mesh.indices, GL_UNSIGNED_INT, 0);
      });
}

WindowPreProcessing::WindowPreProcessing(flecs::world& world) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  auto monitor = glfwGetPrimaryMonitor();
  auto video_mode = glfwGetVideoMode(monitor);

  auto glfw_window = glfwCreateWindow(video_mode->width, video_mode->height,
                                      "FLUX", monitor, NULL);

  glfwMakeContextCurrent(glfw_window);
  glfwSetFramebufferSizeCallback(glfw_window, framebuffer_size_callback);

  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  glEnable(GL_DEPTH_TEST);
  glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  world.set<Window>({glfw_window});

  world.system<Window>().kind(flecs::OnLoad).each([=](Window& window) {
    process_input(window.ptr);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  });
}

void process_input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

}  // namespace flux::modules