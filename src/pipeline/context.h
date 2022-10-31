#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <glm/glm.hpp>

namespace cpuRE {
  struct Options {
    bool draw_bin   = true;
    bool draw_tile  = true;
    bool draw_bounds= false;
    bool draw_stamp = true;
    bool draw_truth = false;
  };

  struct Status {
    bool cull_face = true;
  };

  struct Context {
    glm::mat4 mvp;
    glm::ivec4 viewport;
    glm::vec4 pixel_scale;
    unsigned char* color_buffer;
    unsigned short* depth_buffer;
    Options options;
    Status status;
  };
}

#endif
