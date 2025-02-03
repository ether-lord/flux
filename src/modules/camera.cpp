#include "camera.h"

#include <iostream>

#include "modules/input.h"
#include "modules/movement.h"

using namespace glm;

namespace flux {

Camera::Camera(flecs::world& world) {
  FlyCamera camera;
  camera.yaw = -90.f;
  camera.pitch = 0.f;
  camera.zoom = 45.f;
  camera.sensitivity = 0.1f;

  world.set<FlyCamera>(camera);
  world.entity<FlyCamera>().add<InputTarget>();
  world.entity<FlyCamera>().set<Position>(Position{0.f, 0.f, 3.f});
  world.entity<FlyCamera>().set<Direction>({0.f, 0.f, 0.f});
  world.entity<FlyCamera>().set<Speed>({0.05f});

  world.system<FlyCamera, Direction, const InputTarget>("CameraMovementSystem")
      .kind(flecs::PreUpdate)
      .each([](flecs::entity e, FlyCamera& camera, Direction& direction,
               const InputTarget&) {
        auto keyboard_state = e.world().get<InputData>()->keyboard_state;

        vec3 direction_vector{0.f};

        for (auto [key, state] : keyboard_state) {
          switch (state) {
            case KeyState::kPressed:
              if (key == KeyboardKey::kKeyW)
                direction_vector += camera.target;
              else if (key == KeyboardKey::kKeyS)
                direction_vector -= camera.target;
              else if (key == KeyboardKey::kKeyD)
                direction_vector += camera.right;
              else if (key == KeyboardKey::kKeyA)
                direction_vector -= camera.right;
            default:
              break;
          }
        }

        if (length(direction_vector) > 0.f) normalize(direction_vector);

        direction.x = direction_vector.x;
        direction.y = direction_vector.y;
        direction.z = direction_vector.z;

        auto mouse_offset = e.world().get<InputData>()->mouse_offset;

        camera.yaw += mouse_offset.x;
        camera.pitch += mouse_offset.y;

        if (camera.pitch > 89.9f) camera.pitch = 89.9f;
        if (camera.pitch < -89.9f) camera.pitch = -89.9f;

        vec3 target;
        target.x = cos(radians(camera.yaw)) * cos(radians(camera.pitch));
        target.y = sin(radians(camera.pitch));
        target.z = sin(radians(camera.yaw)) * cos(radians(camera.pitch));
        camera.target = normalize(target);

        camera.right = normalize(cross(camera.target, camera.up));
      });
}

}  // namespace flux