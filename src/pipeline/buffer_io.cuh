#ifndef __BUFFERIO_H__
#define __BUFFERIO_H__

#include <algorithm>
#include <glm/glm.hpp>
#include <cuda_runtime.h>

namespace cpuRE::BufferIO {
  __device__
  static void writeColor(int x, int y, const glm::vec4& color, uint8_t* buffer, const int width) {
    constexpr int SCALE = (1 << (sizeof(uint8_t) * 8)) - 1;
    *(buffer + (y * width + x) * 4 + 0) = static_cast<uint8_t>(color.x * SCALE);
    *(buffer + (y * width + x) * 4 + 1) = static_cast<uint8_t>(color.y * SCALE);
    *(buffer + (y * width + x) * 4 + 2) = static_cast<uint8_t>(color.z * SCALE);
    *(buffer + (y * width + x) * 4 + 3) = static_cast<uint8_t>(color.w * SCALE);
  }

  __device__
  static glm::vec4 readColor(int x, int y, uint8_t* buffer, const int width) {
    constexpr float SCALE = 1.f / ((1 << (sizeof(uint8_t) * 8)) - 1);
    return {
      *(buffer + (y * width + x) * 4 + 0) * SCALE,
      *(buffer + (y * width + x) * 4 + 1) * SCALE,
      *(buffer + (y * width + x) * 4 + 2) * SCALE,
      *(buffer + (y * width + x) * 4 + 3) * SCALE
    };
  }

  __device__
  static void writeDepth(int x, int y, const float depth, uint16_t* buffer, const int width) {
    constexpr int SCALE = (1 << (sizeof(uint16_t) * 8)) - 1;
    *(buffer + y * width + x) = static_cast<uint16_t>(depth * SCALE);
  }

  __device__
  static float readDepth(int x, int y, uint16_t* buffer, const int width) {
    constexpr float SCALE = 1.f / ((1 << (sizeof(uint16_t) * 8)) - 1);
    return *(buffer + y * width + x) * SCALE;
  }
}

#endif