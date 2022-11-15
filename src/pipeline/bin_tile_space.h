#ifndef __BINTILESPACE_H__
#define __BINTILESPACE_H__

#include <glm/glm.hpp>
#include "viewport.h"

namespace cpuRE {
  struct BinTileSpace {
  private:
    static constexpr int TileNumBits  = 3;
    static constexpr int TileSizeBits = 3;
    static constexpr int BinSizeBits  = TileNumBits + TileSizeBits;
  public:
    static constexpr int TileNum  = 1 << TileNumBits;
    static constexpr int TileSize = 1 << TileSizeBits;

    static constexpr int BinSize = TileNum * TileSize;

    static glm::ivec2 bin(int x, int y) {
      return { x >> BinSizeBits, y >> BinSizeBits };
    }

    static int binLeft(int i) {
      return i << BinSizeBits;
    }

    static int binTop(int j) {
      return j << BinSizeBits;
    }

    static int numBins(const glm::ivec2& from, const glm::ivec2& end) {
      return (end.x - from.x + 1) * (end.y - from.y + 1);
    }

    static int tileLeft(const glm::ivec2& bin, const glm::ivec2& tile) {
      return binLeft(bin.x) + (tile.x << TileSizeBits);
    }

    static int tileTop(const glm::ivec2& bin, const glm::ivec2& tile) {
      return binTop(bin.y) + (tile.y << TileSizeBits);
    }

    static glm::ivec2 binFromId(int binid, const glm::ivec2& from, const glm::ivec2& end) {
      auto width = end.x - from.x + 1;
      return { from.x + binid % width, from.y + binid / width };
    }

    static glm::ivec4 tileBounds(const glm::ivec2& bin, const glm::ivec4& bounds) {
      auto x = binLeft(bin.x);
      auto y = binTop(bin.y);

      return {
        glm::clamp((bounds.x - x) >> TileSizeBits, 0, TileNum - 1),
        glm::clamp((bounds.y - y) >> TileSizeBits, 0, TileNum - 1),
        glm::clamp((bounds.z - x - 1) >> TileSizeBits, 0, TileNum - 1),
        glm::clamp((bounds.w - y - 1) >> TileSizeBits, 0, TileNum - 1),
      };
    }

    static glm::ivec4 pixelBounds(const glm::ivec2& bin, const glm::ivec2& tile, const glm::ivec4& bounds) {
      auto tile_x = tileLeft(bin, tile);
      auto tile_y = tileTop (bin, tile);
      return {
        glm::clamp(bounds.x - tile_x, 0, TileSize - 1),
        glm::clamp(bounds.y - tile_y, 0, TileSize - 1),
        glm::clamp(bounds.z - tile_x - 1, 0, TileSize - 1),
        glm::clamp(bounds.w - tile_y - 1, 0, TileSize - 1),
      };
    }

    struct TransformedBin {
      glm::vec2 start;
      glm::vec2 tile_size;
      glm::vec2 inv_tile_size;

      TransformedBin(int left, int top, const glm::vec4& pixel_scale) :
        start(clipcoords2DFromRaster(left, top, pixel_scale)),
        tile_size(clipvecFromRaster(TileSize, TileSize, pixel_scale)),
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
        start(clipcoords2DFromRaster(left, top, pixel_scale)),
        fragment_size(clipvecFromRaster(1, 1, pixel_scale)),
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
      return TransformedBin(binLeft(bin.x), binTop(bin.y), pixel_scale);
    }

    static TransformedTile transformTile(const glm::ivec2& bin,
      const glm::ivec2& tile, const glm::vec4& pixel_scale) {
      return TransformedTile(tileLeft(bin, tile), tileTop(bin, tile), pixel_scale);
    }
  };
}

#endif
