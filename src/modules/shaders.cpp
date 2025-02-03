#include "modules/shaders.h"

#include <glad.h>

#include <iostream>

#include "components/graphics.h"
#include "components/shader.h"
#include "resources_manager.h"

using namespace std;
using namespace flux::components;
using namespace flux::resources;

namespace flux::modules {

Shaders::Shaders(flecs::world& world) {
  world.add<BasicShader>();

  ShaderInfo vertex_shader_info{GL_VERTEX_SHADER, "vertex"};
  ShaderInfo frag_shader_info{GL_FRAGMENT_SHADER, "fragment"};
  ShaderData shader_data{{vertex_shader_info, frag_shader_info}};

  auto basic_shader = world.get_mut<BasicShader>();
  basic_shader->id = glCreateProgram();

  for (const auto& shader : shader_data.data) {
    auto shader_source = ResourcesManager::get().GetShaderSource(shader.name);
    auto shader_source_cstr = shader_source.c_str();
    auto shader_id = glCreateShader(shader.type);
    glShaderSource(shader_id, 1, &shader_source_cstr, NULL);

    int compile_status = 0;
    glCompileShader(shader_id);
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status);

    if (!compile_status) {
      std::string log;
      log.reserve(LOG_BUFF_SIZE);

      glGetShaderInfoLog(shader_id, LOG_BUFF_SIZE, NULL, log.data());
      cout << log << endl;
    }

    glAttachShader(basic_shader->id, shader_id);
  }

  glLinkProgram(basic_shader->id);

  int link_status = 0;
  glGetProgramiv(basic_shader->id, GL_LINK_STATUS, &link_status);

  if (!link_status) {
    std::string log;
    log.reserve(LOG_BUFF_SIZE);

    glGetProgramInfoLog(basic_shader->id, LOG_BUFF_SIZE, NULL, log.data());
    cout << log << endl;
  }
}

}  // namespace flux::modules
