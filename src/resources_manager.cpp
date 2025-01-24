#include "resources_manager.h"

#include <unistd.h>

#include <filesystem>
#include <memory>
#include <iostream>
#include <cstring>
#include <cstdio>

namespace fs = std::filesystem;

using std::string;
using std::make_unique;

namespace flux::resources {

ResourcesManager::ResourcesManager() { cwd_ = fs::current_path(); }

string ResourcesManager::GetFileSource(const string &path) const {
  string file_path = cwd_;
  file_path.append("/").append(path);

  return get_file_source(file_path);
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