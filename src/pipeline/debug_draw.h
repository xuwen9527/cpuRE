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

  void drawPixel(int x, int y, const glm::vec4& color, Context& context) {
    if (x < context.viewport.z && x >= 0 && y < context.viewport.w && y >= 0) {
      auto c = BufferIO::readColor(x, y, context.color_buffer, context.viewport.z);
      c = c * (1.f - color.w) + color * color.w;
      c.w = 1.f;
      BufferIO::writeColor(x, y, c, context.color_buffer, context.viewport.z);
    }
  }

  void drawBin(float binx, float biny, Context& context) {
    glm::vec4 color(1.f, 1.f, 1.f, 1.f);
    auto pixel = rastercoordsFromClip(binx, biny, context.viewport);
    for (int i = 0; i < BinTileSpace::BIN_HEIGHT; ++i) {
      auto x = pixel.x;
      auto y = pixel.y + i;
      drawPixel(x, y, color, context);
    }
    for (int i = 0; i < BinTileSpace::BIN_HEIGHT; ++i) {
      auto x = pixel.x + BinTileSpace::BIN_WIDTH - 1;
      auto y = pixel.y + i;
      drawPixel(x, y, color, context);
    }
    for (int i = 0; i < BinTileSpace::BIN_WIDTH; ++i) {
      auto x = pixel.x + i;
      auto y = pixel.y;
      drawPixel(x, y, color, context);
    }
    for (int i = 0; i < BinTileSpace::BIN_WIDTH; ++i) {
      auto x = pixel.x + i;
      auto y = pixel.y + BinTileSpace::BIN_HEIGHT - 1;
      drawPixel(x, y, color, context);
    }
  }

  void drawTile(float tilex, float tiley, Context& context) {
    glm::vec4 color((tilex + 1.f) * 0.5f, (tiley + 1.f) * 0.5f, 0.f, 1.f);
    auto pixel = rastercoordsFromClip(tilex, tiley, context.viewport);
    for (int i = 0; i < BinTileSpace::StampNumY; ++i) {
      auto x = pixel.x;
      auto y = pixel.y + i;
      drawPixel(x, y, color, context);
    }
    for (int i = 0; i < BinTileSpace::StampNumY; ++i) {
      auto x = pixel.x + BinTileSpace::StampNumX - 1;
      auto y = pixel.y + i;
      drawPixel(x, y, color, context);
    }
    for (int i = 0; i < BinTileSpace::StampNumX; ++i) {
      auto x = pixel.x + i;
      auto y = pixel.y;
      drawPixel(x, y, color, context);
    }
    for (int i = 0; i < BinTileSpace::StampNumX; ++i) {
      auto x = pixel.x + i;
      auto y = pixel.y + BinTileSpace::StampNumY - 1;
      drawPixel(x, y, color, context);
    }
  }

  void drawBounds(const glm::ivec4& bounds, Context& context) {
    glm::vec4 color(1.0f, 0.4f, 0.4f, 0.5f);
    auto pixel = glm::ivec2(bounds.x, bounds.y);
    for (int i = 0; i < bounds.w - bounds.y; ++i) {
      auto x = pixel.x;
      auto y = pixel.y + i;
      drawPixel(x, y, color, context);
    }
    for (int i = 0; i < bounds.w - bounds.y; ++i) {
      auto x = pixel.x + bounds.z - bounds.x - 1;
      auto y = pixel.y + i;
      drawPixel(x, y, color, context);
    }
    for (int i = 0; i < bounds.z - bounds.x; ++i) {
      auto x = pixel.x + i;
      auto y = pixel.y;
      drawPixel(x, y, color, context);
    }
    for (int i = 0; i < bounds.z - bounds.x; ++i) {
      auto x = pixel.x + i;
      auto y = pixel.y + bounds.w - bounds.y - 1;
      drawPixel(x, y, color, context);
    }
  }
}

#endif
