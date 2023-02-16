#ifndef __TRIANGLE_BUFFER_H__
#define __TRIANGLE_BUFFER_H__

#include <glm/fwd.hpp>

namespace cpuRE {
  class TriangleBuffer {
  public:
    static const uint32_t NUM = 1024u;

    __device__
    void allocate() {
      return atomicInc(&next_, NUM - 1);
    }

    __device__
    void store(uint32_t triangle_id, const glm::mat3& m, const glm::vec3& uz, const glm::ivec4& bounds) {
      auto triangle = get_triangle(triangle_id);

      triangle[0] = m[0][0]; triangle[1] = m[0][1]; triangle[2] = m[0][2];
      triangle[3] = m[1][0]; triangle[4] = m[1][1]; triangle[5] = m[1][2];
      triangle[6] = m[2][0]; triangle[7] = m[2][1]; triangle[8] = m[2][2];

      triangle[9]  = uz.x;
      triangle[10] = uz.y;
      triangle[11] = uz.z;

      triangle[12] = bounds.x;
      triangle[13] = bounds.y;
      triangle[14] = bounds.z;
      triangle[15] = bounds.w;
    }

    __device__
    void load(uint32_t triangle_id, glm::mat3& m, glm::vec3& uz, glm::ivec4& bounds) const {
      auto triangle = get_triangle(triangle_id);

      m[0][0] = triangle[0]; m[0][1]= triangle[1]; m[0][2] = triangle[2];
      m[1][0] = triangle[3]; m[1][1]= triangle[4]; m[1][2] = triangle[5];
      m[2][0] = triangle[6]; m[2][1]= triangle[7]; m[2][2] = triangle[8];

      uz.x = triangle[9];
      uz.y = triangle[10];
      uz.z = triangle[11];

      bounds.x = triangle[12];
      bounds.y = triangle[13];
      bounds.z = triangle[14];
      bounds.w = triangle[15];
    }

    void store(const glm::vec3& value) {

    }

  protected:
    static constexpr uint32_t SIZE = 16u;

    const float* get_triangle(uint32_t triangle_id) const {
      return data_ + triangle_id * SIZE;
    }

    float* get_triangle(uint32_t triangle_id) {
      return data_ + triangle_id * SIZE;
    }

    uint32_t next_ = 0u;
    float data_[SIZE * NUM];
  };
}

#endif