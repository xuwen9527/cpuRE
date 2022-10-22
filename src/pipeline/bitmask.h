#ifndef __BITMASK_H__
#define __BITMASK_H__

namespace cpuRE {
  template <unsigned int Rows, unsigned int Cols>
  struct BitMask {
    static constexpr uint64_t ROW_MASK_PATTERN = (1U << Rows) - 1U;

    uint64_t mask = 0xFFFFFFFFFFFFFFFFULL;

    void markRow(int row, int col, int revert) {
      uint64_t row_mask = revert ? 
                          (ROW_MASK_PATTERN >> col) :
                          (ROW_MASK_PATTERN >> col) ^ ROW_MASK_PATTERN;
      mask = mask & ~(row_mask << (row * Rows));
    }

    bool check(int row, int col) {
      auto bit = (1ULL << (Rows - 1 - col)) << (row * Cols);
      return bit & mask;
    }
  };
}

#endif
