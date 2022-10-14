#ifndef __GEOMETRY_STAGE_H__
#define __GEOMETRY_STAGE_H__

#include <glm/gtx/matrix_operation.hpp>
#include "triangle_buffer.h"
#include "geometry.h"
#include "clipping.h"
#include "viewport.h"
#include "context.h"

namespace cpuRE {
  template <typename VertexShader>
  struct GeometryStage {
    static void triangle(const glm::vec4& p1, const glm::vec4& p2,
                         const glm::vec4& p3, Context& context,
                         TriangleBuffer& triangle_buffer) {
      glm::vec2 bounds_min, bounds_max;
      if (clipTriangle(p1, p2, p3, bounds_min, bounds_max)) {
        return;
      }

      if (bounds_min.x > 1.0f || bounds_max.x < -1.0f ||
          bounds_min.y > 1.0f || bounds_max.y < -1.0f) {
        return;
      }

      glm::ivec4 bounds = computeRasterBounds(bounds_min, bounds_max, context.viewport);

      glm::mat3 m(p1.x, p2.x, p3.x,
                  p1.y, p2.y, p3.y,
                  p1.w, p2.w, p3.w);

      auto det = glm::determinant(m);
      if (det <= 0.f)
        return;

      auto m_inv = glm::adjugate(m) * 1.f / det;
      auto uz = m_inv * glm::vec3(p1.z, p2.z, p3.z);

      triangle_buffer.storeTriangle(m_inv, uz, bounds);
    }

    static void run(Context& context, TriangleBuffer& triangle_buffer, Geometry& geometry) {
      VertexShader shader;
      triangle_buffer.reserve(geometry.primitives.size());

      for (auto primitive : geometry.primitives) {
        auto p0 = shader(context, geometry.vertices[primitive.x]);
        auto p1 = shader(context, geometry.vertices[primitive.y]);
        auto p2 = shader(context, geometry.vertices[primitive.z]);
        triangle(p0, p1, p2, context, triangle_buffer);
      }
    }
  };
}

#endif