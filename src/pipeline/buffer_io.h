#ifndef __BUFFERIO_H__
#define __BUFFERIO_H__

#include <algorithm>
#include <glm/glm.hpp>

namespace cpuRE::BufferIO {
  static void writeColor(int x, int y, const glm::vec4& color, unsigned char* color_buffer, const int width) {
    constexpr int SCALE = (1 << (sizeof(unsigned char) * 8)) - 1;
    *(color_buffer + (y * width + x) * 4 + 0) = static_cast<unsigned char>(color.x * SCALE);
    *(color_buffer + (y * width + x) * 4 + 1) = static_cast<unsigned char>(color.y * SCALE);
    *(color_buffer + (y * width + x) * 4 + 2) = static_cast<unsigned char>(color.z * SCALE);
    *(color_buffer + (y * width + x) * 4 + 3) = static_cast<unsigned char>(color.w * SCALE);
  }

  static glm::vec4 readColor(int x, int y, unsigned char* color_buffer, const int width) {
    constexpr float SCALE = 1.f / ((1 << (sizeof(unsigned char) * 8)) - 1);
    return {
      *(color_buffer + (y * width + x) * 4 + 0) * SCALE,
      *(color_buffer + (y * width + x) * 4 + 1) * SCALE,
      *(color_buffer + (y * width + x) * 4 + 2) * SCALE,
      *(color_buffer + (y * width + x) * 4 + 3) * SCALE
    };
  }

  static void writeDepth(int x, int y, const float depth,
                         unsigned short* depth_buffer, const int width) {
    constexpr int SCALE = (1 << (sizeof(unsigned short) * 8)) - 1;
    *(depth_buffer + y * width + x) = static_cast<unsigned short>(depth * SCALE);
  }

  static float readDepth(int x, int y,
                         unsigned short* depth_buffer, const int width) {
    constexpr float SCALE = 1.f / ((1 << (sizeof(unsigned short) * 8)) - 1);
    return *(depth_buffer + y * width + x) * SCALE;
  }
}

#endif