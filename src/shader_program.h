#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "shader.h"

namespace flux::shader {

class ShaderProgram {
  using mat4 = glm::mat4;

 public:
  explicit ShaderProgram();

  void AttachShader(const Shader& shader) const;
  void Use() const;
  void Link();
  void Delete();
  int GetLinkStatus() const;

  void SetInt(const std::string& name, int value) const;
  void SetMat4(const std::string& name, const mat4& val) const;

  std::string GetInfoLog() const;

 private:
  int GetUniformLoc(const std::string& name) const;

  unsigned int id_;
  int link_status_;
};

}  // namespace flux::shader