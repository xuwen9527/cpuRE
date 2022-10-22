#ifndef __VIEWPORT_H__
#define __VIEWPORT_H__

#include <algorithm>
#include <glm/glm.hpp>

namespace cpuRE {
  static glm::ivec4 computeRasterBounds(const glm::vec2& bounds_min, const glm::vec2& bounds_max, const glm::vec4& viewport) {
    float vp_scale_x = 0.5f * viewport.z;
    float vp_scale_y = 0.5f * viewport.w;

    float x_min = std::max((bounds_min.x + 1.0f) * vp_scale_x + viewport.x, viewport.x);
    float y_min = std::max((bounds_min.y + 1.0f) * vp_scale_y + viewport.y, viewport.y);

    float x_max = std::min((bounds_max.x + 1.0f) * vp_scale_x + viewport.x, viewport.x + viewport.z);
    float y_max = std::min((bounds_max.y + 1.0f) * vp_scale_y + viewport.y, viewport.y + viewport.w);

    return glm::ivec4(
      std::ceil (x_min - 0.5f), std::ceil (y_min - 0.5f),
      std::floor(x_max + 0.5f), std::floor(y_max + 0.5f));
  }

  static glm::vec4 computePixelScale(const glm::vec4& viewport) {
    return {
      2.0f / viewport.z,
      2.0f / viewport.w,
      -1.0f - viewport.x * 2.0f / viewport.z + 1.0f / viewport.z,
      -1.0f - viewport.y * 2.0f / viewport.w + 1.0f / viewport.w
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
