#ifndef __RASTERIZATION_STAGE_H__
#define __RASTERIZATION_STAGE_H__

#include "triangle_buffer.h"
#include "context.h"
#include "viewport.h"
#include "buffer_io.h"
#include "bin_tile_space.h"
#include "bin_rasterizaion.h"
#include "tile_rasterizaion.h"

namespace cpuRE {
  template <typename FragmentShader>
  struct RasterizationStage {
    static void run(const TriangleBuffer& triangle_buffer, Context& context) {
      FragmentShader shader;

      for (auto x = context.viewport.x; x < context.viewport.z; ++x) {
        for (auto y = context.viewport.y; y < context.viewport.w; ++y) {
          auto p = clipcoordsFromRaster(x, y, context.pixel_scale);

          for (const auto& triangle : triangle_buffer) {
            const auto& m = std::get<0>(triangle);
            auto u = p * m;

            if (u.x >= 0.f && u.y >= 0.f && u.z >= 0.f) {
              const auto& uz = std::get<1>(triangle);
              auto z = glm::dot(uz, p);

              if (z >= -1.f && z <= 1.f) {
                u /= u.x + u.y + u.z;
                auto color = shader(u);
                BufferIO::writeColor(x, y, color, context.color_buffer, context.viewport.z);
                BufferIO::writeDepth(x, y, z, context.depth_buffer, context.viewport.z);
              }
            }
          }
        }
      }
    }
  };

  template <typename FragmentShader>
  struct BinTileRasterizationStage {
    static void run(const TriangleBuffer& triangle_buffer, Context& context) {
      for (TriangleBuffer::size_type i = 0; i < triangle_buffer.size(); ++i) {
        const auto& triangle = triangle_buffer[i];
        const auto& bounds = std::get<2>(triangle);
        const auto& m      = std::get<0>(triangle);

        auto from_bin = BinTileSpace::bin(bounds.x, bounds.y);
        auto end_bin  = BinTileSpace::bin(bounds.z, bounds.w);
        auto num_bins = BinTileSpace::numBins(from_bin, end_bin);

        for (auto i = 0; i < num_bins; ++i) {
          auto binid = BinTileSpace::binid(i, from_bin, end_bin);
          auto tile_mask = BinRasterizaion::run(binid, bounds, m, context);

          int count = tile_mask.count();
          for (int i = 0; i < count; ++i) {
             auto tileid = tile_mask.bitid(i);
             TileRasterizaion::run(binid, tileid, bounds, m, context);
          }
        }
      }
    }
  };
}

#endif