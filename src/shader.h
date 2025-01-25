#pragma once

#include <string>

#include "glad.h"

namespace flux::shader {

const int LOG_BUFF_SIZE = 1024;

enum class ShaderType {
  VERTEX = GL_VERTEX_SHADER,
  FRAGMENT = GL_FRAGMENT_SHADER
};

class Shader {
 public:
  explicit Shader(ShaderType type, const std::string& source);
  ~Shader();

  void Compile();
  unsigned int GetId() const;
  int GetCompileStatus() const;

  std::string GetInfoLog() const;

 private:
  int compile_status_;
  unsigned int id_;
  std::string source_;

  ShaderType type_;
};
}  // namespace flux::shader