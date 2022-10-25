#ifndef __BINRASTERIZAION_H__
#define __BINRASTERIZAION_H__

#include <glm/glm.hpp>
#include "context.h"
#include "buffer_io.h"
#include "bin_tile_space.h"
#include "bitmask.h"
#include "debug_draw.h"

namespace cpuRE {
  struct BinRasterizaion {
    using TileMask = BitMask<BinTileSpace::TileNumX, BinTileSpace::TileNumY>;

    static TileMask run(const glm::ivec2& binid, const glm::ivec4& bounds, const glm::mat3& m, Context& context) {
      TileMask tile_mask;
      auto tile_bounds = BinTileSpace::tileBounds(binid, bounds);
      tile_mask.set(tile_bounds.x, tile_bounds.y, tile_bounds.z, tile_bounds.w);

      auto bin_space = BinTileSpace::transformBin(binid, context.pixel_scale);

      for (auto e = 0; e < 3; ++e) {
        const auto& edge = m[e];
        auto invx = (edge.x != 0.f) ? 1.f / edge.x : 1.f / 0.000001f;

        int unset_right = edge.x < 0.f;
        int offset_row  = edge.y > 0.f;

        for (auto row = 0; row < BinTileSpace::TileNumY; ++row) {
          auto y = bin_space.start.y + (row + offset_row) * bin_space.stamp_size.y;
          auto x = (-y * edge.y - edge.z) * invx;

          auto col = bin_space.tileFromX(x);
          col += unset_right || (col < 0);

          tile_mask.markRow(row, col, unset_right);

          // if (context.debug_options.draw_bin_rasterization) {
          //   drawPixel(x, y, { 1.f, 1.f, 1.f, 1.f }, context);
          // }
        }
      }

      if (context.debug_options.draw_bin_rasterization) {
        if (context.debug_options.draw_bin_rasterization) {
          drawBin(bin_space.start.x, bin_space.start.y, context);
        }

        for (auto row = 0; row < BinTileSpace::TileNumY; ++row) {
          for (auto col = 0; col < BinTileSpace::TileNumX; ++col) {
            if (tile_mask.check(row, col)) {
              auto x = bin_space.start.x + col * bin_space.stamp_size.x;
              auto y = bin_space.start.y + row * bin_space.stamp_size.y;
              drawTile(x, y, context);
            }
          }
        }
      }

      return tile_mask;
    }
  };
}

#endif
