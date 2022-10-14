#include <memory>
#include "geometry.h"

namespace cpuRE {
  std::shared_ptr<Geometry> createTriangleGeometry() {
    std::shared_ptr<Geometry> geometry = std::make_shared<Geometry>();
    geometry->vertices = {
      glm::vec3(-1.0f, -1.0f, 0.0f),
      glm::vec3( 1.0f, -1.0f, 0.0f),
      glm::vec3( 0.0f,  1.0f, 0.0f)
    };

    geometry->primitives = {
      glm::ivec3(0, 1, 2)
    };

    return geometry;
  }
}