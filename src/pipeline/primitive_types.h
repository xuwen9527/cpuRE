#ifndef __PRIMITIVE_TYPE__
#define __PRIMITIVE_TYPE__

#include <glm/glm.hpp>

namespace cpuRE {
  struct TrianglesPrimitiveType {
    static constexpr unsigned int primitives(unsigned int indices) {
      return indices / 3U;
    }

    static glm::ivec3 vertices(unsigned int triangle) {
      auto v0 = 3U * triangle;
      return { v0, v0 + 1, v0 + 2 };
    }
  };
}

#endif
