#pragma once

#include <string>
#include <json.hpp>

namespace flux::resources {

std::string get_file_source(const std::string& path);

class ResourcesManager {
 public:
  ResourcesManager();

  std::string GetFileSource(const std::string&) const;

 private:
  std::string cwd_;  
};
}  // namespace flux::resources
