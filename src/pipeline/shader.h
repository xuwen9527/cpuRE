#ifndef __SHADER_H__
#define __SHADER_H__

#include <glm/vec4.hpp>
#include "context.h"

namespace cpuRE {
  struct SimpleVertexShader {
    glm::vec4 operator() (const Context& context, const glm::vec3& vertex) {
      return context.mvp * glm::vec4(vertex, 1.f);
    }
  };

  struct SimpleFragmentShader {
    glm::vec4 operator() (const glm::vec3& u) {
      return glm::vec4(u, 1.f);
    }

    bool discarded() {
      return false;
    }
  };
}

#endif