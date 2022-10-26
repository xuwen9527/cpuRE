#ifndef __DEBUG_DRAW_H__
#define __DEBUG_DRAW_H__

#include "context.h"
#include "viewport.h"
#include "buffer_io.h"

namespace cpuRE {
  void drawPixel(float x, float y, const glm::vec4& color, Context& context) {
    if (x < 1.f && x >= -1.0 && y < 1.f && y >= -1.f) {
      auto pixel = rastercoordsFromClip(x, y, context.viewport);
      auto c = BufferIO::readColor(pixel.x, pixel.y, context.color_buffer, context.viewport.z);
      c = c * (1.f - color.w) + color * color.w;
      c.w = 1.f;
      BufferIO::writeColor(pixel.x, pixel.y, c, context.color_buffer, context.viewport.z);
    }
  }

  void drawBin(float binx, float biny, Context& context) {
    glm::vec4 color(1.f, 1.f, 1.f, 1.f);
    auto pixel = rastercoordsFromClip(binx, biny, context.viewport);
    for (int i = 0; i < BinTileSpace::BIN_HEIGHT; ++i) {
      auto x = pixel.x;
      auto y = pixel.y + i;
      if (x < context.viewport.z && y < context.viewport.w) {
        BufferIO::writeColor(x, y, color, context.color_buffer, context.viewport.z);
      }
    }
    for (int i = 0; i < BinTileSpace::BIN_HEIGHT; ++i) {
      auto x = pixel.x + BinTileSpace::BIN_WIDTH - 1;
      auto y = pixel.y + i;
      if (x < context.viewport.z && y < context.viewport.w) {
        BufferIO::writeColor(x, y, color, context.color_buffer, context.viewport.z);
      }
    }
    for (int i = 0; i < BinTileSpace::BIN_WIDTH; ++i) {
      auto x = pixel.x + i;
      auto y = pixel.y;
      if (x < context.viewport.z && y < context.viewport.w) {
        BufferIO::writeColor(x, y, color, context.color_buffer, context.viewport.z);
      }
    }
    for (int i = 0; i < BinTileSpace::BIN_WIDTH; ++i) {
      auto x = pixel.x + i;
      auto y = pixel.y + BinTileSpace::BIN_HEIGHT - 1;
      if (x < context.viewport.z && y < context.viewport.w) {
        BufferIO::writeColor(x, y, color, context.color_buffer, context.viewport.z);
      }
    }
  }

  void drawTile(float tilex, float tiley, Context& context) {
    glm::vec4 color((tilex + 1.f) * 0.5f, (tiley + 1.f) * 0.5f, 0.f, 1.f);
    auto pixel = rastercoordsFromClip(tilex, tiley, context.viewport);
    for (int i = 0; i < BinTileSpace::StampNumY; ++i) {
      auto x = pixel.x;
      auto y = pixel.y + i;
      if (x < context.viewport.z && y < context.viewport.w) {
        BufferIO::writeColor(x, y, color, context.color_buffer, context.viewport.z);
      }
    }
    for (int i = 0; i < BinTileSpace::StampNumY; ++i) {
      auto x = pixel.x + BinTileSpace::StampNumX - 1;
      auto y = pixel.y + i;
      if (x < context.viewport.z && y < context.viewport.w) {
        BufferIO::writeColor(x, y, color, context.color_buffer, context.viewport.z);
      }
    }
    for (int i = 0; i < BinTileSpace::StampNumX; ++i) {
      auto x = pixel.x + i;
      auto y = pixel.y;
      if (x < context.viewport.z && y < context.viewport.w) {
        BufferIO::writeColor(x, y, color, context.color_buffer, context.viewport.z);
      }
    }
    for (int i = 0; i < BinTileSpace::StampNumX; ++i) {
      auto x = pixel.x + i;
      auto y = pixel.y + BinTileSpace::StampNumY -1;
      if (x < context.viewport.z && y < context.viewport.w) {
        BufferIO::writeColor(x, y, color, context.color_buffer, context.viewport.z);
      }
    }
  }
}

#endif
