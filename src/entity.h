#pragma once

#include <cstdint>

namespace flux::entity {

using ID = std::uint64_t;

static ID last_id = 0;

inline ID CreateEntityId() {
  return last_id++ % INT64_MAX;
}

}  // namespace flux::entity
