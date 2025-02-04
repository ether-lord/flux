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

  world.import <Shaders>();
  world.import <Textures>();
  world.import <Camera>();
  world.import <Movement>();
  world.import <Input>();

  world.system<Window>("WindowPreProcessing")
      .kind(flecs::OnLoad)
      .each([=](Window& window) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      });

  unsigned int meshes_vao, vbo, ebo;
  glGenVertexArrays(1, &meshes_vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glBindVertexArray(meshes_vao);

  world
      .system<const Mesh, const TextureHandle, const Transform>(
          "Meshses Buffering")
      .run([meshes_vao, vbo, ebo](flecs::iter& it) {
        vector<float> vbo_data;
        vector<unsigned int> indices;
        const int vertex_parameters = 6;

        auto renderables =
            it.world()
                .query<const Mesh, const TextureHandle, const Transform>();

        renderables.each([&](flecs::entity e, const Mesh& mesh,
                             const TextureHandle& texture,
                             const Transform& transform) {
          auto model = mat4(1.f);
          model = translate(model, transform.position);
          model = scale(model, transform.scale);
          model = rotate(model, radians(transform.rotation.x), {1.f, 0.f, 0.f});
          model = rotate(model, radians(transform.rotation.y), {0.f, 1.f, 0.f});
          model = rotate(model, radians(transform.rotation.z), {0.f, 0.f, 1.f});

          int offset = vbo_data.size() / vertex_parameters;
          for (const auto& i : mesh.indices) {
            indices.push_back(i + offset);
          }

          for (const auto& v : mesh.vertices) {
            auto pos = vec4(v.position, 1.f);
            pos = model * pos;
            auto uv = v.uv;

            vbo_data.push_back(pos.x);
            vbo_data.push_back(pos.y);
            vbo_data.push_back(pos.z);
            vbo_data.push_back(uv.x);
            vbo_data.push_back(uv.y);
            vbo_data.push_back(texture.id);
          }
        });

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vbo_data[0]) * vbo_data.size(),
                     &vbo_data[0], GL_STATIC_DRAW);

        auto stride = sizeof(Vertex) + sizeof(TextureHandle::id);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(indices[0]) * indices.size(), &indices[0],
                     GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride,
                              (void*)offsetof(Vertex, uv));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 1, GL_UNSIGNED_INT, GL_FALSE, stride,
                              (void*)sizeof(Vertex));
        glEnableVertexAttribArray(2);

        it.world().set<MeshesRenderData>(
            {meshes_vao, (unsigned int)indices.size()});
      });

  world.system<const MeshesRenderData>("Rendering")
      .kind(flecs::OnStore)
      .each([](flecs::iter it, size_t, const MeshesRenderData& render_data) {
        auto projection = it.world().get<Projection>();

        auto fly_camera = it.world().entity<FlyCamera>();
        auto camera_position_cpmponent = fly_camera.get<Position>();
        auto camera_direction_component = fly_camera.get<Direction>();
        vec3 camera_position{camera_position_cpmponent->x,
                             camera_position_cpmponent->y,
                             camera_position_cpmponent->z};
        vec3 camera_up = fly_camera.get<FlyCamera>()->up;
        vec3 camera_target = fly_camera.get<FlyCamera>()->target;

        auto meshes_vao = render_data.vao;
        auto indices = render_data.indices;
        glBindVertexArray(meshes_vao);

        auto shader = it.world().lookup("flux::Shaders::default");
        if (!shader.is_valid() || !shader.has<Shader>()) return;

        auto shader_id = shader.get<Shader>()->id;
        glUseProgram(shader_id);

        auto view =
            lookAt(camera_position, camera_position + camera_target, camera_up);
        int view_loc = glGetUniformLocation(shader_id, "view");
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, value_ptr(view));
        int projection_loc = glGetUniformLocation(shader_id, "projection");
        glUniformMatrix4fv(projection_loc, 1, GL_FALSE,
                           value_ptr(projection->matirx));

        glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

}  // namespace flux