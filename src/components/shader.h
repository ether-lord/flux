#pragma once

#include <string>
#include <vector>

namespace flux::components {

struct BasicShader {
  unsigned int id;
};

struct ShaderInfo {
  unsigned int type;
  std::string name;
};

struct ShaderData {
  std::vector<ShaderInfo> data;
};

}  // namespace flux::components
