#ifndef __CLIPPING_H__
#define __CLIPPING_H__

#include <algorithm>
#include <glm/glm.hpp>

namespace cpuRE {
  static inline glm::vec3 hom_min(const glm::vec3& a, const glm::vec3& b) {
    return glm::vec3(std::min(a.x * b.z, b.x * a.z), std::min(a.y * b.z, b.y * a.z), a.z * b.z);
  }

  static inline glm::vec3 hom_max(const glm::vec3& a, const glm::vec3& b) {
   return glm::vec3(std::max(a.x * b.z, b.x * a.z), std::max(a.y * b.z, b.y * a.z), a.z * b.z);
  }

  static inline glm::vec2 proj(const glm::vec3& v) {
    return (1.0f / v.z) * glm::vec2(v.x, v.y);
  }

  static inline glm::vec3 clipEdgeNear(const glm::vec4& p1, const glm::vec4& p2) {
    float t = (-p1.w - p1.z) / (p2.z - p1.z + p2.w - p1.w);
    return glm::vec3(glm::mix(p1.x, p2.x, t), glm::mix(p1.y, p2.y, t), glm::mix(p1.w, p2.w, t));
  }

  static inline glm::vec3 clipEdgeFar(const glm::vec4& p1, const glm::vec4& p2) {
    float t = (p1.w - p1.z) / (p2.z - p1.z + p1.w - p2.w);
    return glm::vec3(glm::mix(p1.x, p2.x, t), glm::mix(p1.y, p2.y, t), glm::mix(p1.w, p2.w, t));
  }

  static inline glm::vec3 clipEdge(const glm::vec4& p1, const glm::vec4& p2, unsigned int outcode) {
    return outcode == 1U ? clipEdgeNear(p1, p2) : clipEdgeFar(p1, p2);
  }

  static inline unsigned int computeOutcode(const glm::vec4& p) {
    unsigned int near_code = p.z < -p.w ? 1U : 0U;
    unsigned int far_code = p.z > p.w ? 2U : 0U;
    return near_code | far_code;
  }

  static inline void clipCorner(const glm::vec4& p1, const glm::vec4& p2, const glm::vec4& p3, unsigned int outcode_1, unsigned int outcode_2, unsigned int outcode_3, glm::vec3& p_11, glm::vec3& p_12) {
    if (outcode_1 == 0U) {
      p_11 = p_12 = glm::vec3(p1.x, p1.y, p1.w);
      return;
    }

    if ((outcode_1 ^ outcode_2) != 0)
      p_11 = clipEdge(p1, p2, outcode_1);

    if ((outcode_1 ^ outcode_3) != 0)
      p_12 = clipEdge(p1, p3, outcode_1);

    if ((outcode_1 ^ outcode_2) == 0)
      p_11 = p_12;

    if ((outcode_1 ^ outcode_3) == 0)
      p_12 = p_11;
  }

  bool clipTriangle(const glm::vec4& p1, const glm::vec4& p2, const glm::vec4& p3,
                    glm::vec2& bounds_min, glm::vec2& bounds_max) {
    unsigned int outcode_1 = computeOutcode(p1);
    unsigned int outcode_2 = computeOutcode(p2);
    unsigned int outcode_3 = computeOutcode(p3);

    if ((outcode_1 ^ outcode_2) == 0U && (outcode_1 ^ outcode_3) == 0U) {
      glm::vec3 p1xyw(p1.x, p1.y, p1.w), p2xyw(p2.x, p2.y, p2.w), p3xyw(p3.x, p3.y, p3.w);
      bounds_min = proj(hom_min(p1xyw, hom_min(p2xyw, p3xyw)));
      bounds_max = proj(hom_max(p1xyw, hom_max(p2xyw, p3xyw)));
      return outcode_1 != 0U;
    }

    glm::vec3 p_11, p_12;
    clipCorner(p1, p2, p3, outcode_1, outcode_2, outcode_3, p_11, p_12);
    glm::vec3 p_21, p_22;
    clipCorner(p2, p3, p1, outcode_2, outcode_3, outcode_1, p_21, p_22);
    glm::vec3 p_31, p_32;
    clipCorner(p3, p1, p2, outcode_3, outcode_1, outcode_2, p_31, p_32);

    bounds_min = proj(hom_min(p_11, hom_min(p_12, hom_min(p_21, hom_min(p_22, hom_min(p_31, p_32))))));
    bounds_max = proj(hom_max(p_11, hom_max(p_12, hom_max(p_21, hom_max(p_22, hom_max(p_31, p_32))))));

    return false;
  }
}
#endif
