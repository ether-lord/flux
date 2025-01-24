#pragma once

#include <vector>

#include "shader.h"

namespace flux::shader {

class ShaderProgram {
 public:
  explicit ShaderProgram();

  void AttachShader(const Shader& shader) const;
  void Use() const;
  void Link();
  void Delete();
  int GetLinkStatus() const;
  
  std::string GetInfoLog() const;

 private:
  unsigned int id_;
  int link_status_;
};

}  // namespace flux::shader