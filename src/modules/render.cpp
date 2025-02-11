#include "render.h"

#include <iostream>

#include "camera.h"
#include "glad.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "input.h"
#include "light.h"
#include "movement.h"
#include "shaders.h"
#include "textures.h"
#include "window.h"

using namespace glm;
using namespace std;

namespace flux {

static std::unordered_map<unsigned int, DrawElementsIndirectCommand>
    mesh_id_to_draw_command;

Render::Render(flecs::world& world) {
  world.component<Mesh>().add(flecs::Target);

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
  world.import <Light>();

  unsigned int meshes_geometry;
  glGenVertexArrays(1, &meshes_geometry);
  glBindVertexArray(meshes_geometry);

  unsigned int meshes_vertex_buffer;
  glCreateBuffers(1, &meshes_vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, meshes_vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8192, 0, GL_STATIC_DRAW);

  unsigned int meshes_index_buffer;
  glCreateBuffers(1, &meshes_index_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes_index_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * 8192, 0,
               GL_STATIC_DRAW);

  GLuint draw_cmd_buffer;
  glCreateBuffers(1, &draw_cmd_buffer);
  glBindBuffer(GL_DRAW_INDIRECT_BUFFER, draw_cmd_buffer);

  auto persistent_mapped_buffer_flags =
      GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
  glBufferStorage(GL_DRAW_INDIRECT_BUFFER, 8192, 0,
                  persistent_mapped_buffer_flags);
  DrawElementsIndirectCommand* draw_commands_data =
      (DrawElementsIndirectCommand*)glMapBufferRange(
          GL_DRAW_INDIRECT_BUFFER, 0, 8192, persistent_mapped_buffer_flags);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshLayout), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshLayout),
                        (void*)offsetof(Vertex, normal));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshLayout),
                        (void*)offsetof(Vertex, uv));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(3, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(MeshLayout),
                        (void*)sizeof(Vertex));
  glEnableVertexAttribArray(3);

  world.observer<Mesh>("Mesh buffering")
      .event(flecs::OnSet)
      .each([meshes_vertex_buffer, meshes_index_buffer, draw_commands_data,
             this](flecs::entity e, Mesh& mesh) {
        static unsigned int index_offset = 0;
        static unsigned int vertex_offset = 0;

        if (mesh_id_to_draw_command.count(e.id()) > 0) return;

        DrawElementsIndirectCommand draw_command = {
            .count = (unsigned int)mesh.indices.size(),
            .instance_count = 1,
            .first_index = index_offset,
            .base_vertex = vertex_offset,
            .base_instance = 0};

        mesh_id_to_draw_command[e.id()] = draw_command;

        glNamedBufferSubData(meshes_vertex_buffer,
                             vertex_offset * sizeof(Vertex),
                             mesh.vertices.size() * sizeof(Vertex),
                             (const void*)mesh.vertices.data());

        glNamedBufferSubData(meshes_index_buffer,
                             index_offset * sizeof(unsigned int),
                             mesh.indices.size() * sizeof(unsigned int),
                             (const void*)mesh.indices.data());


        draw_commands_data[draw_commands_count++] = draw_command;
        vertex_offset += mesh.vertices.size();
        index_offset += mesh.indices.size();
      });

  world.system<Window>("WindowPreProcessing")
      .kind(flecs::OnLoad)
      .each([=](Window& window) {
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      });

  world.system<>("Rendering").kind(flecs::OnStore).run([](flecs::iter& it) {
    auto drawables = it.world().query<const Drawable>();

    auto shader_id =
        it.world().get_mut<LoadedShaders>()->shader_name_to_id["default"];

    glUseProgram(shader_id);

    auto projection = it.world().get<Projection>();

    auto fly_camera = it.world().entity<FlyCamera>();
    auto camera_position_cpmponent = fly_camera.get<Position>();
    auto camera_direction_component = fly_camera.get<Direction>();
    vec3 camera_position{camera_position_cpmponent->x,
                         camera_position_cpmponent->y,
                         camera_position_cpmponent->z};
    vec3 camera_up = fly_camera.get<FlyCamera>()->up;
    vec3 camera_target = fly_camera.get<FlyCamera>()->target;

    auto view =
        lookAt(camera_position, camera_position + camera_target, camera_up);
    int view_loc = glGetUniformLocation(shader_id, "view");
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, value_ptr(view));
    int projection_loc = glGetUniformLocation(shader_id, "projection");
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE,
                       value_ptr(projection->matirx));

    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, (const void*)0,
                                1, 0);
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