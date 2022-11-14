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
  struct BitMask {
  private:
    static constexpr int RowsBits = 3;
    static constexpr int ColsBits = 3;
  public:
    static constexpr int Rows = 1 << RowsBits;
    static constexpr int Cols = 1 << ColsBits;

    static constexpr uint64_t RowMask = (1U << Rows) - 1U;

    uint64_t mask = 0xFFFFFFFFFFFFFFFFULL;

    void set(int lowerX, int lowerY, int upperX, int upperY) {
      upperX = Cols - 1 - upperX;
      lowerY = lowerY << RowsBits;
      upperY = (Rows - 1 - upperY) << RowsBits;

      uint64_t row_mask = ((RowMask << lowerX) & RowMask) >> lowerX;
      row_mask = row_mask >> upperX << upperX;
      mask = row_mask * 0x0101010101010101ULL;

      mask = mask >> lowerY << lowerY << upperY >> upperY;
    }

    void markRow(int row, int col, int revert) {
      uint64_t row_mask = revert ?
                          (RowMask >> col) :
                          (RowMask >> col) ^ RowMask;
      mask = mask & ~(row_mask << (row * Rows));
    }

    bool check(int row, int col) {
      auto bit = (1ULL << (Rows - 1 - col)) << (row << ColsBits);
      return bit & mask;
    }

    int count() {
      return __popcll(mask);
    }

    glm::ivec2 coord(int index) {
      index += 1;
      uint32_t p = 32;
      for (uint32_t offset = p / 2; offset > 0; offset /= 2)
        p = (__popcll(mask >> p) < index) ? (p - offset) : (p + offset);
      p = (__popcll(mask >> p) == index) ? p : p - 1;

      return { Cols - 1U - (p & (Cols - 1)), p >> ColsBits};
    }
  };
}

#endif
