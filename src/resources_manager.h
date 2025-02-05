#pragma once

#include <json.hpp>
#include <string>

namespace flux::resources {

using json = nlohmann::json;

std::string get_file_source(const std::string& path);

class ResourcesManager {
 public:
  static ResourcesManager& get();

  std::string GetAbsolutePath(const std::string& rel_path) const;
  std::string GetPathToTexture(const std::string& name) const;
  std::string GetFileSource(const std::string& rel_path) const;
  std::string GetShaderSource(const std::string& name) const;
  std::string GetShaderSource(const std::string& name, unsigned int type) const;

 private:
  ResourcesManager();

  const std::string settings_path_ = "/res/settings.json";

  std::string cwd_;
  std::string shaders_location_;
  std::string textures_location_;
  std::string shaders_default_ext_;
  std::string vertex_shader_name_;
  std::string fragment_shader_name_;

  json settings_;
};

}  // namespace flux::resources
