#include "shader_program.h"

#include "glad.h"

namespace flux::shader {

ShaderProgram::ShaderProgram() { id_ = glCreateProgram(); }

void ShaderProgram::AttachShader(const Shader& shader) const {
  glAttachShader(id_, shader.GetId());
}

void ShaderProgram::Link() {
  glLinkProgram(id_);
  glGetProgramiv(id_, GL_LINK_STATUS, &link_status_);
}

void ShaderProgram::Delete() {
  glDeleteProgram(id_);
}

int ShaderProgram::GetLinkStatus() const { return link_status_; }

std::string ShaderProgram::GetInfoLog() const {
  std::string log;
  log.reserve(LOG_BUFF_SIZE);

  glGetShaderInfoLog(id_, LOG_BUFF_SIZE, NULL, log.data());
  return log;
}

void ShaderProgram::Use() const { glUseProgram(id_); }

}  // namespace flux::shader