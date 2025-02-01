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
  world.entity<FlyCamera>().add<InputTarget>();
  world.entity<FlyCamera>().set<Position>(Position{0.f, 0.f, 3.f});
  world.entity<FlyCamera>().set<Speed>({0.05f});
  world.entity<FlyCamera>().set<Direction>({0.f, 0.f, 0.f});

  world.system<FlyCamera, Direction, const InputTarget>("CameraMovementSystem")
      .kind(flecs::PreUpdate)
      .each([](flecs::entity e, FlyCamera& camera, Direction& direction,
               const InputTarget&) {
        auto key_inputs = e.world().get<Input>()->keyboard_events;

        for (auto [key, state] : key_inputs) {
          switch (state) {
            case KeyState::kPressed:
              if (key == KeyboardKey::kKeyW)
                direction.z += -1.f;
              else if (key == KeyboardKey::kKeyS)
                direction.z += 1.f;
              else if (key == KeyboardKey::kKeyD)
                direction.x += 1.f;
              else if (key == KeyboardKey::kKeyA)
                direction.x += -1.f;
              break;
            case KeyState::kReleased:
              if (key == KeyboardKey::kKeyW)
                direction.z -= -1.f;
              else if (key == KeyboardKey::kKeyS)
                direction.z -= 1.f;
              else if (key == KeyboardKey::kKeyD)
                direction.x -= 1.f;
              else if (key == KeyboardKey::kKeyA)
                direction.x -= -1.f;
              break;
            default:
              break;
          }
        }
      });
}

}  // namespace flux::modules