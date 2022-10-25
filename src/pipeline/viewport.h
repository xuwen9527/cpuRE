#ifndef __VIEWPORT_H__
#define __VIEWPORT_H__

#include <algorithm>
#include <glm/glm.hpp>

namespace cpuRE {
  static glm::ivec4 computeRasterBounds(const glm::vec2& bounds_min, const glm::vec2& bounds_max, const glm::ivec4& viewport) {
    float vp_scale_x = 0.5f * viewport.z;
    float vp_scale_y = 0.5f * viewport.w;

    int x_min = (bounds_min.x + 1.0f) * vp_scale_x + viewport.x - 0.5f;
    int y_min = (bounds_min.y + 1.0f) * vp_scale_y + viewport.y - 0.5f;

    int x_max = (bounds_max.x + 1.0f) * vp_scale_x + viewport.x + 0.5f;
    int y_max = (bounds_max.y + 1.0f) * vp_scale_y + viewport.y + 0.5f;

    return glm::ivec4(
      std::max(x_min, viewport.x), std::max(y_min, viewport.y),
      std::min(x_max, viewport.x + viewport.z - 1), std::min(y_max, viewport.y + viewport.w - 1));
  }

  static glm::vec4 computePixelScale(const glm::vec4& viewport) {
    return {
      2.0f / viewport.z,
      2.0f / viewport.w,
      -1.0f - viewport.x * 2.0f / viewport.z,
      -1.0f - viewport.y * 2.0f / viewport.w
    };
  }

  static glm::vec3 clipcoordsFromRaster(int x, int y, const glm::vec4& pixel_scale) {
	  return { x * pixel_scale.x + pixel_scale.z, y * pixel_scale.y + pixel_scale.w, 1.0f };
  }

  static glm::ivec2 rastercoordsFromClip(float x, float y, const glm::vec4& viewport) {
    return { (x + 1.0f) * 0.5f * viewport.z + viewport.x, (y + 1.0f) * 0.5f * viewport.w + viewport.y };
  }

  struct RasterToClip {
    static glm::vec2 point(float x, float y, const glm::vec4& pixel_scale) {
      return { x * pixel_scale.x + pixel_scale.z, y * pixel_scale.y + pixel_scale.w };
    }

    static glm::vec2 vec(float x, float y, const glm::vec4& pixel_scale) {
      return { x * pixel_scale.x, y * pixel_scale.y };
    }
  };
}

#endif
