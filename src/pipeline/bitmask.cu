#include "bitmask.cuh"

namespace cpuRE {
  __device__
  int BitMask::count() {
    return __popcll(mask);
  }

  __device__
  glm::ivec2 BitMask::coord(int index) {
    index += 1;
    uint32_t p = 32;
    for (uint32_t offset = p / 2; offset > 0; offset /= 2)
      p = (__popcll(mask >> p) < index) ? (p - offset) : (p + offset);
    p = (__popcll(mask >> p) == index) ? p : p - 1;

    return { Cols - 1U - (p & (Cols - 1)), p >> ColsBits};
  }
}