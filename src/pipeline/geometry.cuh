#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <glm/glm.hpp>

namespace cpuRE {
  struct Geometry {
    glm::vec3* vertices;
    glm::vec3* normals = nullptr;
    std::uint32_t* indices = nullptr;
    std::size_t indices_num = 0U;
  };
}

#endif