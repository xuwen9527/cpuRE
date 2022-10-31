#ifndef __DEBUG_DRAW_H__
#define __DEBUG_DRAW_H__

#include "context.h"
#include "viewport.h"
#include "buffer_io.h"

namespace cpuRE {
  void drawPixel(int x, int y, const glm::vec4& color, Context& context) {
    auto c = BufferIO::readColor(x, y, context.color_buffer, context.viewport.z);
    c = c * (1.f - color.w) + color * color.w;
    c.w = 1.f;
    BufferIO::writeColor(x, y, c, context.color_buffer, context.viewport.z);
  }

  void drawPixel(float x, float y, const glm::vec4& color, Context& context) {
    auto pixel = rastercoordsFromClip(x, y, context.viewport);
    drawPixel(pixel.x, pixel.y, color, context);
  }

  void drawBin(float binx, float biny, Context& context) {
    glm::vec4 color(1.f, 1.f, 1.f, 1.f);
    auto pixel = rastercoordsFromClip(binx, biny, context.viewport);

    auto width = BinTileSpace::BIN_WIDTH;
    auto height = BinTileSpace::BIN_HEIGHT;
    if ((pixel.x + width) >= context.viewport.z) {
      width = context.viewport.z - 1 - pixel.x;
    }
    if ((pixel.y + height) >= context.viewport.w) {
      height = context.viewport.w - 1 - pixel.y;
    }

    for (int i = 0; i < height; ++i) {
      auto x = pixel.x;
      auto y = pixel.y + i;
      drawPixel(x, y, color, context);
    }
    for (int i = 0; i < height; ++i) {
      auto x = pixel.x + width - 1;
      auto y = pixel.y + i;
      drawPixel(x, y, color, context);
    }
    for (int i = 0; i < width; ++i) {
      auto x = pixel.x + i;
      auto y = pixel.y;
      drawPixel(x, y, color, context);
    }
    for (int i = 0; i < width; ++i) {
      auto x = pixel.x + i;
      auto y = pixel.y + height - 1;
      drawPixel(x, y, color, context);
    }
  }

  void drawTile(float tilex, float tiley, Context& context) {
    glm::vec4 color((tilex + 1.f) * 0.5f, (tiley + 1.f) * 0.5f, 0.f, 1.f);
    auto pixel = rastercoordsFromClip(tilex, tiley, context.viewport);

    auto width = BinTileSpace::StampNumX;
    auto height = BinTileSpace::StampNumY;
    if ((pixel.x + width) >= context.viewport.z) {
      width = context.viewport.z - 1 - pixel.x;
    }
    if ((pixel.y + height) >= context.viewport.w) {
      height = context.viewport.w - 1 - pixel.y;
    }

    for (int i = 0; i < height; ++i) {
      auto x = pixel.x;
      auto y = pixel.y + i;
      drawPixel(x, y, color, context);
    }
    for (int i = 0; i < height; ++i) {
      auto x = pixel.x + width - 1;
      auto y = pixel.y + i;
      drawPixel(x, y, color, context);
    }
    for (int i = 0; i < width; ++i) {
      auto x = pixel.x + i;
      auto y = pixel.y;
      drawPixel(x, y, color, context);
    }
    for (int i = 0; i < width; ++i) {
      auto x = pixel.x + i;
      auto y = pixel.y + height - 1;
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
