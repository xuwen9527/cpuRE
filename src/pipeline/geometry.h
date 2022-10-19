#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace cpuRE {
  struct Geometry {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<float> indices;
  };

  std::shared_ptr<Geometry> createTriangleGeometry();

  std::shared_ptr<Geometry> createIcosahedronGeometry();
}

#endif