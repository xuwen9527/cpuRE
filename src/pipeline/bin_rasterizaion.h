#ifndef __BINRASTERIZAION_H__
#define __BINRASTERIZAION_H__

#include <glm/glm.hpp>
#include "context.h"
#include "buffer_io.h"
#include "bin_tile_space.h"
#include "bitmask.h"

#define DRAW_BIN_TILE

namespace cpuRE {
  void drawBin(float binx, float biny, const Context& context) {
    glm::vec4 color((binx + 1.f) * 0.5f, (binx + 1.f) * 0.5f, 0.f, 1.f);
    auto pixel = rastercoordsFromClip(binx, biny, context.viewport);
    for (int i = 0; i < 64; ++i) {
      BufferIO::writeColor(pixel.x, pixel.y + i, color, context.color_buffer, context.viewport.z);
    }
    for (int i = 0; i < 64; ++i) {
      BufferIO::writeColor(pixel.x + 63, pixel.y + i, color, context.color_buffer, context.viewport.z);
    }
    for (int i = 0; i < 64; ++i) {
      BufferIO::writeColor(pixel.x + i, pixel.y, color, context.color_buffer, context.viewport.z);
    }
    for (int i = 0; i < 64; ++i) {
      BufferIO::writeColor(pixel.x + i, pixel.y + 63, color, context.color_buffer, context.viewport.z);
    }
  }

  void drawTile(float tilex, float tiley, const Context& context) {
    glm::vec4 color((tilex + 1.f) * 0.5f, (tiley + 1.f) * 0.5f, 0.f, 1.f);
    auto pixel = rastercoordsFromClip(tilex, tiley, context.viewport);
    for (int i = 0; i < 8; ++i) {
      BufferIO::writeColor(pixel.x, pixel.y + i, color, context.color_buffer, context.viewport.z);
    }
    for (int i = 0; i < 8; ++i) {
      BufferIO::writeColor(pixel.x + 7, pixel.y + i, color, context.color_buffer, context.viewport.z);
    }
    for (int i = 0; i < 8; ++i) {
      BufferIO::writeColor(pixel.x + i, pixel.y, color, context.color_buffer, context.viewport.z);
    }
    for (int i = 0; i < 8; ++i) {
      BufferIO::writeColor(pixel.x + i, pixel.y + 7, color, context.color_buffer, context.viewport.z);
    }
  }

  struct BinRasterizaion {
    static void run(const Context& context, const glm::ivec2& binid, const glm::ivec4& bounds, const glm::mat3& m) {
      // auto bin_bounds = BinTileSpace::binBounds(binid);

      // glm::vec4 color(binid.x / 15.f, binid.y / 15.f, 1.f, 1.f);
   		// for (auto y = bin_bounds.y; y < bin_bounds.w; ++y) {
      //   for (auto x = bin_bounds.x; x < bin_bounds.z; ++x) {
      //     BufferIO::writeColor(x, y, color, context.color_buffer, context.viewport.z);
      //   }
		  // }

      // auto lowerX = glm::clamp(BinTileSpace::insideTileX(binid, bounds.x),     0, BinTileSpace::TileNumX);
      // auto upperX = glm::clamp(BinTileSpace::insideTileX(binid, bounds.z) + 1, 0, BinTileSpace::TileNumX);
      // auto lowerY = glm::clamp(BinTileSpace::insideTileY(binid, bounds.y),     0, BinTileSpace::TileNumY);
      // auto upperY = glm::clamp(BinTileSpace::insideTileY(binid, bounds.w) + 1, 0, BinTileSpace::TileNumY);

      // BitMask tile_mask;
      // tile_mask.repeatRow(lowerX, upperX);
      // tile_mask.andStride(lowerY, upperY);

      // auto start = clipcoordsFromRaster(binid.x - 0.5f, binid.y - 0.5f, context.pixel_scale);

      // auto bin_top = BinTileSpace::top(binid.y);

      auto bin_space = BinTileSpace::clipcoordsFromRaster(binid, context.pixel_scale);

#ifdef DRAW_BIN_TILE
      drawBin(bin_space.start.x, bin_space.start.y, context);
#endif

      for (auto e = 0; e < 3; ++e) {
        auto edge = m[e];
        float invx = (edge.x != 0.f) ? 1.f / edge.x : 1.f / 0.000001f;
        int includeRight = edge.x < 0.f;

        for (auto row = 0; row < BinTileSpace::TileNumY; ++row) {
          auto y = bin_space.start.y + row * bin_space.tile_size.y;
          auto x = (-y * edge.y - edge.z) * invx;

          auto tile_col = bin_space.tileFromX(x);
          bin_space.markTileRow(row, tile_col, includeRight);

#ifdef DRAW_BIN_TILE
          if (tile_col >= 0 && tile_col < BinTileSpace::TileNumX) {
            drawTile(bin_space.start.x + tile_col * bin_space.tile_size.x, y, context);
          }
#endif
          if (binid.x == 0 && binid.y == 0) {
            if (row == 7 && e == 2) {
              printf("%d %d %d: %016llx\n", binid.x, binid.y, row, bin_space.tile_mask);
            }
          }
        }
      }
    }
  };
}

#endif
