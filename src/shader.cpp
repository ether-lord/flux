#include "shader.h"

namespace flux::shader {

Shader::Shader(ShaderType type, const std::string& source)
    : type_(type), source_(source) {
  id_ = glCreateShader(static_cast<int>(type_));

  auto source_c_str = source.c_str();
  glShaderSource(id_, 1, &source_c_str, NULL);
}

Shader::~Shader() { glDeleteShader(id_); }

void Shader::Compile() {
  glCompileShader(id_);
  glGetShaderiv(id_, GL_COMPILE_STATUS, &compile_status_);
}

unsigned int Shader::GetId() const { return id_; }

int Shader::GetCompileStatus() const { return compile_status_; }

std::string Shader::GetInfoLog() const {
  std::string log;
  log.reserve(LOG_BUFF_SIZE);

  glGetShaderInfoLog(id_, LOG_BUFF_SIZE, NULL, log.data());
  return log;
}

}  // namespace flux::shader