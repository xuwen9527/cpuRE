#ifndef __TRIANGLE_BUFFER_H__
#define __TRIANGLE_BUFFER_H__

#include <glm/fwd.hpp>

#include <tuple>
#include <vector>

namespace cpuRE {
  class TriangleBuffer : public std::vector<std::tuple<glm::mat3, glm::vec3, glm::ivec4>> {
  public:
    using base_type = std::vector<std::tuple<glm::mat3, glm::vec3, glm::ivec4>>;
    TriangleBuffer() : base_type() {}

    void storeTriangle(const glm::mat3& m, const glm::vec3& uz, const glm::ivec4& bounds) {
      emplace_back(m, uz, bounds);
    }

    void loadTriangle(unsigned int i, glm::mat3& m, glm::vec3& uz, glm::ivec4& bounds) {
      auto& triangle = base_type::at(i);
      m = std::get<0>(triangle);
      uz = std::get<1>(triangle);
      bounds = std::get<2>(triangle);
    }
  };
}

#endif