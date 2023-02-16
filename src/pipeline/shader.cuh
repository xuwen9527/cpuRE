#ifndef __SHADER_H__
#define __SHADER_H__

#include <glm/vec4.hpp>
#include "context.cuh"

namespace cpuRE {
  struct SimpleVertexShader {
    __device__
    glm::vec4 operator() (const Context& context, const glm::vec3& vertex) {
      return context.mvp * glm::vec4(vertex, 1.f);
    }
  };

  struct SimpleFragmentShader {
    __device__
    glm::vec4 operator() (const glm::vec3& u) {
      return glm::vec4(u, 1.f);
    }

    __device__
    bool discarded() {
      return false;
    }
  };
}

#endif