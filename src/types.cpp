#include "types.h"

namespace flux::types {
Point2d::Point2d() : x(0), y(0) {}

Point2d::Point2d(int x, int y) : x(x), y(y) {}
}  // namespace flux::types