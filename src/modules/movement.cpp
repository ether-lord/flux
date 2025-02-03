#include "movement.h"

namespace flux {

Movement::Movement(flecs::world& world) {
  world.component<Direction>();
  world.component<Position>();
  world.component<Speed>();

  world.system<Position, const Direction, const Speed>("Movement module")
      .kind(flecs::PreUpdate)
      .each([](Position& position, const Direction& direction,
               const Speed& speed) {
        position.x += direction.x * speed.magnitude;
        position.y += direction.y * speed.magnitude;
        position.z += direction.z * speed.magnitude;
      });
}

}  // namespace flux
