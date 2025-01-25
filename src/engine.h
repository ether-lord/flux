#pragma once

#include <unordered_map>

#include "entity.h"
#include "systems/window.h"

namespace flux::engine {

class Engine {
 public:
  Engine();

  void Start();

  bool is_running = false;

 private:
  void InitGlfw();
  void InitGlad();
  void CreateWindow();

  systems::Window window_system;
  std::unordered_map<entity::ID, components::Window> windows;
};

}  // namespace flux::engine
