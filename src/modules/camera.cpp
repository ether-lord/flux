#include "camera.h"

#include "components/graphics.h"
#include "components/input.h"

using namespace flux::components;

namespace flux::modules {

Camera::Camera(flecs::world& world) {
  FlyCamera camera;
  camera.yaw = -90.f;
  camera.pitch = 0.f;
  camera.speed = 2.5f;
  camera.zoom = 45.f;
  camera.sensitivity = 0.1f;

  world.set<FlyCamera>(camera);
  world.entity<FlyCamera>().add<InputHandler>();

  world.system<FlyCamera, const InputHandler>()
      .kind(flecs::PostLoad)
      .each([](flecs::entity e, FlyCamera& camera, const InputHandler& input) {
        
      });
}

}  // namespace flux::modules