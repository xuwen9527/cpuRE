#ifndef __PIPELINE_H__
#define __PIPELINE_H__

#include "geometry_stage.h"
#include "rasterization_stage.h"

namespace cpuRE {
  template <typename VertexShader, typename FragmentShader, typename PrimitiveType>
  struct Pipeline {
    using GeometryStage = GeometryStage<VertexShader, PrimitiveType>;
    using RasterizationStage = BinTileRasterizationStage<FragmentShader>;

    static void run(const Geometry& geometry, Context& context) {
      TriangleBuffer triangle_buffer;
      GeometryStage::run(triangle_buffer, geometry, context);
      RasterizationStage::run(triangle_buffer, context);
    }
  };
}

#endif