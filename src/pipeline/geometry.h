#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace cpuRE {
  struct Geometry {
    std::vector<glm::vec3> vertices;
    std::vector<glm::ivec3> primitives;
  };

  std::shared_ptr<Geometry> createTriangleGeometry();
}

#endif