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

  void drawPixel(float x, float y, const Context& context) {
    if (x < 1.f && x >= -1.0 && y < 1.f && y >= -1.f) {
      auto pixel = rastercoordsFromClip(x, y, context.viewport);
      BufferIO::writeColor(pixel.x, pixel.y, { 1.f, 1.f, 1.f, 1.f}, context.color_buffer, context.viewport.z);
    }
  }

  struct BinRasterizaion {
    static void run(const Context& context, const glm::ivec2& binid, const glm::ivec4& bounds, const glm::mat3& m) {
      // auto bin_bounds = BinTileSpace::binBounds(binid);

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
        float invx = (edge.x != 0.f) ? 1.f / edge.x : -1.f / 0.000001f;
        int unset_right = edge.x < 0.f;
        int offset_row  = edge.y > 0.f;

	      for (auto tile_row = 0; tile_row < BinTileSpace::TileNumY; ++tile_row) {
          auto y = bin_space.start.y + (tile_row + offset_row) * bin_space.tile_size.y;
          auto x = (-y * edge.y - edge.z) * invx;

          auto tile_col = bin_space.tileFromX(x);
          tile_col += unset_right || tile_col < 0;
          bin_space.markTileRow(tile_row, tile_col);

// #ifdef DRAW_BIN_TILE
//           if (tile_col >= 0 && tile_col < BinTileSpace::TileNumX) {
//             drawTile(bin_space.start.x + tile_col * bin_space.tile_size.x, y, context);
//           }
// #endif
          drawPixel(x, y, context);
        }
      }

#ifdef DRAW_BIN_TILE
        for (int row = 0; row < BinTileSpace::TileNumY; ++row) {
          for (int col = 0; col < BinTileSpace::TileNumX; ++col) {
            auto tilebit = (1ULL << (BinTileSpace::TileNumX - 1 - col)) << (row * BinTileSpace::TileNumX);
            if (tilebit & bin_space.tile_mask) {
              drawTile(bin_space.start.x + col * bin_space.tile_size.x,
                       bin_space.start.y + row * bin_space.tile_size.y,
                       context);
            }
          }
        }
#endif
    }
  };
}

#endif
