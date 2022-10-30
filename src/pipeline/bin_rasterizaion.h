#ifndef __BINRASTERIZAION_H__
#define __BINRASTERIZAION_H__

#include <glm/glm.hpp>
#include "context.h"
#include "bin_tile_space.h"
#include "bitmask.h"
#include "debug_draw.h"

namespace cpuRE {
  struct BinRasterizaion {
    using TileMask = BitMask<BinTileSpace::TileNumX, BinTileSpace::TileNumY>;

    static TileMask run(const glm::ivec2& bin, const glm::ivec4& bounds, const glm::mat3& m, Context& context) {
      TileMask tile_mask;
      auto tile_bounds = BinTileSpace::tileBounds(bin, bounds);
      tile_mask.set(tile_bounds.x, tile_bounds.y, tile_bounds.z, tile_bounds.w);

      auto bin_space = BinTileSpace::transformBin(bin, context.pixel_scale);
      constexpr float one_row = (BinTileSpace::StampNumY - 1.f) / BinTileSpace::StampNumY;

      for (auto e = 0; e < 3; ++e) {
        const auto& edge = m[e];
        auto invx = (edge.x != 0.f) ? 1.f / edge.x : 1.f / 0.000001f;

        int unset_right  = edge.x < 0.f;
        float row_offset = edge.y > 0.f ? one_row : 0.f;

        for (auto row = 0; row < BinTileSpace::TileNumY; ++row) {
          auto y = bin_space.start.y + (row + row_offset) * bin_space.stamp_size.y;
          auto x = (-y * edge.y - edge.z) * invx;

          auto col = bin_space.tileFromX(x);
          col += unset_right || (col < 0);

          tile_mask.markRow(row, col, unset_right);
        }
      }

      if (context.options.draw_bin) {
        drawBin(bin_space.start.x, bin_space.start.y, context);
      }

      return tile_mask;
    }
  };
}

#endif
