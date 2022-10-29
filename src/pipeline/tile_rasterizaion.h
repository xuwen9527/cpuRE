#ifndef __TILERASTERIZAION_H__
#define __TILERASTERIZAION_H__

#include <glm/glm.hpp>
#include "context.h"
#include "bin_tile_space.h"
#include "bitmask.h"
#include "debug_draw.h"
#include "shader.h"

namespace cpuRE {
  struct TileRasterizaion {
    using StampMask = BitMask<BinTileSpace::StampNumX, BinTileSpace::StampNumY>;

    static StampMask run(const glm::ivec2& bin, const glm::ivec2& tile,
                         const glm::ivec4& bounds, const glm::mat3& m, Context& context) {
      StampMask stamp_mask;
      auto stamp_bounds = BinTileSpace::stampBounds(bin, tile, bounds);
      stamp_mask.set(stamp_bounds.x, stamp_bounds.y, stamp_bounds.z, stamp_bounds.w);

      auto tile_space = BinTileSpace::transformTile(bin, tile, context.pixel_scale);

      for (auto e = 0; e < 3; ++e) {
        const auto& edge = m[e];
        auto invx = (edge.x != 0.f) ? 1.f / edge.x : 1.f / 0.000001f;

        int unset_right = edge.x < 0.f ? 1 : 0;
        float col_offset = edge.x < 0.f ? 0.f : 1.f ;

        for (auto row = 0; row < BinTileSpace::StampNumY; ++row) {
          auto y = tile_space.start.y + row * tile_space.fragment_size.y;
          auto x = (-y * edge.y - edge.z) * invx;

          auto col = tile_space.stampFromX(x, col_offset);
          col += unset_right || (col < 0);

          stamp_mask.markRow(row, col, unset_right);
        }
      }

      if (context.debug_options.draw_tile) {
        drawTile(tile_space.start.x, tile_space.start.y, context);
      }

      return stamp_mask;
    }
  };
}

#endif
