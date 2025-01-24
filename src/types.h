#pragma once

namespace flux::types {

struct Point2d {
 public:
  Point2d();
  Point2d(int x, int y);

  int x;
  int y;
};

}  // namespace flux::types