#include "render.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include "components/graphics.h"
#include "components/input.h"
#include "components/shader.h"

using namespace glm;
using namespace std;

using namespace flux::components;

namespace flux::modules {

Render::Render(flecs::world& world) {
  world
      .system<const MeshBuffer, const TextureBuffer, const Transform>(
          "MeshRenderer")
      .kind(flecs::OnStore)
      .each([](flecs::iter& it, size_t, const MeshBuffer& mesh,
               const TextureBuffer& texture, const Transform& transform) {
        auto shader = it.world().entity("BasicShader").get<Shader>();
        auto camera = it.world().get<FlyCamera>();
        auto projection = it.world().get<Projection>();

        glBindVertexArray(mesh.vao);

        auto model = mat4(1.f);
        model = translate(model, transform.position);
        model = scale(model, transform.scale);
        model = rotate(model, radians((float)glfwGetTime()), {1.f, 0.f, 0.f});
        model = rotate(model, radians(transform.rotation.y), {0.f, 1.f, 0.f});
        model = rotate(model, radians(transform.rotation.z), {0.f, 0.f, 1.f});

        glUseProgram(shader->id);
        int model_loc = glGetUniformLocation(shader->id, "model");
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, value_ptr(model));
        auto view = lookAt(camera->position, camera->position + camera->target,
                           camera->up);
        int view_loc = glGetUniformLocation(shader->id, "view");
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, value_ptr(view));
        int projection_loc = glGetUniformLocation(shader->id, "projection");
        glUniformMatrix4fv(projection_loc, 1, GL_FALSE,
                           value_ptr(projection->matirx));

        glBindTexture(GL_TEXTURE_2D, texture.id);
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

  world.set<Window>(Window{glfw_window, video_mode});

  world.system<Window>().kind(flecs::OnLoad).each([=](Window& window) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  });

  world.system<Window, const InputHandler>()
      .kind(flecs::PostLoad)
      .each([](flecs::entity e, Window& window, const InputHandler& input) {
        auto keyboard_events = e.world().get<Input>()->keyboard_events;

        if (keyboard_events.count(KeyboardKey::kEscape))
          glfwSetWindowShouldClose(
              window.ptr,
              keyboard_events[KeyboardKey::kEscape] == KeyState::kReleased);
      });
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

}  // namespace flux::modules