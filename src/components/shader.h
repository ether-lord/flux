#pragma once

#include <string>
#include <vector>

namespace flux::components {

struct ShaderInfo {
  unsigned int type;
  std::string name;
};

struct ShaderData {
  std::vector<ShaderInfo> data;
};

struct Shader {
  unsigned int id;
};

}  // namespace flux::components
