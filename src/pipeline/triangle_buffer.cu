#ifndef __TRIANGLE_BUFFER_H__
#define __TRIANGLE_BUFFER_H__

#include <glm/fwd.hpp>

namespace cpuRE {
  __device__
  void TriangleBuffer::allocate() {
    return atomicInc(&next_, NUM - 1);
  }
}

#endif