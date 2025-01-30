#include "render.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "components/graphics.h"
#include "components/input.h"
#include "components/shader.h"

using namespace glm;

using namespace flux::components;

namespace flux::modules {

Render::Render(flecs::world& world) {
  Camera camera;
  camera.yaw = -90.f;
  camera.pitch = 0.f;
  camera.speed = 2.5f;
  camera.zoom = 45.f;
  camera.sensitivity = 0.1f;

  world.set<Camera>(camera);

  world
      .system<const MeshBuffer, const TextureBuffer, const Transform>(
          "MeshRenderer")
      .kind(flecs::OnStore)
      .each([](flecs::iter& it, size_t, const MeshBuffer& mesh,
               const TextureBuffer& texture, const Transform& transform) {
        auto shader = it.world().lookup("BasicShader").get<Shader>();
        auto camera = it.world().get<Camera>();
        auto projection = it.world().get<Projection>();

        auto model = mat4(1.f);
        model = translate(model, transform.position);
        model = scale(model, transform.scale);
        model = rotate(model, radians(transform.rotation.x), {1.f, 0.f, 0.f});
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

  world.set<Window>(Window{glfw_window, video_mode});

  world.system<Window>().kind(flecs::OnLoad).each([=](Window& window) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  });

  world.system<Window, const Input>()
      .kind(flecs::PostLoad)
      .each([](flecs::entity e, Window& window, const Input& input) {
        switch (input.key) {
          case KeyboardKey::kEscape:
            glfwSetWindowShouldClose(window.ptr,
                                     input.state == KeyState::kPressed);
            break;
          default:
            break;
        }

        e.remove<Input>();
      });
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

}  // namespace flux::modules