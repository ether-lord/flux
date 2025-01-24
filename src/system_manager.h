#pragma once

#include "glad.h"
#include "glfw3.h"
#include "types.h"

namespace flux::system {

static const int GLFW_VERSION_HINT_MAJOR = 4;
static const int GLFW_VERSION_HINT_MINOR = 6;

class SystemManager {
  using Point2d = flux::types::Point2d;

 public:
  SystemManager();

  Point2d GetPrimaryMonitorSize() const;
  GLFWmonitor* GetPrimaryMonitor() const;
  GLFWwindow* GetWindow() const;

 private:
  void InitGLFW();
  void InitGLAD();

  GLFWmonitor* monitor_;
  GLFWwindow* window_;
  const GLFWvidmode* video_mode_;

  Point2d monitor_size_;
};

}  // namespace flux::system