#ifndef __BITMASK_H__
#define __BITMASK_H__

#include <cuda_runtime.h>
#include <stdint.h>
#include <glm/glm.hpp>

namespace cpuRE {
  struct BitMask {
  private:
    static constexpr int RowsBits = 3;
    static constexpr int ColsBits = 3;
  public:
    static constexpr int Rows = 1 << RowsBits;
    static constexpr int Cols = 1 << ColsBits;

    static constexpr uint64_t RowMask = (1U << Rows) - 1U;

    uint64_t mask = 0xFFFFFFFFFFFFFFFFULL;

    __device__
    void set(int lowerX, int lowerY, int upperX, int upperY) {
      upperX = Cols - 1 - upperX;
      lowerY = lowerY << RowsBits;
      upperY = (Rows - 1 - upperY) << RowsBits;

      uint64_t row_mask = ((RowMask << lowerX) & RowMask) >> lowerX;
      row_mask = row_mask >> upperX << upperX;
      mask = row_mask * 0x0101010101010101ULL;

      mask = mask >> lowerY << lowerY << upperY >> upperY;
    }

    __device__
    void markRow(int row, int col, int revert) {
      uint64_t row_mask = revert ?
                          (RowMask >> col) :
                          (RowMask >> col) ^ RowMask;
      mask = mask & ~(row_mask << (row * Rows));
    }

    __device__
    bool check(int row, int col) {
      auto bit = (1ULL << (Rows - 1 - col)) << (row << ColsBits);
      return bit & mask;
    }

    __device__
    int count();

    __device__
    glm::ivec2 coord(int index);
  };
}

#endif
