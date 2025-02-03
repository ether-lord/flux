#include "modules/shaders.h"

#include <glad.h>

#include <iostream>

#include "resources_manager.h"

using namespace std;
using namespace flux::resources;

namespace flux {

Shaders::Shaders(flecs::world& world) {
  auto default_shader = world.entity("default");
  default_shader.set<ShaderData>({"default"});

  world.system<ShaderData>("Loader")
      .kind(flecs::OnLoad)
      .each([](flecs::entity e, ShaderData& shader_data) {
        Shader shader{glCreateProgram()};

        auto vertex_shader_source = ResourcesManager::get().GetShaderSource(
            shader_data.name, GL_VERTEX_SHADER);
        auto vertex_shader_source_data = vertex_shader_source.data();

        auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_shader_source_data, NULL);

        int compile_status = 0;
        glCompileShader(vertex_shader);
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_status);

        if (!compile_status) {
          char* log = new char[LOG_BUFF_SIZE];

          glGetShaderInfoLog(vertex_shader, LOG_BUFF_SIZE, NULL, log);
          cout << log << endl;

          delete[] log;
        }

        glAttachShader(shader.id, vertex_shader);

        auto fragment_shader_source = ResourcesManager::get().GetShaderSource(
            shader_data.name, GL_FRAGMENT_SHADER);
        auto fragment_shader_source_data = fragment_shader_source.data();

        auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_shader_source_data, NULL);

        glCompileShader(fragment_shader);
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_status);

        if (!compile_status) {
          char* log = new char[LOG_BUFF_SIZE];

          glGetShaderInfoLog(fragment_shader, LOG_BUFF_SIZE, NULL, log);
          cout << log << endl;

          delete[] log;
        }

        glAttachShader(shader.id, fragment_shader);

        glLinkProgram(shader.id);

        int link_status = 0;
        glGetProgramiv(shader.id, GL_LINK_STATUS, &link_status);

        if (!link_status) {
          char* log = new char[LOG_BUFF_SIZE];

          glGetProgramInfoLog(shader.id, LOG_BUFF_SIZE, NULL, log);
          cout << log << endl;

          delete[] log;
        }

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        e.set<Shader>(shader);
        e.remove<ShaderData>();
      });
}

}  // namespace flux