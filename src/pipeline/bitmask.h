#ifndef __BITMASK_H__
#define __BITMASK_H__

namespace {
  int __popcll(uint64_t x) {
    int c;
    for (c = 0; x; ++c) {
      x &= x - 1ULL;
    }
    return c;
  }
}

namespace cpuRE {
  template <unsigned int Rows, unsigned int Cols>
  struct BitMask {
    static constexpr uint64_t ROW_MASK_PATTERN = (1U << Rows) - 1U;

    uint64_t mask = 0xFFFFFFFFFFFFFFFFULL;

    void set(int lowerX, int upperX, int lowerY, int upperY) {
      upperX = Cols - 1 - upperX;
      lowerY = lowerY * Rows;
      upperY = (Rows - 1 - upperY) * Rows;

      uint64_t row_mask = ((ROW_MASK_PATTERN << lowerX) & ROW_MASK_PATTERN) >> lowerX;
      row_mask = row_mask >> upperX << upperX;
      mask = row_mask * 0x0101010101010101ULL;

      mask = mask >> lowerY << lowerY << upperY >> upperY;
    }

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

    int count() {
      return __popcll(mask);
    }

    glm::ivec2 bitid(int index) {
      // find the nth set bit
      index += 1;
      unsigned int p = 32;
      for (unsigned int offset = p / 2; offset > 0; offset /= 2)
        p = (__popcll(mask >> p) < index) ? (p - offset) : (p + offset);
      p = (__popcll(mask >> p) == index) ? p : p - 1;

      return { Cols - 1U - p % Cols, p / Cols};
    }
  };
}

#endif
