#pragma once

#include "components/position.h"
#include "components/vertices.h"

namespace flux::systems {

struct Drawable2d {
  components::Position* position;
  components::Polygon2d* polygon;
};

class Render {
 public:
  void Update();
  void RenderDrawable2d();
};

}  // namespace flux::systems