#include "camera.h"

#include "components/graphics.h"
#include "components/input.h"
#include "components/movement.h"

using namespace flux::components;

namespace flux::modules {

Camera::Camera(flecs::world& world) {
  FlyCamera camera;
  camera.yaw = -90.f;
  camera.pitch = 0.f;
  camera.zoom = 45.f;
  camera.sensitivity = 0.1f;

  world.set<FlyCamera>(camera);
  world.entity<FlyCamera>().add<InputHandler>();
  world.entity<FlyCamera>().set<Position>(Position{0.f, 0.f, 3.f});
  world.entity<FlyCamera>().set<Speed>({0.05f});
  world.entity<FlyCamera>().set<Direction>({0.f, 0.f, 0.f});
}

}  // namespace flux::modules