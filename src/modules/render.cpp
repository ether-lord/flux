#include "render.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include "modules/camera.h"
#include "modules/input.h"
#include "modules/movement.h"
#include "modules/shaders.h"
#include "modules/textures.h"
#include "modules/window.h"

using namespace glm;
using namespace std;

using namespace flux;

namespace flux {

Render::Render(flecs::world& world) {
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

  world.import <Buffering>();
  world.import <Shaders>();
  world.import <Textures>();
  world.import <Camera>();
  world.import <Movement>();
  world.import <Input>();

  world.system<const MeshBuffer, const Texture, const Transform>("MeshRenderer")
      .kind(flecs::OnStore)
      .each([](flecs::iter& it, size_t, const MeshBuffer& mesh,
               const Texture& texture, const Transform& transform) {
        auto projection = it.world().get<Projection>();

        auto fly_camera = it.world().entity<FlyCamera>();
        auto camera_position_cpmponent = fly_camera.get<Position>();
        auto camera_direction_component = fly_camera.get<Direction>();
        vec3 camera_position{camera_position_cpmponent->x,
                             camera_position_cpmponent->y,
                             camera_position_cpmponent->z};
        vec3 camera_up = fly_camera.get<FlyCamera>()->up;
        vec3 camera_target = fly_camera.get<FlyCamera>()->target;

        glBindVertexArray(mesh.vao);

        auto model = mat4(1.f);
        model = translate(model, transform.position);
        model = scale(model, transform.scale);
        model = rotate(model, radians(transform.rotation.x), {1.f, 0.f, 0.f});
        model = rotate(model, radians(transform.rotation.y), {0.f, 1.f, 0.f});
        model = rotate(model, radians(transform.rotation.z), {0.f, 0.f, 1.f});

        auto shader = it.world().lookup("flux::Shaders::default");
        if (!shader.is_valid() || !shader.has<Shader>()) return;

        auto shader_id = shader.get<Shader>()->id;
        glUseProgram(shader_id);

        int model_loc = glGetUniformLocation(shader_id, "model");
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, value_ptr(model));
        auto view =
            lookAt(camera_position, camera_position + camera_target, camera_up);
        int view_loc = glGetUniformLocation(shader_id, "view");
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, value_ptr(view));
        int projection_loc = glGetUniformLocation(shader_id, "projection");
        glUniformMatrix4fv(projection_loc, 1, GL_FALSE,
                           value_ptr(projection->matirx));

        glBindTexture(GL_TEXTURE_2D, texture.id);
        glDrawElements(GL_TRIANGLES, mesh.indices, GL_UNSIGNED_INT, 0);
      });

  world.system<Window>("WindowPreProcessing")
      .kind(flecs::OnLoad)
      .each([=](Window& window) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      });

  world.system<Window, const InputTarget>("WindowInputHandler")
      .kind(flecs::PostLoad)
      .each([](flecs::entity e, Window& window, const InputTarget& input) {
        auto keyboard_events = e.world().get<InputData>()->keyboard_events;

        if (keyboard_events.count(KeyboardKey::kEscape))
          glfwSetWindowShouldClose(
              window.ptr,
              keyboard_events[KeyboardKey::kEscape] == KeyState::kReleased);
      });
}

Buffering::Buffering(flecs::world& world) {
  world.component<Mesh>();
  world.component<MeshBuffer>();

  world.system<const Mesh>("Mesh Buffering System")
      .kind(flecs::OnLoad)
      .each([](flecs::entity e, const Mesh& mesh_data) {
        const auto& vertices = mesh_data.vertices;
        const auto& indices = mesh_data.indices;

        vector<float> vbo_data;
        for (const auto& v : vertices) {
          const auto& pos = v.position;
          const auto& uv = v.uv;
          vbo_data.push_back(pos.x);
          vbo_data.push_back(pos.y);
          vbo_data.push_back(pos.z);
          vbo_data.push_back(uv.x);
          vbo_data.push_back(uv.y);
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
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(indices[0]) * indices.size(), &indices[0],
                     GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void*)offsetof(Vertex, uv));
        glEnableVertexAttribArray(1);

        e.remove<Mesh>();
        e.set<MeshBuffer>(
            MeshBuffer{.vao = vao, .indices = (unsigned int)indices.size()});
      });
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

}  // namespace flux