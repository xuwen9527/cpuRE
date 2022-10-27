#ifndef __STAMP_SHADING_H__
#define __STAMP_SHADING_H__

#include <glm/glm.hpp>
#include "context.h"
#include "buffer_io.h"
#include "viewport.h"

namespace cpuRE {
  template <typename FragmentShader>
  struct StampShading {
    static void run(const glm::ivec2& fragment, const glm::mat3& m, const glm::vec3& uz, Context& context) {
      if (context.debug_options.draw_stamp) {
        return;
      }
      auto z_dest = BufferIO::readDepth(fragment.x, fragment.y, context.depth_buffer, context.viewport.z);
      auto p = clipcoordsFromRaster(fragment.x, fragment.y, context.pixel_scale);

      auto z = glm::dot(uz, p);
      if (z >= -1.f && z < z_dest) {
        auto u = p * m;
        u /= u.x + u.y + u.z;

        FragmentShader shader;
        auto color = shader(u);
        if (!shader.discarded()) {
          BufferIO::writeColor(fragment.x, fragment.y, color, context.color_buffer, context.viewport.z);
          BufferIO::writeDepth(fragment.x, fragment.y, z, context.depth_buffer, context.viewport.z);
        }
      }
    }
  };
}

#endif
