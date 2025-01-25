#include "shader_program.h"

#include <glm/gtc/type_ptr.hpp>

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

void ShaderProgram::Delete() { glDeleteProgram(id_); }

int ShaderProgram::GetLinkStatus() const { return link_status_; }

void ShaderProgram::SetInt(const std::string& name, int value) const {
  auto uniform_location = GetUniformLoc(name);
  glUniform1i(uniform_location, value);
}

void ShaderProgram::SetMat4(const std::string& name, const mat4& val) const {
  auto uniform_location = GetUniformLoc(name);
  glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(val));
}

std::string ShaderProgram::GetInfoLog() const {
  std::string log;
  log.reserve(LOG_BUFF_SIZE);

  glGetShaderInfoLog(id_, LOG_BUFF_SIZE, NULL, log.data());
  return log;
}

int ShaderProgram::GetUniformLoc(const std::string& name) const {
  return glGetUniformLocation(id_, name.c_str());
}

void ShaderProgram::Use() const { glUseProgram(id_); }

}  // namespace flux::shader