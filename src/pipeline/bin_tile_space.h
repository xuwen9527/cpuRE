#ifndef __BINTILESPACE_H__
#define __BINTILESPACE_H__

#include <glm/glm.hpp>
#include "viewport.h"

namespace cpuRE {
  template <int tile_num_x, int tile_num_y, int tile_width, int tile_height>
  struct PatternBinTileSpace {
  protected:
    static constexpr int bin_width  = tile_num_x * tile_width;
    static constexpr int bin_height = tile_num_y * tile_height;

  public:
    static constexpr int TileNumX   = tile_num_x;
    static constexpr int TileNumY   = tile_num_y;
    static constexpr int TileWidth  = tile_width;
    static constexpr int TileHeight = tile_height;

    static glm::ivec2 bin(int x, int y) {
      return { x / bin_width, y / bin_height };
    }

    static int left(int i) {
      return i * bin_width;
    }

    static int top(int j) {
      return j * bin_height;
    }

    static int right(int i) {
      return left(i) + bin_width;
    }

    static int bottom(int j) {
      return top(j) + bin_height;
    }

    static glm::ivec4 binBounds(const glm::ivec2& bin) {
      return { bin.x * bin_width, bin.y * bin_height, (bin.x + 1) * bin_width, (bin.y + 1) * bin_height };
    }

    static int numBins(const glm::ivec2& from, const glm::ivec2& end) {
      return (end.x - from.x + 1) * (end.y - from.y + 1);
    }

    static glm::ivec2 binid(int i, const glm::ivec2& from, const glm::ivec2& end) {
      auto width = end.x - from.x + 1;
      return { from.x + i % width, from.y + i / width };
    }

    static int insideTileX(const glm::ivec2& binid, int x) {
      return (x - left(binid.x)) / tile_width;
	  }

    static int insideTileY(const glm::ivec2& binid, int y) {
      return (y - top(binid.y)) / tile_height;
    }

    struct TransformedBin {
      glm::vec2 start;
      glm::vec2 tile_size;
      glm::vec2 inv_tile_size;
      uint64_t tile_mask = 0ULL;

      TransformedBin(int left, int top, const glm::vec4& pixel_scale) :
        start(RasterToClip::point(left, top, pixel_scale)),
			  tile_size(RasterToClip::vec(tile_width, tile_height, pixel_scale)),
			  inv_tile_size(1.0f / tile_size.x, 1.0f / tile_size.y) {
      }

   		int tileFromX(float x) {
        float diff = (x - start.x) * inv_tile_size.x;
        int h = 1.f + glm::clamp(diff, -1.f, static_cast<float>(tile_num_x));
        return h - 1;
      }

      void markTileRow(int tile_row, int tile_col) {
        constexpr uint64_t row_mask_pattern = (1U << tile_num_x) - 1;
        uint64_t row_mask = row_mask_pattern >> tile_col;
        tile_mask = tile_mask ^ (row_mask << (tile_row * tile_num_x));
      }

      // bool check(int tile_row, int tile_col) {
      //   1U << tile_col;
      //   //tile_row = ;
      // }
    };

	  static TransformedBin clipcoordsFromRaster(const glm::ivec2& bin, const glm::vec4& pixel_scale) {
		  return TransformedBin(left(bin.x), top(bin.y), pixel_scale);
	  }
  };

  using BinTileSpace = PatternBinTileSpace<8, 8, 8, 8>;
}

#endif
