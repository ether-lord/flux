#pragma once

#include <string>

#include "glad.h"

namespace flux::shader {

enum class ShaderType {
  VERTEX = GL_VERTEX_SHADER,
  FRAGMENT = GL_FRAGMENT_SHADER
};

class Shader {
 public:
  explicit Shader(ShaderType type, const std::string& source);

  void Compile();
  unsigned int GetId() const;
  int GetCompileStatus() const;

  std::string GetShaderInfoLog() const;

 private:
  const size_t log_buff_size_ = 1024;

  int compile_status_;
  unsigned int id_;
  std::string source_;

  ShaderType type_;
};
}  // namespace flux::shader