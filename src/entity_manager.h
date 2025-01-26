#pragma once

#include <vector>

#include "entity.h"
#include "systems/render.h"

namespace flux::entity {

class EntityManager {
 public:
  static EntityManager& get();

  Id CreateEntity();
  void DeleteEntity();

  void AddDrawable2d(const systems::Drawable2d& drawable2d);
  const std::vector<systems::Drawable2d>& GetDrawables2d();

 private:
  EntityManager();

  Id entity_counter_;

  std::vector<systems::Drawable2d> drawables2d_;
};

}  // namespace flux::entity
