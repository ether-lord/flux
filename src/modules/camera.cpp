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

  world.system<FlyCamera, const Input>()
      .kind(flecs::PostLoad)
      .each([](flecs::entity e, FlyCamera& camera, const Input& input) {
        if (input.key == KeyboardKey::kKeyW && input.state == KeyState::kPressed)
          camera.position.z -= camera.speed * e.world().delta_time();

        e.remove<Input>();
      });
}

}  // namespace flux::modules