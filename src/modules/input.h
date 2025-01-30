#pragma once

#include <flecs.h>

namespace flux::modules {

struct InputSystem {
  InputSystem(flecs::world& world);
};

} // namespace flux::modules
