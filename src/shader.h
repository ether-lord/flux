#pragma once

#include <string>

namespace flux::shader {

class Shader {
 public:
  Shader(const std::string& source);

  unsigned int id_;
};
}  // namespace flux::shader