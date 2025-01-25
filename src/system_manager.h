#pragma once

#include <glm/glm.hpp>

#include "glad.h"
#include "glfw3.h"
#include "types.h"

namespace flux::system {

static const int GLFW_VERSION_HINT_MAJOR = 4;
static const int GLFW_VERSION_HINT_MINOR = 6;

class SystemManager {
  using vec2 = glm::vec2;

 public:
  SystemManager();

  vec2 GetPrimaryMonitorSize() const;
  GLFWmonitor* GetPrimaryMonitor() const;
  GLFWwindow* GetWindow() const;

 private:
  void InitGLFW();
  void InitGLAD();

  GLFWmonitor* monitor_;
  GLFWwindow* window_;
  const GLFWvidmode* video_mode_;

  vec2 monitor_size_;
};

}  // namespace flux::system