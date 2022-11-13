#ifndef __BINTILESPACE_H__
#define __BINTILESPACE_H__

#include <glm/glm.hpp>
#include "viewport.h"

namespace cpuRE {
  struct BinTileSpace {
  public:
    static constexpr int TileNum  = 8;
    static constexpr int TileSize = 8;

    static constexpr int BinSize = TileNum * TileSize;

    static glm::ivec2 bin(int x, int y) {
      return { x / BinSize, y / BinSize };
    }

    static int left(int i) {
      return i * BinSize;
    }

    static int top(int j) {
      return j * BinSize;
    }

    static int numBins(const glm::ivec2& from, const glm::ivec2& end) {
      return (end.x - from.x + 1) * (end.y - from.y + 1);
    }

    static glm::ivec2 tile(const glm::ivec2& bin, const glm::ivec2& tile) {
      return { left(bin.x) + tile.x * TileSize, top(bin.y) + tile.y * TileSize };
    }

    static glm::ivec2 binFromId(int binid, const glm::ivec2& from, const glm::ivec2& end) {
      auto width = end.x - from.x + 1;
      return { from.x + binid % width, from.y + binid / width };
    }

    static glm::ivec4 tileBounds(const glm::ivec2& bin, const glm::ivec4& bounds) {
      auto x = left(bin.x);
      auto y = top(bin.y);

      return {
        glm::clamp((bounds.x - x) / TileSize, 0, TileNum - 1),
        glm::clamp((bounds.y - y) / TileSize, 0, TileNum - 1),
        glm::clamp((bounds.z - x - 1) / TileSize, 0, TileNum - 1),
        glm::clamp((bounds.w - y - 1) / TileSize, 0, TileNum - 1),
      };
    }

    static glm::ivec4 pixelBounds(const glm::ivec2& bin, const glm::ivec2& tile, const glm::ivec4& bounds) {
      auto x = left(bin.x) + tile.x * TileSize;
      auto y = top(bin.y)  + tile.y * TileSize;

      return {
        glm::clamp(bounds.x - x, 0, TileSize - 1),
        glm::clamp(bounds.y - y, 0, TileSize - 1),
        glm::clamp(bounds.z - x - 1, 0, TileSize - 1),
        glm::clamp(bounds.w - y - 1, 0, TileSize - 1),
      };
    }

    struct TransformedBin {
      glm::vec2 start;
      glm::vec2 tile_size;
      glm::vec2 inv_tile_size;

      TransformedBin(int left, int top, const glm::vec4& pixel_scale) :
        start(RasterToClip::point(left, top, pixel_scale)),
        tile_size(RasterToClip::vec(TileSize, TileSize, pixel_scale)),
        inv_tile_size(1.0f / tile_size.x, 1.0f / tile_size.y) {
      }

      int tileFromX(float x) {
        float diff = (x - start.x) * inv_tile_size.x;
        int h = 1.f + glm::clamp(diff, -1.f, static_cast<float>(TileNum));
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

      int pixelFromX(float x, float offset) {
        float diff = (x - start.x) * inv_fragment_size.x + offset;
        int h = 1.f + glm::clamp(diff, -1.f, static_cast<float>(TileSize));
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
        left(bin.x) + tile.x * TileSize,
        top(bin.y)  + tile.y * TileSize,
        pixel_scale);
    }
  };
}

#endif
