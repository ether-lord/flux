#pragma once

#include <json.hpp>
#include <string>

namespace flux::resources {

using json = nlohmann::json;

std::string get_file_source(const std::string& path);

class ResourcesManager {
 public:
  ResourcesManager();

  std::string GetFileSource(const std::string& rel_path) const;
  std::string GetShaderSource(const std::string& name) const;

 private:
  const std::string settings_path_ = "/res/settings.json";

  std::string cwd_;
  std::string shaders_location_;
  std::string shaders_default_ext_;

  json settings_;
};

}  // namespace flux::resources
