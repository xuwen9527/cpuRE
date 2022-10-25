#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <glm/glm.hpp>

namespace cpuRE {
  struct Options {
    int draw_bin = 1;
    int draw_tile = 1;
    int draw_stamp = 1;
    int draw_edge = 0;
  };

  struct Context {
    glm::mat4 mvp;
    glm::ivec4 viewport;
    glm::vec4 pixel_scale;
    unsigned char* color_buffer;
    unsigned short* depth_buffer;
    Options debug_options;
  };
}

#endif
