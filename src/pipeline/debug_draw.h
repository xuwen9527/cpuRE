#ifndef __DEBUG_DRAW_H__
#define __DEBUG_DRAW_H__

#include "context.h"
#include "viewport.h"

namespace cpuRE {
  void drawBin(float binx, float biny, Context& context) {
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

  void drawTile(float tilex, float tiley, Context& context) {
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

  void drawPixel(float x, float y, Context& context) {
    if (x < 1.f && x >= -1.0 && y < 1.f && y >= -1.f) {
      auto pixel = rastercoordsFromClip(x, y, context.viewport);
      BufferIO::writeColor(pixel.x, pixel.y, { 1.f, 1.f, 1.f, 1.f}, context.color_buffer, context.viewport.z);
    }
  }
}

#endif
