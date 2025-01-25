#include "system_manager.h"

using namespace glm;

namespace flux::system {

SystemManager::SystemManager() {
  InitGLFW();

  monitor_ = glfwGetPrimaryMonitor();
  video_mode_ = glfwGetVideoMode(monitor_);

  monitor_size_ = vec2(video_mode_->width, video_mode_->height);
  window_ = glfwCreateWindow(monitor_size_.x, monitor_size_.y, "FLUX", monitor_,
                             NULL);

  glfwMakeContextCurrent(window_);

  InitGLAD();
}

SystemManager::vec2 SystemManager::GetPrimaryMonitorSize() const { return monitor_size_; }

GLFWmonitor* SystemManager::GetPrimaryMonitor() const { return monitor_; }

GLFWwindow* SystemManager::GetWindow() const { return window_; }

void SystemManager::InitGLFW() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLFW_VERSION_HINT_MAJOR);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLFW_VERSION_HINT_MINOR);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void SystemManager::InitGLAD() {
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

}  // namespace flux::system