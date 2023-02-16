#ifndef __SHADING_H__
#define __SHADING_H__

#include <glm/glm.hpp>
#include "context.cuh"
#include "buffer_io.cuh"
#include "viewport.cuh"

namespace cpuRE {
  template <typename FragmentShader>
  struct Shading {
    __device__
    static void run(const glm::ivec2& fragment, const glm::mat3& m, const glm::vec3& uz, Context& context) {
      auto p = clipcoordsFromRaster(fragment.x, fragment.y, context.pixel_scale);
      auto z = (glm::dot(uz, p) + 1.f) / 2.f;
      if (z >= 0.f) {
        auto z_dest = BufferIO::readDepth(fragment.x, fragment.y, context.depth_buffer, context.viewport.z);
        if(z < z_dest) {
          auto u = p * m;
          u /= u.x + u.y + u.z;

          FragmentShader shader;
          auto color = shader(u);
          if (!shader.discarded()) {
            BufferIO::writeColor(fragment.x, fragment.y, color, context.color_buffer, context.viewport.z);
            BufferIO::writeDepth(fragment.x, fragment.y, z,     context.depth_buffer, context.viewport.z);
          }
        }
      }
    }
  };
}

#endif
