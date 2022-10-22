#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <glm/glm.hpp>

namespace cpuRE {
  struct Context {
    glm::mat4 mvp;
    glm::ivec4 viewport;
    glm::vec4 pixel_scale;
    unsigned char* color_buffer;
    unsigned short* depth_buffer;
    struct {
      int draw_bin_rasterization = 1;
    } debug_options;
  };
}

#endif
