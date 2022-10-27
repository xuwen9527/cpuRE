#ifndef __BINTILESPACE_H__
#define __BINTILESPACE_H__

#include <glm/glm.hpp>
#include "viewport.h"

namespace cpuRE {
  template <int tile_num_x, int tile_num_y, int stamp_num_x, int stamp_num_y>
  struct BinTileSpacePattern {
  public:
    static constexpr int BIN_WIDTH  = tile_num_x * stamp_num_x;
    static constexpr int BIN_HEIGHT = tile_num_y * stamp_num_y;

    static constexpr int TileNumX  = tile_num_x;
    static constexpr int TileNumY  = tile_num_y;
    static constexpr int StampNumX = stamp_num_x;
    static constexpr int StampNumY = stamp_num_y;

    static glm::ivec2 bin(int x, int y) {
      return { x / BIN_WIDTH, y / BIN_HEIGHT };
    }

    static int left(int i) {
      return i * BIN_WIDTH;
    }

    static int top(int j) {
      return j * BIN_HEIGHT;
    }

    static int numBins(const glm::ivec2& from, const glm::ivec2& end) {
      return (end.x - from.x + 1) * (end.y - from.y + 1);
    }

    static glm::ivec2 tile(const glm::ivec2& bin, const glm::ivec2& tile) {
      return { left(bin.x) + tile.x * stamp_num_x, top(bin.y) + tile.y * stamp_num_y };
    }

    static glm::ivec2 binFromId(int binid, const glm::ivec2& from, const glm::ivec2& end) {
      auto width = end.x - from.x + 1;
      return { from.x + binid % width, from.y + binid / width };
    }

    static glm::ivec4 tileBounds(const glm::ivec2& bin, const glm::ivec4& bounds) {
      auto x = left(bin.x);
      auto y = top(bin.y);

      return {
        glm::clamp((bounds.x - x) / stamp_num_x, 0, tile_num_x - 1),
        glm::clamp((bounds.y - y) / stamp_num_y, 0, tile_num_y - 1),
        glm::clamp((bounds.z - x) / stamp_num_x, 0, tile_num_x - 1),
        glm::clamp((bounds.w - y) / stamp_num_y, 0, tile_num_y - 1),
      };
    }

    struct TransformedBin {
      glm::vec2 start;
      glm::vec2 stamp_size;
      glm::vec2 inv_stamp_size;

      TransformedBin(int left, int top, const glm::vec4& pixel_scale) :
        start(RasterToClip::point(left, top, pixel_scale)),
        stamp_size(RasterToClip::vec(stamp_num_x, stamp_num_y, pixel_scale)),
        inv_stamp_size(1.0f / stamp_size.x, 1.0f / stamp_size.y) {
      }

      int tileFromX(float x) {
        float diff = (x - start.x) * inv_stamp_size.x;
        int h = 1.f + glm::clamp(diff, -1.f, static_cast<float>(tile_num_x));
        return h - 1;
      }
    };

    struct TransformedTile {
      glm::vec2 start;
      glm::vec2 fragment_size;
      glm::vec2 inv_fragment_size;

      TransformedTile(int left, int top, const glm::vec4& pixel_scale) :
        start(RasterToClip::point(left, top, pixel_scale)),
        fragment_size(RasterToClip::vec(1.f, 1.f, pixel_scale)),
        inv_fragment_size(1.0f / fragment_size.x, 1.0f / fragment_size.y) {
      }

      int stampFromX(float x, float offset) {
        float diff = (x - start.x) * inv_fragment_size.x + offset;
        int h = 1.f + glm::clamp(diff, -1.f, static_cast<float>(stamp_num_x));
        return h - 1;
      }
    };

    static TransformedBin transformBin(const glm::ivec2& bin,
      const glm::vec4& pixel_scale) {
      return TransformedBin(left(bin.x), top(bin.y), pixel_scale);
    }

    static TransformedTile transformTile(const glm::ivec2& bin,
      const glm::ivec2& tile, const glm::vec4& pixel_scale) {
      return TransformedTile(
        left(bin.x) + tile.x * stamp_num_x,
        top(bin.y)  + tile.y * stamp_num_y,
        pixel_scale);
    }
  };

  using BinTileSpace = BinTileSpacePattern<8, 8, 8, 8>;
}

#endif
