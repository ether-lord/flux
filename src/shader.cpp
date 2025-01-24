#include "shader.h"

namespace flux::shader {

Shader::Shader(ShaderType type, const std::string& source)
    : type_(type), source_(source) {
  id_ = glCreateShader(static_cast<int>(type_));

  auto source_c_str = source.c_str();
  glShaderSource(id_, 1, &source_c_str, NULL);
}

void Shader::Compile() {
  glCompileShader(id_);
  glGetShaderiv(id_, GL_COMPILE_STATUS, &compile_status_);
}

unsigned int Shader::GetId() const { return id_; }

int Shader::GetCompileStatus() const { return compile_status_; }

std::string Shader::GetShaderInfoLog() const {
  std::string log;
  log.reserve(log_buff_size_);

  glGetShaderInfoLog(id_, log_buff_size_, NULL, log.data());
  return log;
}

}  // namespace flux::shader