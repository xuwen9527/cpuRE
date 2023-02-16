#ifndef __RASTERIZATION_STAGE_H__
#define __RASTERIZATION_STAGE_H__

#include "triangle_buffer.cuh"
#include "context.cuh"
#include "viewport.cuh"
#include "buffer_io.cuh"
#include "bin_tile_space.cuh"
#include "bin_rasterizaion.cuh"
#include "tile_rasterizaion.cuh"
#include "shading.cuh"

namespace cpuRE {
  template <typename FragmentShader>
  struct RasterizationStage {
    __device__
    static void run(const TriangleBuffer& triangle_buffer, Context& context) {
      FragmentShader shader;

      glm::mat3 m;
      glm::vec3 uz;
      glm::ivec4 bounds;

      for (uint32_t i = 0; i < TriangleBuffer::NUM; ++i) {
        triangle_buffer.load(i, m, uz, bounds);
        for (auto x = context.viewport.x; x < context.viewport.z; ++x) {
          for (auto y = context.viewport.y; y < context.viewport.w; ++y) {
            auto p = clipcoordsFromRaster(x, y, context.pixel_scale);
            auto u = p * m;

            if (u.x >= 0.f && u.y >= 0.f && u.z >= 0.f) {
              auto z = glm::dot(uz, p);

              auto z_dest = BufferIO::readDepth(x, y, context.depth_buffer, context.viewport.z);
              if (z >= -1.f && z <= 1.f && z < z_dest) {
                u /= u.x + u.y + u.z;
                auto color = shader(u);
                BufferIO::writeColor(x, y, color, context.color_buffer, context.viewport.z);
                BufferIO::writeDepth(x, y, z,     context.depth_buffer, context.viewport.z);
              }
            }
          }
        }
      }
    }
  };

  template <typename FragmentShader>
  struct BinTileRasterizationStage {
    __device__
    static void run(const TriangleBuffer& triangle_buffer, Context& context) {
      // for (TriangleBuffer::size_type triid = 0; triid < triangle_buffer.size(); ++triid) {
      //   // const auto& triangle = triangle_buffer[triid];
      //   // const auto& m      = std::get<0>(triangle);
      //   // const auto& uz     = std::get<1>(triangle);
      //   // const auto& bounds = std::get<2>(triangle);

      //   glm::mat3 m;
      //   glm::vec3 uz;
      //   glm::ivec4 bounds;

      //   auto from_bin = BinTileSpace::bin(bounds.x, bounds.y);
      //   auto end_bin  = BinTileSpace::bin(bounds.z - 1, bounds.w - 1);
      //   auto num_bins = BinTileSpace::numBins(from_bin, end_bin);

      //   for (auto binid = 0; binid < num_bins; ++binid) {
      //     auto bin = BinTileSpace::binFromId(binid, from_bin, end_bin);
      //     auto tile_mask = BinRasterizaion::run(bin, bounds, m, context);

      //     int num_tile = tile_mask.count();
      //     for (int tileid = 0; tileid < num_tile; ++tileid) {
      //       auto tile = tile_mask.coord(tileid);
      //       auto pixel_mask = TileRasterizaion::run(bin, tile, bounds, m, context);

      //       if (context.options.draw_pixel) {
      //         int num_pixel = pixel_mask.count();
      //         for (int pixelid = 0; pixelid < num_pixel; ++pixelid) {
      //           auto fragment = pixel_mask.coord(pixelid);
      //           fragment.x += BinTileSpace::tileLeft(bin, tile);
      //           fragment.y += BinTileSpace::tileTop (bin, tile);

      //           Shading<FragmentShader>::run(fragment, m, uz, context);
      //         }
      //       }
      //     }
      //   }

      //   if (context.options.draw_bounds) {
      //     drawBounds(bounds, context);
      //   }

        // if (context.options.draw_truth) {
          // for (auto x = context.viewport.x; x < context.viewport.z; ++x) {
          //   for (auto y = context.viewport.y; y < context.viewport.w; ++y) {
          //     auto p = clipcoordsFromRaster(x, y, context.pixel_scale);
          //     auto u = p * m;

          //     if (u.x >= 0.f && u.y >= 0.f && u.z >= 0.f) {
          //       const auto& uz = std::get<1>(triangle);
          //       auto z = glm::dot(uz, p);
          //       if (z >= -1.f && z <= 1.f) {
          //         drawPixel(x, y, { 0.f, 1.f, 1.f, 0.5f }, context);
          //       }
          //     }
          //   }
          // }
        // }
      // }
    }
  };
}

#endif