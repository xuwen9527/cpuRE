#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <glm/glm.hpp>
#include "framebuffer.h"

namespace cpuRE {
  struct Context {
    glm::mat4 mvp;
    glm::ivec4 viewport;
    glm::vec4 pixel_scale;
    unsigned char* color_buffer;
    unsigned short* depth_buffer;
  };
}

#endif
