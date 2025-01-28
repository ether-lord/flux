#include "resources_manager.h"

#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <memory>

namespace fs = std::filesystem;

using std::make_unique;
using std::string;

namespace flux::resources {

ResourcesManager::ResourcesManager() {
  cwd_ = fs::current_path();

  std::string settings_src = this->GetFileSource(settings_path_);
  settings_ = json::parse(settings_src);

  if (settings_.is_discarded()) {
    std::cerr << "failed to read settings.json from " << settings_src
              << std::endl;
    return;
  }

  shaders_location_ = settings_["res"]["shader"]["location"];
  textures_location_ = settings_["res"]["textures"]["location"];
  shaders_default_ext_ = settings_["res"]["shader"]["default_ext"];
}

ResourcesManager &ResourcesManager::get() {
  static ResourcesManager instance;
  return instance;
}

string ResourcesManager::GetAbsolutePath(const std::string &rel_path) const {
  string path = cwd_;

  if (rel_path.front() != '/') path.append("/");
  path.append(rel_path);

  return path;
}

std::string ResourcesManager::GetPathToTexture(const std::string &name) const {
  auto path = cwd_;
  return path.append(textures_location_).append(name);
}

string ResourcesManager::GetFileSource(const string &path) const {
  string file_path = GetAbsolutePath(path);
  return get_file_source(file_path);
}

std::string ResourcesManager::GetShaderSource(const std::string &name) const {
  string shader_path = shaders_location_ + name + shaders_default_ext_;
  return GetFileSource(shader_path);
}

std::string get_file_source(const std::string &path) {
  FILE *file = fopen(path.c_str(), "rb");

  if (!file) {
    perror("resources::read_file fopen");
    return string();
  }

  // getting the size of the file in bytes
  fseek(file, 0, SEEK_END);
  size_t file_size = ftell(file);
  fseek(file, 0, SEEK_SET);
  string buff(file_size, ' ');

  auto fread_res = fread(buff.data(), file_size, 1, file);
  fclose(file);

  if (!fread_res) {
    perror("resources::read_file fread");
    return string();
  }

  return buff;
}

}  // namespace flux::resources