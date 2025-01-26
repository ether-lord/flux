#include "entity_manager.h"

namespace flux::entity {

EntityManager::EntityManager() { entity_counter_ = 0; }

EntityManager& EntityManager::get() {
  static EntityManager instance;
  return instance;
}

Id EntityManager::CreateEntity() { return entity_counter_++ % INT64_MAX; }

void EntityManager::DeleteEntity() {}

void EntityManager::AddDrawable2d(const systems::Drawable2d& drawable2d) {
  drawables2d_.push_back(drawable2d);
}

const std::vector<systems::Drawable2d>& EntityManager::GetDrawables2d() {
  return drawables2d_;
}

}  // namespace flux::entity
