#pragma once

#include <cstdint>

namespace flux::entity {
using Id = std::uint64_t;

struct Entity {
  Id id;
};

}  // namespace flux::entity
