#ifndef __BUFFERIO_H__
#define __BUFFERIO_H__

#include <algorithm>
#include <glm/glm.hpp>

namespace cpuRE::BufferIO {
  static void writeColor(int x, int y, const glm::vec4& color, unsigned char* color_buffer, const int width) {
    *(color_buffer + (y * width + x) * 4 + 0) = static_cast<unsigned char>(color.x * 255.f);
    *(color_buffer + (y * width + x) * 4 + 1) = static_cast<unsigned char>(color.y * 255.f);
    *(color_buffer + (y * width + x) * 4 + 2) = static_cast<unsigned char>(color.z * 255.f);
    *(color_buffer + (y * width + x) * 4 + 3) = static_cast<unsigned char>(color.w * 255.f);
  }

  static glm::vec4 readColor(int x, int y, unsigned char* color_buffer, const int width) {
    return {
      *(color_buffer + (y * width + x) * 4 + 0) / 255.0f,
      *(color_buffer + (y * width + x) * 4 + 1) / 255.0f,
      *(color_buffer + (y * width + x) * 4 + 2) / 255.0f,
      *(color_buffer + (y * width + x) * 4 + 3) / 255.0f
    };
  }

  static void writeDepth(int x, int y, const float depth,
                         unsigned short* depth_buffer, const int width) {
    *(depth_buffer + y * width + x) = static_cast<unsigned short>(depth * 65535.f);
  }

  static float readDepth(int x, int y,
                         unsigned short* depth_buffer, const int width) {
    return *(depth_buffer + y * width + x) / 65535.f;
  }
}

#endif