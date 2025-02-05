#include "light.h"

namespace flux {

Light::Light(flecs::world& world) {
  world.component<AmbientLight>();
  world.set<AmbientLight>({{1.f, 1.f, 1.f}, 0.1});
}

}  // namespace flux