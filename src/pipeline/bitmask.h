#ifndef __BITMASK_H__
#define __BITMASK_H__

#include "bin_tile_space.h"

namespace cpuRE {
  struct BitMask {
    uint64_t mask = 0;

    void repeatRow(int begin, int end) {
      unsigned int row = ((0x1U << (end - begin)) - 1) << begin;
      mask = 0x0101010101010101ULL * row;
    }

    void andStride(int begin, int end) {
      mask = mask & (((0x1ULL << (end - begin)) - 1) << begin);
    }

    void xorRow(int row, int col) {
      constexpr uint64_t line_mask_pattern = (1U << 8) - 1;
      uint64_t line_mask = line_mask_pattern >> col;
      line_mask = line_mask << 8;
      mask = mask ^ line_mask;
    }
  };
}

#endif
