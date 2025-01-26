#include <iostream>

#include "engine.h"
#include "entity_manager.h"
#include "render.h"

using namespace flux;
using namespace flux::engine;
using namespace flux::systems;
using namespace flux::components;
using namespace flux::entity;

int main() {
  Engine engine = Engine();

  Position* pos = new Position;
  Polygon2d* polygon = new Polygon2d;

  polygon->vertices = {{0.f, 0.5f, 0.f},
                       {0.5f, 0.f, 0.f},
                       {0.f, -0.5f, 0.f},
                       {-0.5f, 0.f, 0.f}};

  polygon->colors = polygon->vertices;
  polygon->indices = {0, 1, 2, 0, 2, 3};

  Drawable2d drawable = {pos, polygon};

  EntityManager::get().AddDrawable2d(drawable);

  engine.Start();

  delete pos;
  delete polygon;

  return 0;
}